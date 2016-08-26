#include "RangeChar.h"
#include <sstream> // для stringstream
#include <algorithm> // для find


/*CRangeChar::CRangeChar() {
  charRange=NULL;
  codeError=ERROR_NONE;
}; */

 CRangeChar::CRangeChar(const CRangeChar & obj) {
  codeError=ERROR_NONE;
  charRange=obj.charRange;
  minValue=obj.minValue; 
  maxValue=obj.maxValue;
  value=minValue;  
}; 

 CRangeChar::CRangeChar(string charRange_) {
    codeError=ERROR_NONE;
    ParseRangeChar(charRange_);
}; 

int CRangeChar::LastError(){
  return codeError;
};


void CRangeChar::AppendRange(char firstChar,char lastChar){  
    char step = (firstChar<lastChar) ? 1 : -1;
    char ch=firstChar-step;
    do {
       ch+=step;
       charRange->push_back(ch);       
    }  while(ch-lastChar !=0);
}

void CRangeChar::ParseRangeChar(string& range){
  std::stringstream ss(range);
  string item; 
  char firstChar;
  char lastChar;
  charRange=new  vector<char>; // todo удаление в деструкторе если создавали сами
  while (std::getline(ss, item, ',')){
    if(item.length()==3){
      if(item[1]=='-'){
          firstChar = item[0]; // с какого символа начать
          lastChar = item[2]; //каким символом закончить   
          AppendRange (firstChar,lastChar);         
      }
    } 
    else if(item.length()==1)  {
       charRange->push_back(item[0]); 
    }
    
  }
  
  if (charRange->size()==0){
     msgErr= "range char not valid";
     codeError=ERROR_RANGE_CHAR_NOT_VALID;
     return ;
     //throw THROW_RANGE_CHAR_NOT_VALID;
  }
  else{
    minValue=0;
    value=minValue;
    maxValue=charRange->size()-1;
  }
};

bool CRangeChar::Inc() {
 if (value==maxValue){
    return false;
 }
 value++;
 return true;
}

void CRangeChar::ValueToFloor() {
 value=minValue;
}

 char CRangeChar::GetValue() {
   return  charRange->at(value);
 
 };
 
 void CRangeChar::SetValue(char nv){
    vector<char>::iterator it=std::find(charRange->begin(), charRange->end(), nv);
    if (it != charRange->end())  { 
       value=it-charRange->begin();
    }
    else{
     msgErr= "value for setting out of range";
     codeError=ERROR_SETTING_VALUE;
     return ;
    }
    
 }
 
size_t CRangeChar::GetRangeSize(){
   return charRange->size();
 }
 