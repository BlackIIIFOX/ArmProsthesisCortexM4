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
* @brief �������������� ����� ��������� BinaryWriterStruct.
* @retval ����� ��������� BinaryWriterStruct.
*/
BinaryWriterStruct* BinaryWriter_Init();

/**
* @brief ���������� ���������� ������ � ������ ����.
* ������ ��� ������� ���� ���������� �����������.
* @param writer ������, �������� ����� ��� ������.
* @param outSizeArray ������ ������� ����, � ������� ��� ��������� �����. 
* ����� �������������� ������������� ��� ����������� ����� �������.
* @retval ����� ������ ������.
*/
uint8_t* BinaryWriter_ToArray(BinaryWriterStruct* writer, int* outSizeArray);

/**
* @brief ����������� ��� �������, ������������ ���������� BinaryWriterStruct.
* @param writer ������������ BinaryWriterStruct.
* @retval None.
*/
void BinaryWriter_Destroy(BinaryWriterStruct* writer);

/**
* @brief ���������� ������������ Boolean �������� � ������� �����, �������������� 0 false
* � 1, �������������� true.
* @param writer ������, �������� ����� ��� ������.
* @param value Boolean �������� ��� ������ (0 ��� 1).
* @retval None.
*/
void BinaryWriter_WriteBoolean(BinaryWriterStruct* writer, bool value);

/**
* @brief ���������� ���� ��� ����� � ������� ����� � ���������� ������� � ������ �� ����
* ����.
* @param value Boolean �������� ��� ������ (0 ��� 1).
* @retval None.
*/
void BinaryWriter_WriteByte(BinaryWriterStruct* writer, uint8_t value);

/**
* @brief ���������� ������� ������� ������ � ������� �����.
* ������� ������� �� ��� ���������� ������.
* @param writer ������, �������� ����� ��� ������.
* @param buffer ������ ������, ���������� ������ ��� ������.
* @param count ���������� ������������ ������.
* @retval None.
*/
void BinaryWriter_WriteBytes(BinaryWriterStruct* writer, uint8_t* buffer, int count);

/**
* @brief  ���������� ���� � ������� ����� � ���������� ������� ������� � ������.
* @param writer ������, �������� ����� ��� ������.
* @param value ������ ������� ��� ������.
* @retval None.
*/
void BinaryWriter_WriteChar(BinaryWriterStruct* writer, char value);

/**
* @brief  ���������� ������. ������ ������������ ��������� ����� ������,
* ������� ������������ ��� ������� ����� ����������� �����.
* @param writer ������, �������� ����� ��� ������.
* @param value �������� ��� ������.
* @param lenght ����� ������.
* @retval None.
*/
void BinaryWriter_WriteString(BinaryWriterStruct* writer, char* value, uint8_t lenght);

/**
* @brief ���������� ��������������� ����� ����� ��� ����� � ������� ����� � ����������
* � ������ �� ��� �����.
* @param writer ������, �������� ����� ��� ������.
* @param value ������������ ����� ����� �� ������ ��� ������.
* @retval None.
*/
void BinaryWriter_WriteUInt16(BinaryWriterStruct* writer, uint16_t value);

/**
* @brief ��������� ����� ����� ��� ����� ������ 4 ����� � ������� � ������ �������� ������,
* ������� � ������ �� ������ ����� ������.
* @param writer ������, �������� ����� ��� ������.
* @param value ��������������� ����� ����� ��� ����� ��� ������.
* @retval None.
*/
void BinaryWriter_WriteUInt32(BinaryWriterStruct* writer, uint32_t value);

/**
* @brief ���������� ����������� ����� ����� �� ������ � ������� ����� � ���������� �������
* � ������ �� ��� �����.
* @param writer ������, �������� ����� ��� ������.
* @param value ������������ ����� ����� �� ������ ��� ������.
* @retval None.
*/
void BinaryWriter_WriteInt16(BinaryWriterStruct* writer, int16_t value);

/**
* @brief ���������� ��������������� ����� ����� �� ������ � ������� ����� � ����������
* ������� � ������ �� ������ ����� ������.
* @param writer ������, �������� ����� ��� ������.
* @param value ��������������� ����� ����� �� ������ ��� ������.
* @retval None.
*/
void BinaryWriter_WriteInt32(BinaryWriterStruct* writer, int32_t value);

/**
* @brief ���������� �������� � ��������� ������� 4 ����� � ������� ����� � ����������
* ������� � ������ �� ������ ����� ������.
* @param writer ������, �������� ����� ��� ������.
* @param value ������������ �������� � ��������� ������� 4 �����.
* @retval None.
*/
void BinaryWriter_WriteSingle(BinaryWriterStruct* writer, float value);

/**
* @brief ���������� ���������. ������� ������ ���������� ��� ����� ���������� ����� ��������� � �����.
* ������������ ���������� ����� ��������� ������������.
* ���� ������� ������ ������ �� ������������� ��������� ����������� ����������,
* �� ����������� ������������ ��������� ������ ������ ����������� �����������.
* ����� ��������� deep copy ������� ������ � �������� ������ ����� � ���������.
* ������ ����� ��� ���� ���������.
* @param writer ������, �������� ����� ��� ������.
* @param minCapacity ����� ����������� ����������� ����������� ������.
*/
void BinaryWriter_EnsureCapacityInternal(BinaryWriterStruct* writer, int minCapacity);

/**
* @brief ���������� ���������. ��������� ���������� ������ �� ��������� �����.
* @param writer ������, �������� ����� ��� ������.
* @param minCapacity ����� ����������� ����������� ����������� ������.
*/
void BinaryWriter_GrowBuffer(BinaryWriterStruct* writer, int minCapacity);


#endif