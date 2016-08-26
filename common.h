#ifndef COMMON_H
#define COMMON_H

#include "noerrors.h"
#include <string>

#define  EXIT_IF_ERROR() \
  if (eventError.LastError() != ERROR_NONE) return ;
  
using std::string;



class CEventError{
public:
  static int codeError;
  CEventError();
  int LastError(); 
  void CreateEventError(string msgErr,int codeError); 
};

int  StringToInt(string  s);
string to_string(size_t i) ;
string to_string(char c);
 
 #endif // COMMON_H
