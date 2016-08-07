#ifndef ARHIVE7ZIP_H
#define ARHIVE7ZIP_H

#include <string>
#include <exception> //для  exception
#include "windows.h"
#include "GenPassword.h"

using std::string;

class ex_error_load_7zdll : public std::exception{};
class ex_error_load_Unzip_from_7zdll : public std::exception{};
class ex_error_load_OpenArhive_from_7zdll : public std::exception{};
class ex_logical_error_ArhiveNotOpen_in_7zdll : public std::exception{};

/*
*
*
*
класс для работы с архиватором 7 zip
*
*
*
*/
const int  SUCCESS_UNZIP=1;
const int  NOT_SUCCESS_UNZIP=0;
const int  ERROR_ARHIVE_NOT_OPEN=-1; 

typedef int (__stdcall *FUnzip)( const char * );
typedef void (__stdcall *FOpenArhive)( const char * );

class CArhive7zip {
public:
  
  CArhive7zip( string& pArhiveName,string& path7zip_);
  virtual bool Unzip(string& password);
  bool FindPassword(CGenPassword& genPassword);
private:
  string path7zip;
  string arhiveName;  
  string textout ; 
  string sFind  ;
};

class CArhiveWrapper7zip :public CArhive7zip{
public:
  
  CArhiveWrapper7zip( string& pArhiveName,string& path7zip_);
  bool Unzip(string& password);
  ~CArhiveWrapper7zip();
private:
  FUnzip fUnzip;
  HINSTANCE hDllInstance ;
};

#endif // ARHIVE7ZIP_H