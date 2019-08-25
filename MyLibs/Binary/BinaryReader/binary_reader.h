#ifndef __BINARY_READER_H
#define __BINARY_READER_H

#include "stdbool.h"
#include "stdint.h"
#include "stdbool.h"

/* TODO: можно спокойно продолжать чтение, даже если вышли за размерность потока для чтения. */

typedef struct {
  unsigned int CurrentReadPosition;
  uint8_t* BinaryDataArray;
} BinaryReaderStruct;
  

/**
* @brief Инициализирует новый экземпляр BinaryReaderStruct.
* @param binaryData Входной поток в виде массива байт.
*/
BinaryReaderStruct* BinaryReader_Init(uint8_t* binaryData);

/**
* @brief Освобождает все ресурсы, используемые переданным BinaryReaderStruct.
* @param reader особождаемый BinaryReaderStruct.
*/
void BinaryReader_Destroy(BinaryReaderStruct* reader);

/**
* @brief Считывает значение Boolean из текущего потока и перемещает текущую позицию в
* потоке на один байт вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Значение true, если байт не равен нулю; в противном случае — значение false.
*/
bool BinaryReader_ReadBoolean(BinaryReaderStruct* reader);

/**
* @brief Считывает из текущего потока следующий байт и перемещает текущую позицию в потоке
* на один байт вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Следующий байт, считанный из текущего потока.
*/
uint8_t BinaryReader_ReadByte(BinaryReaderStruct* reader);

/**
* @brief Считывает указанное количество байтов из текущего потока в массив байтов и перемещает
* текущую позицию на это количество байтов.
* @param reader объект, хранящий поток для чтения.
* @retval Массив байтов, в котором содержатся данные, считанные из базового потока.
*/
uint8_t BinaryReader_ReadBytes(BinaryReaderStruct* reader, int count);

/**
* @brief Считывает следующий знак из текущего потока и изменяет текущую позицию в потоке
* на один байт вперед. Читаемые знаким могут занимать только один байт.
* @param reader объект, хранящий поток для чтения.
* @retval Символ, считанный из текущего потока.
*/
char BinaryReader_ReadChar(BinaryReaderStruct* reader);

/**
* @brief Считывает строку из текущего потока. Строка предваряется значением длины строки,
* которое закодировано как краткое целое беззнаковое число.
* @param reader объект, хранящий поток для чтения.
* @retval Считываемая строка.
*/
char* BinaryReader_ReadString(BinaryReaderStruct* reader);

/**
* @brief Считывает целое число без знака длиной 2 байта в формате с прямым порядком байтов,
* из текущего потока и перемещает текущую позицию в потоке на два байта вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Целое число без знака длиной 2 байта, считанное из текущего потока.
*/
char BinaryReader_ReadUInt16(BinaryReaderStruct* reader);

/**
* @brief Считывает целое число без знака длиной 4 байта в формате с прямым порядком байтов,
* из текущего потока и перемещает текущую позицию в потоке на четыре байта вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Целое число без знака длиной 4 байта, считанное из текущего потока.
*/
char BinaryReader_ReadUInt32(BinaryReaderStruct* reader);

/**
* @brief Считывает целое число со знаком длиной 2 байта в формате с прямым порядком байтов,
* из текущего потока и перемещает текущую позицию в потоке на два байта вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Целое число со знаком длиной 2 байта, считанное из текущего потока.
*/
char BinaryReader_BinaryReader_ReadInt16(BinaryReaderStruct* reader);

/**
* @brief Считывает целое число со знаком длиной 4 байта из текущего потока и перемещает
* из текущего потока и перемещает текущую позицию в потоке на четыре байта вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Целое число со знаком длиной 2 байта, считанное из текущего потока.
*/
char BinaryReader_ReadInt32(BinaryReaderStruct* reader);

/**
* @brief Считывает число с плавающей запятой длиной 8 байт из текущего потока и перемещает
* текущую позицию в потоке на восемь байт вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Число с плавающей запятой длиной 8 байт, считанное из текущего потока.
*/
char BinaryReader_ReadDouble(BinaryReaderStruct* reader);

/**
* @brief Считывает число с плавающей запятой длиной 4 байта из текущего потока и перемещает
* текущую позицию в потоке на четыре байта вперед.
* @param reader объект, хранящий поток для чтения.
* @retval Число с плавающей запятой длиной 4 байта, считанное из текущего потока.
*/
char BinaryReader_ReadSingle(BinaryReaderStruct* reader);

#endif