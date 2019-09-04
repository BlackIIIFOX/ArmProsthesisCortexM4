#ifndef __GESTURE_MODEL_H
#define __GESTURE_MODEL_H

#include "stdbool.h"
#include "stdint.h"
#include "list.h"
#include <stdio.h>

/* Информация о жесте. */
#define SIZE_OF_ID 16

typedef struct {
  
  /* Время изменения жеста. */
  uint32_t TimeChange;
  
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
  uint8_t Id[SIZE_OF_ID];
  
  /* Имя жеста */
  char* Name;
  
  /* Информация о жесте. */
  InfoGestureModel InfoGesture;
  
  /* Коллекция действий жеста. */
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