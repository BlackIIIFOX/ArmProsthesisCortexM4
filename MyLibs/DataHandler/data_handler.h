#ifndef data_handler
#define data_handler

#include "stm32f4xx_hal.h"

//==============================================================================
//=================================Package Veriables============================
//==============================================================================
#define BUFFER_SIZE 16 // Max count in buffer 
#define BUFFER_MASK (BUFFER_SIZE-1) // Mask 

typedef struct{
  uint8_t* package;
  uint32_t countData;
} StructPackage;

typedef struct{
  uint32_t* buffer;
  unsigned char IndexStart;
  unsigned char IndexEnd;
} StructPackageBuffer;


//==============================================================================
//=================================Receive Veriables============================
//==============================================================================
#define startFiled "1N7ROINm"
#define endFiled "R{D98V89"
#define addressPC 0x00
#define addressHand 0x01
#define addressVoice 0x02
#define versionProtocol "0.1"
#define startFiled "1N7ROINm"
#define endFiled "R{D98V89"

typedef struct{
  uint32_t* next_addr;
  uint32_t* prev_addr;
  uint8_t data;
} StructReceiveData;

typedef struct{
  unsigned int count_data;
  uint8_t state_start_receive;
  uint8_t crc8;
  uint32_t count_main_field;
  StructReceiveData* list_receive_head;
  StructReceiveData* list_receive_last;
} StructListReceiveData;



//==============================================================================
//============================Receive Functions=================================
//==============================================================================
//API
StructPackage* Create_Transmit_Package(uint8_t distAddress, uint8_t* data, int count_data); // Создает новый пакет для передачи
void Create_Data_Receive(); //создание нового списка
void Clear_Data_Receive(); //освободить память
void Append_Data_Receive(uint8_t data); //добавить 
void Delete_Data_Receive(int index); // delete elem by index
void SetPackageBuf(StructPackageBuffer* newPackageBuf);

//Внутренние функции
void InitElemDataReceive(StructReceiveData* list); //иницилизировать структуру
unsigned char CheackStartField();
unsigned char CheackVersionField();
uint32_t ReadCountMainField();
unsigned char CheackStopField();
uint8_t* dataConvertToArray();
unsigned char CalculateCRC8(unsigned char *data, unsigned int Length);




//==============================================================================
//============================Package Functions=================================
//==============================================================================
#define BUFFER_SIZE 16 // Max count in buffer 
#define BUFFER_MASK (BUFFER_SIZE-1) // Mask 

void CreatePackageBuffer(StructPackageBuffer* packageBuffer);
unsigned char PackageBufAdd(StructPackageBuffer* packageBuffer, StructPackage* package);
StructPackage* PackageBufGet(StructPackageBuffer* packageBuffer);
unsigned char PackageBufferCount(StructPackageBuffer* packageBuffer);
unsigned char PackageBufferState(StructPackageBuffer* packageBuffer);

//==============================================================================

#endif