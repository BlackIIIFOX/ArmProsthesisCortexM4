#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gesture_model.h"
#include "binary_reader.h"

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

uint8_t* GestureModel_SerializeGesture(GestureModel* gesture)
{
  // NOT IMPLEMENT!!!
}

GestureModel* GestureModel_DeserializeGesture(uint8_t* binaryData)
{
  GestureModel* gesture = GestureModel_Init();
  
  BinaryReaderStruct* reader = BinaryReader_Init(binaryData);
  
  for (int i = 0; i < 16; i++) 
  {
     gesture->Id[i] = BinaryReader_ReadByte(reader);
  }
  
  gesture->Name = BinaryReader_ReadString(reader);
  
  gesture->InfoGesture.TimeChange = BinaryReader_ReadDouble(reader);
  gesture->InfoGesture.IterableGesture = BinaryReader_ReadBoolean(reader);
  gesture->InfoGesture.NumberOfGestureRepetitions = BinaryReader_ReadByte(reader);
  gesture->InfoGesture.NumberOfMotions = BinaryReader_ReadByte(reader);
  
  for (int i = 0; i < gesture->InfoGesture.NumberOfMotions; i++)
  {
    MotionModel* motion = MotionModel_Init();
    motion->PointerFinger = BinaryReader_ReadByte(reader);
    motion->MiddleFinger = BinaryReader_ReadByte(reader);
    motion->RingFinder = BinaryReader_ReadByte(reader);
    motion->ThumbFinger = BinaryReader_ReadByte(reader);
    motion->StatePosBrush = BinaryReader_ReadUInt16(reader);
    motion->DelMotion = BinaryReader_ReadUInt16(reader);
    
    list_append(gesture->ListMotions, motion);
  }
  
  return gesture;
}

void GestureModel_Destroy(GestureModel* gesture)
{
  list_destroy(gesture->ListMotions);
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