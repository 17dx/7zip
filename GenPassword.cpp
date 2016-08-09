#include "GenPassword.h"
#include <sstream> // для stringstream
#include <cmath> // для pow
#include <cstdlib> //Для atoi

//конструктор
CGenPassword::CGenPassword(){
  minLen=0;
  maxLen=0;
  stepOnLen=0; 
}

CGenPassword::CGenPassword(string& lenRange, string& range){
  ParseRangeLength (lenRange); 
  Init(minLen,range);
  numbPassword=0;
  CreatePassword(); //создать начальный пароль  
}


void CGenPassword::ReInit(int len){ 
  lenPassword=len;  
  delete[] arr ;
  arr = new char [lenPassword]; //выделяем память под массив
  for(int i=0; i < lenPassword; i++){
    arr[i]=minValue;
  }  
};

//проинициализировать переменные
// ::эта функция пригодится наследникам
void CGenPassword::Init(int len, string& range){ 
  ParseRangeChar(range);
  lenPassword=len; 
  if (len==0) {
       msgErr="little length password";
       throw THROW_LITTLE_LENGTH_PASSWORD;
  }
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


int CGenPassword::StringToInt(string  s){
  int length=s.length();

  for (int i=0; i<length; i++){
    if (not isdigit(s[i]))
    {
       msgErr="no digital value by length password";
       throw THROW_OPTION_NO_DIGITAL_ARGUMENT ;
    }
  }
  return atoi(s.c_str());
}

void CGenPassword::ParseRangeLength(string& range){
  minLen=0;
  maxLen=0;
  stepOnLen=0;
  std::size_t pos= range.find("-"); 
  if (pos == string::npos ){
    minLen= StringToInt(range);
    maxLen= minLen;
  }
  else{
    if (pos == range.size()){
       msgErr="no correct value range length password";
       throw THROW_RANGE_LENGTH_NOT_VALID ;
    }
    else{
        minLen= StringToInt(range.substr(0,pos));
        maxLen= StringToInt(range.substr(pos+1));
        stepOnLen=(maxLen-minLen > 0 ) ? 1: -1;        
    }     
  }  
  
  if (( minLen==0) && ( maxLen==0)) {
       msgErr="little length password";
       throw THROW_LITTLE_LENGTH_PASSWORD;
  }
  return ;
};
    
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
    if (maxLen !=0 && lenPassword != maxLen){      
      lenPassword+=stepOnLen;
      ReInit(lenPassword);
      return true;
    }
    else{
      return false;
    }  
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
 
 double CGenPassword::CountPasswords(){
   if (maxLen !=0){
       double count=0;
       int i = minLen-stepOnLen;
       do{
           i+=stepOnLen;
           count+=  pow(charRange.size(),i);       
       }while (i != maxLen);
       return  ceil(count);
   }
    return  ceil(pow(charRange.size(),lenPassword)); 
   
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
   numbPassword=0;
   CreatePassword();   
 }
 
 void CGenPasswordOnMask::CreatePassword(){
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password[posStar[i]]=charRange[arr[i]];
   }
 }