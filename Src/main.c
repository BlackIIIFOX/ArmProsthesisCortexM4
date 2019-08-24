/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
******************************************************************************
* @attention
*
* <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
* All rights reserved.</center></h2>
*
* This software component is licensed by ST under BSD 3-Clause license,
* the "License"; You may not use this file except in compliance with the
* License. You may obtain a copy of the License at:
*                        opensource.org/licenses/BSD-3-Clause
*
******************************************************************************
*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "data_handler.h"
#include "ssd1306.h"
#include "fonts.h"
#include "stdbool.h"
#include "list.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* Тип режима работы протеза. Протез может работать в трех режимах, 
которые могут переключаться как с помощью внешних воздействий, так и автоматически. */
enum TypeWork {
  /* Миоэлектрический режим работы протеза. В данном режиме выбор исполняемого 
  жеста основывается только на миоэлектрических датчиках. */
  MyoelectricMode,
  
  /* Режим прямого управления. В данном режиме выбор исполняемого 
  жеста основывается на принимаемых командах с внешнего устройства. */
  ManualMode,
  
  /* Смешанный режим работы протеза. В данном режиме выбор исполняемого 
  жеста основывается как на миоэлектрических датчиках, 
  так и на принимаемых с внешнего устройства командах. */
  MixedMode
};

/* Информация о жесте. */
typedef struct {
  
  /* Время изменения жеста. */
  long TimeChange;
  
  /* Состояние итерируемости жеста. */
  bool IterableGesture;
  
  /* Количество повторений жеста. */
  uint8_t NumberOfGestureRepetitions;
  
  /* Кол-во действий в жесте. */
  uint8_t NumberOfMotions;
  
} InfoGestureModel;

/* Единичное действие жеста. Содержит поции пальцев и кисти, а так же задержку, 
необходимую перед следующим действием. */
typedef struct {
  
  /* Положение указательного пальца в градусах (0-180). */
  uint8_t PointerFinger;
  
  /* Положение среднего пальца в градусах (0-180). */
  uint8_t MiddleFinger;
  
  /* Положение безымянного пальца в градусах (0-180). */
  uint8_t RingFinder;
  
  /* Положение мезинца в градусах (0-180). */
  uint8_t LittleFinger;
  
  /* Положение большого пальца в градусах (0-180). */
  uint8_t ThumbFinger;
  
  /* Положение кисти в градусах (0-350). */
  uint16_t StatePosBrush;
  
  /* Задержка между действиями в милисекундах. */
  uint16_t DelMotion;
  
} MotionModel;

/* Жест, исполняемый протезом */
typedef struct {
  
  /* Id жеста в Guid. */
  uint8_t Id[16];
  
  /* Имя жеста */
  char* Name;
  
  /* Информация о жесте. */
  InfoGestureModel InfoGesture;
  
  /* Коллекция действий жеста. */
  list ListMotions;
  
} GestureModel;

/* Информация о текущей конфигурации и работе протеза. */
typedef struct {
  //  
  /* Состояние подключения Emg датчика №1. */
  bool StateEnableEmg1;
  
  /* Состояние подключения Emg датчика №2. */
  bool StateEnableEmg2;
  
  /* Состояние подключения дисплея. */
  bool StateDisplay;
  
  /* Состояние подключения драйвера моторов. */
  bool StateMotorDriver;
  
  /* Состояние подключения внешнего устройства по bluetooth. */
  bool StateBluetoothConnect;
  
  /* Состояние подключения SD карты. */
  bool StateSDCard;
  
  /* Время последней синхронизации. В Unix времени. */
  long LastTimeSync;
  
  /* Текущий режим работы протеза. */
  enum TypeWork OperatingMode;
  
  /* Текущее положение протеза. */
  MotionModel ProsthesisCurrentPosition;
  
  /* Текущий список жестов протеза. */
  list ListMotions;
  
} ConfigModel;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TimeDelayInitMs 500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* CAN PV */

/* Текущая конфигурация протеза. */
ConfigModel configuration;

/* Кол-во милисекунд, прошедших со времени старта МК. */
uint64_t TimeFromStartInMillis;

/* Кол-во милисекунд, продеших со времени последнего приема с драйвера моторов. */
uint64_t TimeLastReceiveMotorDriverInMillis;

/* CAN bus filter configuration. */
CAN_FilterTypeDef sFilterConfig;

/* CAN bus header transmit configuration. */
CAN_TxHeaderTypeDef TxHeader;

/* CAN bus header receive configuration. */
CAN_RxHeaderTypeDef RxHeader;

/* CAN transmit data. */
uint8_t TxData[8];

/* CAN receive data. */
uint8_t RxData[8];

/* CAN number buffer for receive and trasnmit. */
uint32_t TxMailbox;

/* FATFS PV */

/* File system object for SD card logical drive */
FATFS SDFatFs;

/* File object. */
FIL MyFile;

/* SD logical drive path. */
extern char SDPath[4];

/* SD file open result. */
FRESULT fr;

// Time system
uint32_t time_last_receive = 0;

// For receive
uint8_t dataRx;
StructPackageBuffer receivePackage;
StructPackageBuffer transmitPackage;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* Объявление функций. */

/* Реализация функций. */


/** 
* @brief Выполняет инициализацию Emg датчика №1. А именно провреяет состояние пина,
* к которому подключен штекер, определяющий подключение Emg1.
* Устанавливает configuration.StateEnableEmg1 в true, если подключен 
* и выводит информацию о успешном подключении.
* @retval None.
*/
bool InitEmg1()
{
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_SET;
}

/** 
* @brief Выполняет инициализацию Emg датчика №2. А именно провреяет состояние пина,
* к которому подключен штекер, определяющий подключение Emg2. 
* Устанавливает configuration.StateEnableEmg2 в true, если подключен 
* и выводит информацию о успешном подключении.
* @retval None.
*/
bool InitEmg2()
{
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET;
}

/** 
* @brief Выполняет отображение сообщения ни дисплее по заданным 
* координатам пикселей. Координаты представляют из себя пиксели на дисплее.
* @param message Выводимое на дисплей сообщение.
* @param posX Координата x для вывода сообщения.
* @param posY Координата y для вывода сообщения.
* @retval None.
*/
void Display_Print(char* message, int posX, int posY)
{
  if (configuration.StateDisplay)
  {
    ssd1306_Fill(Black);
    ssd1306_SetCursor(posX,posY);
    ssd1306_WriteString(message, Font_11x18, White);
    ssd1306_UpdateScreen();
  }
}

void RecognizePackage(StructPackage* recPackage)
{
  switch ( recPackage->package[0] ) 
  {
  case 0x01:
    {
      HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
      break;
    }
  case 0x02:
    {
      ssd1306_Fill(White);
      ssd1306_SetCursor(0,23);
      ssd1306_WriteString((char*)(&(recPackage->package[1])),Font_11x18,Black);
      ssd1306_UpdateScreen();
      break;
    }    
  }
  
  free(recPackage->package);
  free(recPackage);
}

/** 
* @brief Проверка подключения какого либо устройства по I2C шине.
* @param address Адрес проверяемого устройства на шине I2C.
* @retval Состояние подключения устройства. True - подключено.
*/
bool CheckI2CDevice(uint8_t address) 
{
  HAL_StatusTypeDef res;
  res = HAL_I2C_IsDeviceReady(&hi2c1, address, 5, 10);
  if(res == HAL_OK) 
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
* @brief Выполняет необходимую программную иницилизацию CAN и 
* переменных для работы с ней.
* @retval Успешность инициализации CAN шины.
*/
bool CAN_Init()
{
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0X0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
    return false;
  }
  
  if (HAL_CAN_Start(&hcan1) != HAL_OK)
  {
    Error_Handler();
    return false;
  }
  
  if (HAL_CAN_ActivateNotification (&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
  {
    Error_Handler();
    return false;
  }
  
  TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.TransmitGlobalTime = DISABLE;
  TxHeader.DLC = 8;
  TxData[0] = 1;
  TxData[1] = 2;
  TxData[2] = 3;
  TxData[3] = 4;
  TxData[4] = 5;
  TxData[5] = 6;
  TxData[6] = 7;
  TxData[7] = 8;

  //HAL_CAN_Receive_IT(&hcan1, 0);
  
  return true;
}

/**
* @brief Выполняет необходимую программную иницилизацию CAN шины и провреяет
* состояние подключения драйвера моторов, ожидая приход состояния по CAN шине
* в течении 3 секунд.
* @retval Состояние подключения драйвера моторов.
*/
bool DriverMotorInit()
{
  /* Сначала иницилизациия CAN шины для обмена с контроллером моторов. */
  if (CAN_Init())
  {
    uint64_t timeStartWait = TimeFromStartInMillis;
    while (TimeFromStartInMillis - timeStartWait < 3000)
    {
      if (TimeLastReceiveMotorDriverInMillis != 0)
      {
        return true;
      }
    }
  }
  
  return false;
}

/**
* @brief Выполняет иницилизацию дисплея.
* @retval Успешность иницилизации дисплея.
*/
bool Display_Init()
{
  /* Проврека состояния подключения дисплея и иницилизация.*/
  if (CheckI2CDevice(SSD1306_I2C_ADDR))
  {
    ssd1306_Init();
    return true;
  }
  
  return false;
}

/**
* @brief Выполняет необходимую программную иницилизацию Flash карты и 
* переменных для работы с ней.
* @retval None
*/
bool Flash_Init()
{
  volatile FRESULT res;
  res =  f_mount(&SDFatFs, (TCHAR const*)SDPath, 1);
  
  if (res != FR_OK)
  {
    BYTE work[_MIN_SS]; /* Work area (larger is better for processing time) */
    if (res == FR_NO_FILESYSTEM)
    { 
      res = f_mkfs((TCHAR const*)SDPath, FM_ANY , 0, work, sizeof(work));
      
      res =  f_mount(&SDFatFs, (TCHAR const*)SDPath, 1);
      if (res == FR_OK)
      {
        return true;
      }
    }
    
    return false;
  }
  else
  {
    return true;
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  for (int i = 0; i < 50000; i++) 
  {
  }
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_CAN1_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  
  /* Запуск таймера для расчет времени со старта системы. */
  HAL_TIM_Base_Start_IT(&htim4);
  
  /* Иницилизация дисплея и вывод сообщения о состоянии запуска системы. */
  HAL_Delay(3000);
  if (Display_Init())
  {
    configuration.StateDisplay = true;
  }
  
  HAL_Delay(TimeDelayInitMs);
  Display_Print("Start", 40, 30);
  HAL_Delay(TimeDelayInitMs);
  
  /* Проверка подключения EMG датчиков. */
  if (InitEmg1())
  {
    configuration.StateEnableEmg1 = true;
    Display_Print("EMG1 init", 0, 30);
    HAL_Delay(TimeDelayInitMs);
  }
  
  if (InitEmg2())
  {
    configuration.StateEnableEmg2 = true;
    Display_Print("EMG2 init", 0, 30);
    HAL_Delay(TimeDelayInitMs);
  }
  
  /* Инициализация драйвера моторов. */
  if (DriverMotorInit())
  {
    configuration.StateMotorDriver = true;
    Display_Print("Motor init", 0, 30);
    HAL_Delay(TimeDelayInitMs);
  }
  else
  {
    Display_Print("Motor error", 0, 30);
    Error_Handler();
    while(1);
  }
  
  /* Иницилизация протоколо приемо-передачи. */
  Create_Data_Receive();
  CreatePackageBuffer(&receivePackage);
  CreatePackageBuffer(&transmitPackage);
  SetPackageBuf(&receivePackage);
  HAL_UART_Receive_IT(&huart2, &dataRx, 1);
  
  /* Иницилизация SD карты. */
  if (Flash_Init())
  {
    configuration.StateSDCard = true;
    Display_Print("SD init", 0, 30);
    HAL_Delay(TimeDelayInitMs);
  }
  else
  {
    Display_Print("SD error", 0, 30);
    Error_Handler();
    while(1);
  }
  
  Display_Print("Success", 0, 30);
  HAL_Delay(5000);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint8_t str[] = "USART Transmit\r\n";
    HAL_UART_Transmit(&huart2,str,16,0xFFFF);
    
    //HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15); 
    
    //    if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1))
    //    {
    //      HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
    //    }
    
    if(PackageBufferCount(&receivePackage) > 0)
    {
      StructPackage* currentPackage = PackageBufGet(&receivePackage);
      RecognizePackage(currentPackage);
    }
    
    HAL_StatusTypeDef status =  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
    TxData[7] = TxData[7] + 1;
    HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
* @brief  Обработчик принятия сообщения по CAN.
* @param  hcan CAN, вызвавший прерывание.
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData);
  TimeLastReceiveMotorDriverInMillis = TimeFromStartInMillis;
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15); 
}

/**
* @brief  Обработчик принятия сообщения по UART.
* @param  huart UART, вызвавший прерывание.
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2)
  {
    uint32_t time = HAL_GetTick();
    if((time - time_last_receive) > 1000)
    {
      Clear_Data_Receive();
    }
    
    Append_Data_Receive(dataRx);
    time_last_receive = time;
    HAL_UART_Receive_IT(&huart2, &dataRx, 1);
  }
}

/* 
* Прерывание таймеров.
* htim4 отвечает за тактирование времени со старта системы и прерывается кажду 1мс.
* @param htim таймер, вызвавший прерывание.
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
  if (htim == &htim4)
  {
    TimeFromStartInMillis++;
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15,GPIO_PIN_SET); 
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
  tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
