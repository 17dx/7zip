#ifndef ARHIVE7ZIP_H
#define ARHIVE7ZIP_H

#include <string>
#include "GenPassword.h"
using std::string;
/*
*
*
*
класс для работы с архиватором 7 zip
*
*
*
*/

class CArhive7zip {
public:
  
  CArhive7zip( string& pArhiveName,string& path7zip_);
  bool Unzip(string& password);
  bool FindPassword(CGenPassword& genPassword);
private:
  string path7zip;
  string arhiveName;  
  string textout ; 
  string sFind  ;
};

#endif // ARHIVE7ZIP_H