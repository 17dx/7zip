#ifndef RANGECHAR_H
#define RANGECHAR_H

#include <string>
#include <vector>
#include "noerrors.h"

using std::string;
using std::vector;

class CRangeChar{
public:
  string msgErr;
  
  //CRangeChar() ; 
   //CRangeChar(vector<char> * charRange_) ; 
   CRangeChar(const CRangeChar & obj);
   CRangeChar(string charRange_) ; 
  bool Inc() ;
  void ValueToFloor() ;
  char GetValue() ; 
  size_t GetRangeSize() ;
  int LastError();  
  //vector<char> * GetPCharRange();
protected:
  unsigned char value;  
  unsigned char minValue; // todo всегда ноль
  unsigned char maxValue;  //todo хранить по size charRange 
  vector<char> * charRange; 
  int codeError;
  void ParseRangeChar(string& range);
  void AppendRange(char firstChar,char lastChar);
  
};

#endif // RANGECHAR_H