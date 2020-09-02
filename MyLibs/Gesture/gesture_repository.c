#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gesture_repository.h"

GestureRepositoryStruct* GestureRepository_Init()
{
  GestureRepositoryStruct* repo = malloc(sizeof(GestureRepositoryStruct));
  memset(repo, 0, sizeof(GestureRepositoryStruct));
  repo->ListGestures = malloc(sizeof(list));
  list_new(repo->ListGestures, sizeof(GestureModel), GestureModel_Destroy);
  return repo;
}

void GestureRepository_Destroy(GestureRepositoryStruct* repo)
{
  list_destroy(repo->ListGestures);
  free(repo);
}

void GestureRepository_Load(GestureRepositoryStruct* repo)
{
  //TODO: имплементация загрузки с флешки.
}

int GestureRepository_GetCount(GestureRepositoryStruct* repo)
{
  return list_size(repo->ListGestures);
}

GestureModel* GestureRepository_GetByIndex(GestureRepositoryStruct* repo, int index)
{
  if (list_size(repo->ListGestures) < index)
  {
    return NULL;
  }
  
  return list_get(repo->ListGestures, index)->data;
}

GestureModel* GestureRepository_GetById(GestureRepositoryStruct* repo, uint8_t* id)
{
  for (int i = 0; i < list_size(repo->ListGestures); i++)
  {
    GestureModel* gesture = list_get(repo->ListGestures, i)->data;
    if (GestureModel_IdEquals(gesture, id))
    {
      return gesture;
    }
  }
  
  return NULL;
}

GestureModel* GestureRepository_GetByName(GestureRepositoryStruct* repo, char* name)
{
  for (int i = 0; i < list_size(repo->ListGestures); i++)
  {
    GestureModel* gesture = list_get(repo->ListGestures, i)->data;
    if (GestureModel_NameEquals(gesture, name))
    {
      return gesture;
    }
  }
  
  return NULL;
}

bool GestureRepository_Add(GestureRepositoryStruct* repo, GestureModel* gestureForAdd)
{
  for (int i = 0; i < list_size(repo->ListGestures); i++)
  {
    GestureModel* gesture = list_get(repo->ListGestures, i)->data;
    if (GestureModel_IdEquals(gesture, gestureForAdd->Id))
    {
      return false;
    }
  }
  
  list_append(repo->ListGestures, gestureForAdd);
  
  // TODO: сохранение на флешку.
  return true;
}

bool GestureRepository_Remove(GestureRepositoryStruct* repo, GestureModel* gestureForRemove)
{
  for (int i = 0; i < list_size(repo->ListGestures); i++)
  {
    GestureModel* gesture = list_get(repo->ListGestures, i)->data;
    if (GestureModel_IdEquals(gesture, gestureForRemove->Id))
    {
      list_remove(repo->ListGestures, i);
      
      // TODO: удаление с флешки.
      return true;
    }
  }
  
  return false;
}