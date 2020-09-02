#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gesture_model.h"
#include "binary_reader.h"
#include "binary_writer.h"

GestureModel* GestureModel_Init()
{
  GestureModel* gesture = malloc(sizeof(GestureModel));
  memset(gesture, 0, sizeof(GestureModel));
  gesture->ListMotions = malloc(sizeof(list));
  list_new(gesture->ListMotions, sizeof(MotionModel), MotionModel_Destroy);
  return gesture;
}

GestureModel* GestureModel_InitFromBinary(uint8_t* binaryData)
{
  GestureModel* gesture = GestureModel_Init();
  
  return gesture;
}

bool GestureModel_IdEquals(GestureModel* gesture, uint8_t* other_id)
{
  for (int i = 0; i < SIZE_OF_ID; i++)
  {
    if (gesture->Id[i] != other_id[i])
    {
      return false;
    }
  }
  
  return true;
}

bool GestureModel_NameEquals(GestureModel* gesture, char* other_name)
{
  int resultCmp = strcmp(gesture->Name, other_name);
  
  if (resultCmp == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

uint8_t* GestureModel_SerializeGesture(GestureModel* gesture, int* sizeOfArray)
{
  BinaryWriterStruct* binaryWriter = BinaryWriter_Init();
  
  BinaryWriter_WriteBytes(binaryWriter, gesture->Id, SIZE_OF_ID);
  BinaryWriter_WriteString(binaryWriter, gesture->Name, strlen(gesture->Name));
  BinaryWriter_WriteUInt32(binaryWriter, gesture->InfoGesture.TimeChange);
  BinaryWriter_WriteBoolean(binaryWriter, gesture->InfoGesture.IterableGesture);
  BinaryWriter_WriteByte(binaryWriter, gesture->InfoGesture.NumberOfGestureRepetitions);
  BinaryWriter_WriteByte(binaryWriter, gesture->InfoGesture.NumberOfMotions);
  
  for (int i = 0; i < gesture->InfoGesture.NumberOfMotions; i++)
  {
    MotionModel* motion = ((listNode*)list_get(gesture->ListMotions, 0))->data;
    
    BinaryWriter_WriteByte(binaryWriter, motion->PointerFinger);
    BinaryWriter_WriteByte(binaryWriter, motion->MiddleFinger);
    BinaryWriter_WriteByte(binaryWriter, motion->RingFinder);
    BinaryWriter_WriteByte(binaryWriter, motion->LittleFinger);
    BinaryWriter_WriteByte(binaryWriter, motion->ThumbFinger);
    BinaryWriter_WriteUInt16(binaryWriter, motion->StatePosBrush);
    BinaryWriter_WriteUInt16(binaryWriter, motion->DelMotion);
  }
  
  int sizeArray;
  uint8_t* array = BinaryWriter_ToArray(binaryWriter, &sizeArray);
  *sizeOfArray = sizeArray;
  return array;
}

GestureModel* GestureModel_DeserializeGesture(uint8_t* binaryData, int sizeOfArray)
{
  GestureModel* gesture = GestureModel_Init();
  
  BinaryReaderStruct* reader = BinaryReader_Init(binaryData);
  
  for (int i = 0; i < SIZE_OF_ID; i++) 
  {
     gesture->Id[i] = BinaryReader_ReadByte(reader);
  }
  
  gesture->Name = BinaryReader_ReadString(reader);
  
  gesture->InfoGesture.TimeChange = BinaryReader_ReadUInt32(reader);
  gesture->InfoGesture.IterableGesture = BinaryReader_ReadBoolean(reader);
  gesture->InfoGesture.NumberOfGestureRepetitions = BinaryReader_ReadByte(reader);
  gesture->InfoGesture.NumberOfMotions = BinaryReader_ReadByte(reader);
  
  for (int i = 0; i < gesture->InfoGesture.NumberOfMotions; i++)
  {
    MotionModel* motion = MotionModel_Init();
    motion->PointerFinger = BinaryReader_ReadByte(reader);
    motion->MiddleFinger = BinaryReader_ReadByte(reader);
    motion->RingFinder = BinaryReader_ReadByte(reader);
    motion->LittleFinger = BinaryReader_ReadByte(reader);
    motion->ThumbFinger = BinaryReader_ReadByte(reader);
    motion->StatePosBrush = BinaryReader_ReadUInt16(reader);
    motion->DelMotion = BinaryReader_ReadUInt16(reader);
    
    list_append(gesture->ListMotions, motion);
  }
  
  return gesture;
}

void GestureModel_Destroy(void* gesture)
{
  list_destroy(((GestureModel*)gesture)->ListMotions);
  free(gesture);
}

MotionModel* MotionModel_Init()
{
  MotionModel* motion = malloc(sizeof(MotionModel));
  memset(motion, 0, sizeof(MotionModel));
  return motion;
}

void MotionModel_Destroy(void *motion)
{
  free((MotionModel*)motion);
}

void TestSerializeGesture()
{
  uint8_t data[37];
  data[0] = 0xc0;
  data[1] = 0x45;
  data[2] = 0x8e;
  data[3] = 0x05;
  data[4] = 0x10;
  data[5] = 0xa4;
  data[6] = 0x08;
  data[7] = 0x49;
  data[8] = 0x9c;
  data[9] = 0xd7;
  data[10] = 0xe5;
  data[11] = 0xec;
  data[12] = 0x40;
  data[13] = 0xcf;
  data[14] = 0x87;
  data[15] = 0x34;
  data[16] = 0x04;
  data[17] = 0x4e;
  data[18] = 0x61;
  data[19] = 0x6d;
  data[20] = 0x65;
  data[21] = 0xe7;
  data[22] = 0x4e;
  data[23] = 0x6e;
  data[24] = 0x5d;
  data[25] = 0x01;
  data[26] = 0x03;
  data[27] = 0x01;
  data[28] = 0x03;
  data[29] = 0x02;
  data[30] = 0x04;
  data[31] = 0x01;
  data[32] = 0x06;
  data[33] = 0x05;
  data[34] = 0x00;
  data[35] = 0x0f;
  data[36] = 0x00;
  
  GestureModel* gesture = GestureModel_DeserializeGesture(data, 37);
}

void TestDeserializeGesture()
{
  GestureModel* gesture = GestureModel_Init();
  gesture->Id[0] = 0xc0;
  gesture->Id[1] = 0x45;
  gesture->Id[2] = 0x8e;
  gesture->Id[3] = 0x05;
  gesture->Id[4] = 0x10;
  gesture->Id[5] = 0xa4;
  gesture->Id[6] = 0x08;
  gesture->Id[7] = 0x49;
  gesture->Id[8] = 0x9c;
  gesture->Id[9] = 0xd7;
  gesture->Id[10] = 0xe5;
  gesture->Id[11] = 0xec;
  gesture->Id[12] = 0x40;
  gesture->Id[13] = 0xcf;
  gesture->Id[14] = 0x87;
  gesture->Id[15] = 0x34;
  
  gesture->Name = "Name";
  gesture->InfoGesture.TimeChange = 1567515228;
  gesture->InfoGesture.IterableGesture = true;
  gesture->InfoGesture.NumberOfGestureRepetitions = 3;
  gesture->InfoGesture.NumberOfMotions = 1;
  
  MotionModel* motion = MotionModel_Init();
  motion->PointerFinger = 3;
  motion->MiddleFinger = 2;
  motion->RingFinder = 4;
  motion->LittleFinger = 1;
  motion->ThumbFinger = 6;
  motion->StatePosBrush = 5;
  motion->DelMotion = 15;
  
  list_append(gesture->ListMotions, motion);
  
  int* sizeOfArray = malloc(sizeof(int));
  uint8_t* data = GestureModel_SerializeGesture(gesture, sizeOfArray);
  GestureModel* gestureNew = GestureModel_DeserializeGesture(data, *sizeOfArray);
}