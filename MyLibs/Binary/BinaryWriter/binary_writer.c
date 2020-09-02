#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "binary_writer.h"

BinaryWriterStruct* BinaryWriter_Init()
{
  BinaryWriterStruct* binaryWriter = malloc(sizeof(BinaryWriterStruct));
  binaryWriter->CurrentSizeBuffer = BASE_SIZE_BUFFER;
  binaryWriter->Buffer = malloc(sizeof(uint8_t) * BASE_SIZE_BUFFER);
  binaryWriter->CurrentWritePosition = 0;
  
  return binaryWriter;
}

void BinaryWriter_Destroy(BinaryWriterStruct* writer)
{
  free(writer->Buffer);
  free(writer);
}

void BinaryWriter_EnsureCapacityInternal(BinaryWriterStruct* writer, int minCapacity)
{
  if (writer->CurrentSizeBuffer < minCapacity)
  {
    BinaryWriter_GrowBuffer(writer, minCapacity);
  }
}

void BinaryWriter_GrowBuffer(BinaryWriterStruct* writer, int minCapacity)
{
  // overflow-conscious code
  int newCapacity = writer->CurrentSizeBuffer;
 
  while(1)
  {
    newCapacity = newCapacity + (newCapacity >> 1);
    
    if (newCapacity - minCapacity >= 0)
    {
      break;
    }
  }
  
  // minCapacity is usually close to size, so this is a win:
  uint8_t* oldBuffer = writer->Buffer;
  uint8_t* newBuffer = malloc(sizeof(uint8_t) * newCapacity);
  memcpy(newBuffer, oldBuffer, writer->CurrentWritePosition);
  writer->Buffer = newBuffer;
  writer->CurrentSizeBuffer = newCapacity;
  free(oldBuffer);
}

void BinaryWriter_WriteBoolean(BinaryWriterStruct* writer, bool value)
{
  BinaryWriter_EnsureCapacityInternal(writer, writer->CurrentWritePosition + 1);
  writer->Buffer[writer->CurrentWritePosition] = value;
  writer->CurrentWritePosition += 1;
}

void BinaryWriter_WriteByte(BinaryWriterStruct* writer, uint8_t value)
{
  BinaryWriter_EnsureCapacityInternal(writer, writer->CurrentWritePosition + 1);
  writer->Buffer[writer->CurrentWritePosition] = value;
  writer->CurrentWritePosition += 1;
}

void BinaryWriter_WriteBytes(BinaryWriterStruct* writer, uint8_t* buffer, int count)
{
  BinaryWriter_EnsureCapacityInternal(writer, writer->CurrentWritePosition + count);
  
  for (int i = 0; i < count; i++)
  {
    writer->Buffer[writer->CurrentWritePosition + i] = buffer[i];
  }
  
  writer->CurrentWritePosition += count;
}

void BinaryWriter_WriteChar(BinaryWriterStruct* writer, char value)
{
  BinaryWriter_EnsureCapacityInternal(writer, writer->CurrentWritePosition + 1);
  writer->Buffer[writer->CurrentWritePosition] = value;
  writer->CurrentWritePosition += 1;
}

void BinaryWriter_WriteString(BinaryWriterStruct* writer, char* value, uint8_t lenght)
{
  BinaryWriter_EnsureCapacityInternal(writer, writer->CurrentWritePosition + lenght + 1);
  
  writer->Buffer[writer->CurrentWritePosition] = lenght;
  writer->CurrentWritePosition += 1;
  
  for (int i = 0; i < lenght; i++)
  {
    writer->Buffer[writer->CurrentWritePosition + i] = value[i];
  }
  
  writer->CurrentWritePosition += lenght;
}

void BinaryWriter_WriteUInt16(BinaryWriterStruct* writer, uint16_t value)
{
  BinaryWriter_EnsureCapacityInternal(writer, writer->CurrentWritePosition + 2);
  writer->Buffer[writer->CurrentWritePosition] = value;
  writer->Buffer[writer->CurrentWritePosition + 1] = value >> 8;
  writer->CurrentWritePosition += 2;
}

void BinaryWriter_WriteUInt32(BinaryWriterStruct* writer, uint32_t value)
{
  BinaryWriter_EnsureCapacityInternal(writer, writer->CurrentWritePosition + 4);
  writer->Buffer[writer->CurrentWritePosition] = value;
  writer->Buffer[writer->CurrentWritePosition + 1] = value >> 8;
  writer->Buffer[writer->CurrentWritePosition + 2] = value >> 16;
  writer->Buffer[writer->CurrentWritePosition + 3] = value >> 24;
  writer->CurrentWritePosition += 4;
}

uint8_t* BinaryWriter_ToArray(BinaryWriterStruct* writer, int* outSizeArray)
{
  int sizeOutBuffer = writer->CurrentWritePosition - 1;
  uint8_t* array = malloc(sizeof(uint8_t) * sizeOutBuffer);
  memcpy(array, writer->Buffer, sizeOutBuffer);
  *outSizeArray = sizeOutBuffer;
  return array;
}