#ifndef __GESTURE_MODEL_H
#define __GESTURE_MODEL_H

#include "stdbool.h"
#include "stdint.h"
#include "list.h"
#include <stdio.h>

/* Информация о жесте. */
typedef struct {
  
  /* Время изменения жеста. */
  long TimeChange;
  
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
  uint8_t Id[16];
  
  /* Имя жеста */
  char* Name;
  
  /* Информация о жесте. */
  InfoGestureModel InfoGesture;
  
  /* Коллекция действий жеста. */
  list* ListMotions;
  
} GestureModel;

GestureModel* GestureModel_Init();

GestureModel* GestureModel_InitFromBinary(uint8_t* binaryData);

MotionModel* MotionModel_Init();

void MotionModel_Destroy(void *motion);

uint8_t* GestureModel_SerializeGesture(GestureModel* gesture);

GestureModel* GestureModel_DeserializeGesture(uint8_t* binaryData);

void GestureModel_Destroy(GestureModel* gesture);

 
#endif