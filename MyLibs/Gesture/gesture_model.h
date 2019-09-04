#ifndef __GESTURE_MODEL_H
#define __GESTURE_MODEL_H

#include "stdbool.h"
#include "stdint.h"
#include "list.h"
#include <stdio.h>

/* ���������� � �����. */
#define SIZE_OF_ID 16

typedef struct {
  
  /* ����� ��������� �����. */
  uint32_t TimeChange;
  
  /* ��������� ������������� �����. */
  bool IterableGesture;
  
  /* ���������� ���������� �����. */
  uint8_t NumberOfGestureRepetitions;
  
  /* ���-�� �������� � �����. */
  uint8_t NumberOfMotions;
  
} InfoGestureModel;

/* ��������� �������� �����. �������� ����� ������� � �����, � ��� �� ��������, 
����������� ����� ��������� ���������. */
typedef struct {
  
  /* ��������� ������������� ������ � �������� (0-180). */
  uint8_t PointerFinger;
  
  /* ��������� �������� ������ � �������� (0-180). */
  uint8_t MiddleFinger;
  
  /* ��������� ����������� ������ � �������� (0-180). */
  uint8_t RingFinder;
  
  /* ��������� ������� � �������� (0-180). */
  uint8_t LittleFinger;
  
  /* ��������� �������� ������ � �������� (0-180). */
  uint8_t ThumbFinger;
  
  /* ��������� ����� � �������� (0-350). */
  uint16_t StatePosBrush;
  
  /* �������� ����� ���������� � ������������. */
  uint16_t DelMotion;
  
} MotionModel;

/* ����, ����������� �������� */
typedef struct {
  
  /* Id ����� � Guid. */
  uint8_t Id[SIZE_OF_ID];
  
  /* ��� ����� */
  char* Name;
  
  /* ���������� � �����. */
  InfoGestureModel InfoGesture;
  
  /* ��������� �������� �����. */
  list* ListMotions;
  
} GestureModel;

GestureModel* GestureModel_Init();

GestureModel* GestureModel_InitFromBinary(uint8_t* binaryData);

bool GestureModel_IdEquals(GestureModel* gesture, uint8_t* other_id);

bool GestureModel_NameEquals(GestureModel* gesture, char* other_name);

MotionModel* MotionModel_Init();

void MotionModel_Destroy(void *motion);

uint8_t* GestureModel_SerializeGesture(GestureModel* gesture, int* sizeOfArray);

GestureModel* GestureModel_DeserializeGesture(uint8_t* binaryData, int sizeOfArray);

void GestureModel_Destroy(void* gesture);

 
#endif