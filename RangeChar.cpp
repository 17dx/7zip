#include "RangeChar.h"
#include <sstream> // для stringstream
#include <algorithm> // для find


 CRangeChar::CRangeChar(const CRangeChar & obj) {
  charRange=obj.charRange;
  minValue=obj.minValue; 
  maxValue=obj.maxValue;
  charRangeAsString=obj.charRangeAsString;
  value=minValue;  
}; 

 CRangeChar::CRangeChar(string charRange_) {
    charRangeAsString=charRange_;
    ParseRangeChar(charRange_);
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
     eventError.CreateEventError("range char not valid",ERROR_RANGE_CHAR_NOT_VALID);
     return ;
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
     string msg_error= "value for setting( ";
     msg_error+=to_string(nv )+" ) out of range ("+charRangeAsString+")";
     eventError.CreateEventError(msg_error,ERROR_SETTING_VALUE) ;
     return ;
    }
    
 }
 
size_t CRangeChar::GetRangeSize(){
   return charRange->size();
 }
 