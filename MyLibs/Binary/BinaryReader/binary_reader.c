#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "binary_reader.h"

BinaryReaderStruct* BinaryReader_Init(uint8_t* binaryData)
{
  BinaryReaderStruct* binaryReader = malloc(sizeof(BinaryReaderStruct));
  
  memset(binaryReader, 0, sizeof(BinaryReaderStruct));
  binaryReader->BinaryDataArray = binaryData;
  
  return binaryReader;
}

void BinaryReader_Destroy(BinaryReaderStruct* reader)
{
  free(reader);
}

bool BinaryReader_ReadBoolean(BinaryReaderStruct* reader)
{
  bool value = reader->BinaryDataArray[reader->CurrentReadPosition];
  reader->CurrentReadPosition += 1;
  return value;
}

uint8_t BinaryReader_ReadByte(BinaryReaderStruct* reader)
{
  uint8_t value = reader->BinaryDataArray[reader->CurrentReadPosition];
  reader->CurrentReadPosition += 1;
  return value;
}

uint8_t* BinaryReader_ReadBytes(BinaryReaderStruct* reader, int count)
{
  uint8_t* value = malloc(sizeof(uint8_t) * count);
  for (int i = 0; i < count; i++)
  {
    value[i] = reader->BinaryDataArray[i + reader->CurrentReadPosition];
  }
  
  reader->CurrentReadPosition += count;
  return value;
}

char BinaryReader_ReadChar(BinaryReaderStruct* reader)
{
  char value = reader->BinaryDataArray[reader->CurrentReadPosition];
  reader->CurrentReadPosition += 1;
  return value;
}

char* BinaryReader_ReadString(BinaryReaderStruct* reader)
{
  uint8_t count = reader->BinaryDataArray[reader->CurrentReadPosition];
  reader->CurrentReadPosition += 1;
  
  char* value = malloc(sizeof(char) * count + 1);
  for (int i = 0; i < count; i++)
  {
    value[i] = reader->BinaryDataArray[i + reader->CurrentReadPosition];
  }
  
  value[count + 1] = 0; // Завершаем строку.
  reader->CurrentReadPosition += count;
  return value;
}

uint16_t BinaryReader_ReadUInt16(BinaryReaderStruct* reader)
{
  uint16_t value = (reader->BinaryDataArray[reader->CurrentReadPosition]);
  value |= (reader->BinaryDataArray[reader->CurrentReadPosition + 1]  << 8);
  
  reader->CurrentReadPosition += 2;
  return value;
}

uint32_t BinaryReader_ReadUInt32(BinaryReaderStruct* reader)
{
  uint32_t value = (reader->BinaryDataArray[reader->CurrentReadPosition]);
  value |= (reader->BinaryDataArray[reader->CurrentReadPosition + 1]  << 8);
  value |= (reader->BinaryDataArray[reader->CurrentReadPosition + 2]  << 16);
  value |= (reader->BinaryDataArray[reader->CurrentReadPosition + 3]  << 24);
  
  reader->CurrentReadPosition += 4;
  return value;
}

double BinaryReader_ReadDouble(BinaryReaderStruct* reader)
{
//  double value = (reader->BinaryDataArray[reader->CurrentReadPosition] << 56);
//  value |= (double)(reader->BinaryDataArray[reader->CurrentReadPosition + 1] << 48);
//  value |= (double)(reader->BinaryDataArray[reader->CurrentReadPosition + 2] << 40);
//  value |= (double)(reader->BinaryDataArray[reader->CurrentReadPosition + 3] << 32);
//  value |= (double)(reader->BinaryDataArray[reader->CurrentReadPosition + 4] << 24);
//  value |= (double)(reader->BinaryDataArray[reader->CurrentReadPosition + 5] << 16);
//  value |= (double)(reader->BinaryDataArray[reader->CurrentReadPosition + 6] << 8);
//  value |= (double)(reader->BinaryDataArray[reader->CurrentReadPosition + 7]);
  double value = 0;
  reader->CurrentReadPosition += 8;
  return value;
}