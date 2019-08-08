#include "data_handler.h"
#include <stddef.h>
//#include <cstdlib>
#include <stdlib.h>

//==============================================================================
//=================================Veriables====================================
//==============================================================================
StructListReceiveData infoList;
StructPackageBuffer* packageBufReceive = NULL;
// static uint8_t start_filed[8] = { '1', 'N', '7', 'R', 'O', 'I', 'N', 'm' };
// static uint8_t end_filed[8] = { 'R', '{', 'D', '9', '8', 'V', '8', '9' };

//==============================================================================
//================================Package receive===============================
//==============================================================================

void InitElemDataReceive(StructReceiveData* elem)
{
  elem->data = 0;
  elem->next_addr = NULL;
  elem->prev_addr = NULL;
}

void SetPackageBuf(StructPackageBuffer* newPackageBuf)
{
  packageBufReceive = newPackageBuf;
}

void Create_Data_Receive()
{
  StructReceiveData* new_list = malloc(sizeof(StructReceiveData));
  InitElemDataReceive(new_list);
  infoList.count_data = 0;
  infoList.crc8 = 0;
  infoList.list_receive_head = new_list;
  infoList.list_receive_last = new_list;
  infoList.count_main_field = 0;
  infoList.state_start_receive = 0;
}

void Append_Data_Receive(unsigned char data)
{
  infoList.count_data++;
  
  //  if(infoList.count_data == 124)
  //    {
  //        int a = 2;
  //    }
  
  if(infoList.count_data == 1)
    infoList.list_receive_last->data = data;
  else
  {   
    StructReceiveData* new_elem = malloc(sizeof(StructReceiveData));
    InitElemDataReceive(new_elem);
    new_elem->prev_addr = (uint32_t*)infoList.list_receive_last;
    infoList.list_receive_last->next_addr = (uint32_t*)new_elem;
    infoList.list_receive_last = new_elem;
    infoList.list_receive_last->data = data;
  }
  
  if (infoList.state_start_receive == 0)
  {
    if(infoList.count_data == 8)
    {
      if(CheackStartField() == 1)
        infoList.state_start_receive = 1;
      else
      {
        unsigned int* nextAddr = infoList.list_receive_head->next_addr;
        free(infoList.list_receive_head);
        infoList.list_receive_head = (StructReceiveData*)nextAddr;
        infoList.count_data--;
      }
    }
  }
  else
  {
    // Check version protocol
    if(infoList.count_data == 10)
    {
      if(CheackVersionField()==0)
        Clear_Data_Receive();
    }
    
    if(infoList.count_data == 12)
    {
      if(infoList.list_receive_last->data != addressHand)
        Clear_Data_Receive();
    }
    
    if(infoList.count_data == 16)
    {
      infoList.count_main_field = ReadCountMainField();           
    }
    
    if(infoList.count_data == infoList.count_main_field + 17)
    {
      infoList.crc8 = infoList.list_receive_last->data;
    }
    
    if(infoList.count_data == infoList.count_main_field + 17 + 8)
    {
      if(CheackStopField() == 0)
      {
        Clear_Data_Receive();
      }      
      else
      {
        uint8_t* new_data = dataConvertToArray();
        uint8_t crc8 = CalculateCRC8(new_data, infoList.count_main_field);
        if (crc8!=infoList.crc8 && packageBufReceive != NULL)
          free(new_data);
        else
        {
          StructPackage* newPackage = malloc(sizeof(StructPackage));
          newPackage->countData = infoList.count_main_field;
          newPackage->package = new_data;
          PackageBufAdd(packageBufReceive, newPackage);
        }
        
        Clear_Data_Receive();
      }
    }
  }
}

uint8_t* dataConvertToArray()
{
  uint8_t* array = malloc(sizeof(uint8_t)*infoList.count_main_field);
  
  StructReceiveData* dataRx = infoList.list_receive_head;
  for(int i = 0; i < 16; i++)
  {
    dataRx = (StructReceiveData*)dataRx->next_addr;
  }
  
  for(int i = 0; i < infoList.count_main_field; i++)
  {
    array[i] = dataRx->data;
    dataRx = (StructReceiveData*)dataRx->next_addr;
  }
  return array;
}

uint32_t ReadCountMainField()
{
  uint32_t countDataField;
  
  StructReceiveData* dataRx = infoList.list_receive_last;
  countDataField = dataRx->data;
  
  dataRx = (StructReceiveData*)dataRx->prev_addr;
  countDataField |= (dataRx->data)<<8;
  
  dataRx = (StructReceiveData*)dataRx->prev_addr;
  countDataField |= (dataRx->data)<<16;
  
  dataRx = (StructReceiveData*)dataRx->prev_addr;
  countDataField |= (dataRx->data)<<24;
  
  return countDataField;
}

void Clear_Data_Receive()
{
  StructReceiveData* Data_list = infoList.list_receive_head;
  while(1)
  {
    StructReceiveData* next_addr = (StructReceiveData*)Data_list->next_addr;
    if(next_addr == NULL)
    {
      free(Data_list);
      break;
    }
    free(Data_list);
    Data_list = next_addr;
  }
  Create_Data_Receive();
}

unsigned char CheackVersionField()
{
  unsigned char stateCheck = 0;
  if(infoList.list_receive_last->data == versionProtocol[2])
  {
    StructReceiveData* prev_elem = 
      (StructReceiveData*)(infoList.list_receive_last->prev_addr);
    if(prev_elem->data == versionProtocol[0])
      stateCheck = 1;
  }
  return stateCheck;
}

unsigned char CheackStartField()
{
  unsigned char stateCheack = 1;
  StructReceiveData* elem = infoList.list_receive_head;
  for(int i = 0; i < 8; i++)
  {
    if(startFiled[i]!=elem->data)
    {
      stateCheack = 0;
      break;
    }
    elem = (StructReceiveData*)elem->next_addr;
  }
  return stateCheack;
}

unsigned char CheackStopField()
{
  unsigned char stateCheack = 1;
  StructReceiveData* elem = infoList.list_receive_last;
  for(int i = 7; i >= 0; i--)
  {
    if(endFiled[i]!=elem->data)
    {
      stateCheack = 0;
      break;
    }
    elem = (StructReceiveData*)elem->prev_addr;
  }
  return stateCheack;
}

StructPackage* Create_Transmit_Package(uint8_t distAddress, uint8_t* data, int count_data)
{
  int lenght_package = count_data + 25;
  uint8_t* new_package = malloc(sizeof(uint8_t)*lenght_package);
  
  // Стартовая константа
  for(int i = 0; i < 8; i++)
  {
    new_package[i] = startFiled[i];
  }
  
  // Заполнения поля информации пакета
  new_package[8] = versionProtocol[0];
  new_package[9] = versionProtocol[2];
  
  new_package[10] = addressHand;
  new_package[11] = distAddress;
  
  // Заполнение поля размера информационного пакета
  new_package[15] = (uint8_t)(count_data & 0x000000FF);
  new_package[14] = (uint8_t)((count_data & 0x0000FF00) >> 8);
  new_package[13] = (uint8_t)((count_data & 0x00FF0000) >> 16);
  new_package[12] = (uint8_t)((count_data & 0xFF000000) >> 24);
  
  // Создание основного контейнера данных(без CRC,стартовых и стоповых констант)
  uint8_t crc8 = CalculateCRC8(data, count_data);
  
  for(int i = 0; i < count_data; i++)
    new_package[i + 16] = data[i];
  new_package[16 + count_data] = crc8;
  
  // Добавление в конечный пакет стоповой последовательности
  for(int i = 0; i < 8; i++)
    new_package[17 + count_data + i] = endFiled[i];
  
  StructPackage* newPackage = malloc(sizeof(StructPackage));
  newPackage->countData = lenght_package;
  newPackage->package = new_package; 
  
  return newPackage;
}


//==============================================================================
//=============================Buffer package===================================
//==============================================================================

void CreatePackageBuffer(StructPackageBuffer* packageBuffer)
{
  packageBuffer->IndexStart = 0;
  packageBuffer->IndexEnd = 0;
  packageBuffer->buffer = malloc(sizeof(uint32_t)*BUFFER_SIZE);
}


unsigned char PackageBufAdd(StructPackageBuffer* packageBuffer, StructPackage* package)
{
  packageBuffer->IndexEnd++;
  if(packageBuffer->IndexEnd == packageBuffer->IndexStart) return 1;
  packageBuffer->IndexEnd &= BUFFER_MASK; 
  packageBuffer->buffer[packageBuffer->IndexEnd] = (uint32_t)package; 
  return 0;
}


StructPackage* PackageBufGet(StructPackageBuffer* packageBuffer)
{
  packageBuffer->IndexStart++;
  packageBuffer->IndexStart &= BUFFER_MASK;
  return (StructPackage*)packageBuffer->buffer[packageBuffer->IndexStart];
}


unsigned char PackageBufferCount(StructPackageBuffer* packageBuffer)
{
  if(packageBuffer->IndexEnd != packageBuffer->IndexStart) return 1;
  return 0;
}

unsigned char PackageBufferState(StructPackageBuffer* packageBuffer)
{
  if (packageBuffer->IndexEnd >= packageBuffer->IndexStart)
  {
    return (packageBuffer->IndexEnd - packageBuffer->IndexStart);
  }	
  else
  {
    return ((BUFFER_SIZE - packageBuffer->IndexStart) + packageBuffer->IndexEnd);
  }
}

//==============================================================================
//==============================================================================
//==============================================================================







unsigned char CRC8_TABLE[] = {
  0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
  157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
  35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
  190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
  70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
  219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
  101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
  248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
  140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
  17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
  175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
  50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
  202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
  87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
  233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
  116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

unsigned char CalculateCRC8(unsigned char *data, unsigned int Length)
{
  unsigned char result = 0;
  for (int i = 0; i < Length; i++)
  {
    result = CRC8_TABLE[result ^ data[i]];
  }
  return result;
}



// "1N7ROINm"
//  Append_Data_Receive(0);
//  Append_Data_Receive('1');
//  Append_Data_Receive('N');
//  Append_Data_Receive('7');
//  Append_Data_Receive('R');
//  Append_Data_Receive('O');
//  Append_Data_Receive('I');
//  Append_Data_Receive('N');
//  Append_Data_Receive('m');
//  Append_Data_Receive('0');
//  Append_Data_Receive('1');
//  Append_Data_Receive(0);
//  Append_Data_Receive(4);
//  Append_Data_Receive(0);
//  Append_Data_Receive(0);
//  Append_Data_Receive(0);
//  Append_Data_Receive(6);
//  // StartField
//  Append_Data_Receive('1');
//  Append_Data_Receive('N');
//  Append_Data_Receive('7');
//  Append_Data_Receive('R');
//  Append_Data_Receive('O');
//  Append_Data_Receive('I');
//  Append_Data_Receive('N');
//  Append_Data_Receive('m');
//  // Version protocol
//  Append_Data_Receive('0');
//  Append_Data_Receive('1');
//  // Addr Pc
//  Append_Data_Receive(0);
//  // Addr Hand
//  Append_Data_Receive(1);
//  // Count data
//  Append_Data_Receive(0);
//  Append_Data_Receive(0);
//  Append_Data_Receive(0);
//  Append_Data_Receive(2);
//  // Data
//  Append_Data_Receive(2);
//  Append_Data_Receive(3);
//  // CRC
//  Append_Data_Receive(0x73);
//  // StopField "R{D98V89"
//  Append_Data_Receive('R');
//  Append_Data_Receive('{');
//  Append_Data_Receive('D');
//  Append_Data_Receive('9');
//  Append_Data_Receive('8');
//  Append_Data_Receive('V');
//  Append_Data_Receive('8');
//  Append_Data_Receive('9');
