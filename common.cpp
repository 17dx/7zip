#include "common.h"
#include <cstdlib> // для exit
#include <iostream> //для  cout, cin
#include <sstream> //stringstream

int CEventError::codeError;

CEventError::CEventError(){
  codeError=ERROR_NONE;
};

void CEventError::CreateEventError(string msgErr,int codeError_){
  codeError=codeError_;
//#ifdef TEST  
  std::cout<< msgErr<<  std::endl;
  std::cin.get(); 
  exit(0);
//#endif // TEST  
}

int CEventError::LastError(){
  return codeError;
}; 

int StringToInt(string  s){
  int length=s.length();

  for (int i=0; i<length; i++){
    if (not isdigit(s[i]))
    {
       CEventError eventError;
       eventError.CreateEventError("no digital value",ERROR_OPTION_NO_DIGITAL_ARGUMENT);
       return 0;
    }
  }
  return atoi(s.c_str());
}

string to_string(size_t i){
   std::stringstream ss;
   ss << i ;
   return ss.str();
} 

string to_string(char c){
   std::stringstream ss;
   ss << c ;
   return ss.str();
} 

