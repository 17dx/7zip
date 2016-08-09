#include "GenPassword.h"
#include <sstream> // для stringstream
#include <cmath> // для pow

//конструктор
CGenPassword::CGenPassword(){
}

CGenPassword::CGenPassword(int len, string& range){
  Init(len,range);
  CreatePassword(); //создать начальный пароль
}


//проинициализировать переменные
// ::эта функция пригодится наследникам
void CGenPassword::Init(int len, string& range){
  ParseRangeChar(range);
  lenPassword=len;
  numbPassword=0;
  arr = new char [lenPassword]; //выделяем память под массив
  for(int i=0; i < lenPassword; i++){
    arr[i]=minValue;
  }  
};
    
void CGenPassword::AppendRange(char firstChar,char lastChar){  
    char step = (firstChar<lastChar) ? 1 : -1;
    char ch=firstChar-step;
    do {
       ch+=step;
       charRange.push_back(ch);       
    }  while(ch-lastChar !=0);
}
    
void CGenPassword::ParseRangeChar(string& range){
  std::stringstream ss(range);
  string item; 
  char firstChar;
  char lastChar;
  while (std::getline(ss, item, ',')){
    if(item.length()==3){
      if(item[1]=='-'){
          firstChar = item[0]; // с какого символа начать
          lastChar = item[2]; //каким символом закончить   
          AppendRange (firstChar,lastChar);         
      }
    } 
    else if(item.length()==1)  {
       charRange.push_back(item[0]); 
    }

    
  }
  
  if (charRange.size()==0){
     msgErr= "range char not valid";
     throw THROW_RANGE_CHAR_NOT_VALID;
  }
  else{
    minValue=0;
    maxValue=charRange.size()-1;
  }
};

//деструктор
CGenPassword::~CGenPassword(){
  delete [] arr; //освобождаем память
}

//создать следующий пароль
bool CGenPassword::Next(){
  if ( not Inc(lenPassword-1) ){
    return false;
  }
  CreatePassword();  
  return true;  
}

 void CGenPassword::CreatePassword(){
   password="";
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password+=charRange[arr[i]];
   }
   //password.append(arr, lenPassword);
 }
 
 long CGenPassword::CountPasswords(){
   return pow(charRange.size(),lenPassword);
 }

//увеличить символ
bool CGenPassword::Inc(int index){

  if ( arr[index] == maxValue  ){
     // если максимальный номер символа в текущей ячейке
     // то в текущей ячейке начать с начала     
     arr[index] = minValue;
     
     if (index ==0){
       //более старшего разряда нет, больше увеличивать нельзя
       return false;
     } 
     else{
       //а в предыдущей ячейке(в старшем разряде) увеличить символ
       return Inc(index-1);
     }     
  }
  // если до сих пор функция не завершина,
  //  то можно просто увеличить символ
  arr[index]++;
  return true;

}

//////////////////////////////////////////////////////////////////
//////////////////////наследник класса GenPassword //////////////
//////////////////////////////////////////////////////////////////

 CGenPasswordOnMask::CGenPasswordOnMask(string mask,string& range){
   int lenMask=mask.length();
   int countStar=0;
   for(int i = 0; i < lenMask; i++){
        if(mask[i] == '*'){
          posStar.push_back(i);
          countStar++;
        }
   }   
   Init(countStar, range); // создаем и инициализируем массив
   password=mask; 
   CreatePassword();
 }
 
 void CGenPasswordOnMask::CreatePassword(){
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password[posStar[i]]=charRange[arr[i]];
   }
 }