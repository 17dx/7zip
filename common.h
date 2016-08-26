#ifndef COMMON_H
#define COMMON_H

#include "noerrors.h"
#include <string>
#include <sstream> //stringstream

#define  EXIT_IF_ERROR() \
  if (eventError.LastError() != ERROR_NONE) return ;
  
using std::string;

typedef  unsigned long long TCount;

class CEventError{
public:
  static int codeError;
  CEventError();
  int LastError(); 
  void CreateEventError(string msgErr,int codeError); 
};

int  StringToInt(string  s);


template <typename T>
string to_string(const T v){
   std::stringstream ss;
   ss << v ;
   return ss.str();
};
 
 #endif // COMMON_H
