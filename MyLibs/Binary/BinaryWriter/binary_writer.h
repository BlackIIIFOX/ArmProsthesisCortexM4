#ifndef __BINARY_WRITER_H
#define __BINARY_WRITER_H

#include "stdbool.h"
#include "stdint.h"
 
#define BASE_SIZE_BUFFER 16

typedef struct {
  unsigned int CurrentWritePosition;
  unsigned int CurrentSizeBuffer;
  uint8_t* Buffer;
} BinaryWriterStruct;

/**
* @brief Инициализирует новый экземпляр BinaryWriterStruct.
* @retval Новый экземпляр BinaryWriterStruct.
*/
BinaryWriterStruct* BinaryWriter_Init();

/**
* @brief Записывает содержимое потока в массив байт.
* Память для массива него выделяется динамически.
* @param writer объект, хранящий поток для записи.
* @param outSizeArray размер массива байт, в который бал копирован поток. 
* Может использоваться пользователем для определения длины массива.
* @retval Новый массив байтов.
*/
uint8_t* BinaryWriter_ToArray(BinaryWriterStruct* writer, int* outSizeArray);

/**
* @brief Освобождает все ресурсы, используемые переданным BinaryWriterStruct.
* @param writer особождаемый BinaryWriterStruct.
* @retval None.
*/
void BinaryWriter_Destroy(BinaryWriterStruct* writer);

/**
* @brief Записывает однобайтовое Boolean значение в текущий поток, представляющий 0 false
* и 1, представляющие true.
* @param writer объект, хранящий поток для записи.
* @param value Boolean Значение для записи (0 или 1).
* @retval None.
*/
void BinaryWriter_WriteBoolean(BinaryWriterStruct* writer, bool value);

/**
* @brief Записывает байт без знака в текущий поток и перемещает позицию в потоке на один
* байт.
* @param value Boolean Значение для записи (0 или 1).
* @retval None.
*/
void BinaryWriter_WriteByte(BinaryWriterStruct* writer, uint8_t value);

/**
* @brief Записывает области массива байтов в текущий поток.
* текущую позицию на это количество байтов.
* @param writer объект, хранящий поток для записи.
* @param buffer Массив байтов, содержащий данные для записи.
* @param count Количество записываемых байтов.
* @retval None.
*/
void BinaryWriter_WriteBytes(BinaryWriterStruct* writer, uint8_t* buffer, int count);

/**
* @brief  Записывает знак в текущий поток и перемещает текущую позицию в потоке.
* @param writer объект, хранящий поток для записи.
* @param value Символ Юникода для записи.
* @retval None.
*/
void BinaryWriter_WriteChar(BinaryWriterStruct* writer, char value);

/**
* @brief  Записывает строку. Строка предваряется значением длины строки,
* которое закодировано как краткое целое беззнаковое число.
* @param writer объект, хранящий поток для записи.
* @param value Значение для записи.
* @param lenght Длина строки.
* @retval None.
*/
void BinaryWriter_WriteString(BinaryWriterStruct* writer, char* value, uint8_t lenght);

/**
* @brief Записывает четырехбайтовое целое число без знака в текущий поток и перемещает
* в потоке на два байта.
* @param writer объект, хранящий поток для записи.
* @param value Двухбайтовое целое число со знаком для записи.
* @retval None.
*/
void BinaryWriter_WriteUInt16(BinaryWriterStruct* writer, uint16_t value);

/**
* @brief Считывает целое число без знака длиной 4 байта в формате с прямым порядком байтов,
* позицию в потоке на четыре байта вперед.
* @param writer объект, хранящий поток для записи.
* @param value Четырехбайтовое целое число без знака для записи.
* @retval None.
*/
void BinaryWriter_WriteUInt32(BinaryWriterStruct* writer, uint32_t value);

/**
* @brief Записывает двухбайтное целое число со знаком в текущий поток и перемещает позицию
* в потоке на два байта.
* @param writer объект, хранящий поток для записи.
* @param value Двухбайтовое целое число со знаком для записи.
* @retval None.
*/
void BinaryWriter_WriteInt16(BinaryWriterStruct* writer, int16_t value);

/**
* @brief Записывает четырехбайтовое целое число со знаком в текущий поток и перемещает
* позицию в потоке на четыре байта вперед.
* @param writer объект, хранящий поток для записи.
* @param value Четырехбайтовое целое число со знаком для записи.
* @retval None.
*/
void BinaryWriter_WriteInt32(BinaryWriterStruct* writer, int32_t value);

/**
* @brief Записывает значение с плавающей запятой 4 байта в текущий поток и перемещает
* позицию в потоке на четыре байта вперед.
* @param writer объект, хранящий поток для записи.
* @param value Записываемое значение с плавающей запятой 4 байта.
* @retval None.
*/
void BinaryWriter_WriteSingle(BinaryWriterStruct* writer, float value);

/**
* @brief Внутренний интерфейс. Функция должна вызываться при любом добавлении новых элементов в поток.
* Обеспечивает внутренний буфер требуемой размерностью.
* Если текущий размер буфера не удовлетворяет требуемой минимальной размерност,
* то выполняется динамическое выделение нового буфера увеличенной размерности.
* Новый выполняет deep copy старого буфера и заменяет старый буфер в структуре.
* Старый буфер при этом очищается.
* @param writer объект, хранящий поток для записи.
* @param minCapacity Новая минимальная размерность внутреннего буфера.
*/
void BinaryWriter_EnsureCapacityInternal(BinaryWriterStruct* writer, int minCapacity);

/**
* @brief Внутренний интерфейс. Выполняет расширение буфера до требуемой длины.
* @param writer объект, хранящий поток для записи.
* @param minCapacity Новая минимальная размерность внутреннего буфера.
*/
void BinaryWriter_GrowBuffer(BinaryWriterStruct* writer, int minCapacity);


#endif