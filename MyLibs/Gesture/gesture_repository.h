#ifndef __GESTURE_REPOSITORY_H
#define __GESTURE_REPOSITORY_H

#include "stdbool.h"
#include "stdint.h"
#include "list.h"
#include <stdio.h>
#include "gesture_model.h"

/* Репозиторий для доступа к жестам протеза. */
typedef struct {
  
  /* Коллекция жестов. */
  list* ListGestures;
  
} GestureRepositoryStruct;

GestureRepositoryStruct* GestureRepository_Init();

void GestureRepository_Load(GestureRepositoryStruct* repo);

bool GestureRepository_Add(GestureRepositoryStruct* repo, GestureModel* gestureForAdd);

bool GestureRepository_Remove(GestureRepositoryStruct* repo, GestureModel* gestureForRemove);

int GestureRepository_GetCount(GestureRepositoryStruct* repo);

GestureModel* GestureRepository_GetByIndex(GestureRepositoryStruct* repo, int index);

GestureModel* GestureRepository_GetById(GestureRepositoryStruct* repo, uint8_t* id);

GestureModel* GestureRepository_GetByName(GestureRepositoryStruct* repo, char* name);

void GestureRepository_Destroy(GestureRepositoryStruct* repo);

 
#endif