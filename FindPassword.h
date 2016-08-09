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
const int  ERROR_ARHIVE_NOT_OPEN=2; 

typedef int (__stdcall *FUnzip)( const char * );
typedef void (__stdcall *FOpenArhive)( const char * );

class CFindPassword {
public:  
  CFindPassword( bool verbose_);
  virtual bool PasswordIsTrue(string& password)=0;
  bool DoFind(CGenPassword& genPassword);
  virtual ~CFindPassword();
protected:
  bool verbose;
};

class CArhiveConsole7z:public CFindPassword {
public:
  
  CArhiveConsole7z( string& pArhiveName,string& path7zip_,bool verbose_);
  virtual bool PasswordIsTrue(string& password);
protected:
  string path7zip;
  string arhiveName;
  string textout ; 
  string sFind  ;  
};

class CArhiveWith_Dll7z :public CFindPassword{
public:
  
  CArhiveWith_Dll7z( string& pArhiveName,bool verbose_);
  bool PasswordIsTrue(string& password);
  ~CArhiveWith_Dll7z();
private:
  string arhiveName;  
  FUnzip fUnzip;
  HINSTANCE hDllInstance ;
};

class CUserLogon :public CFindPassword{
public:  
  CUserLogon( string& pUserName,bool verbose_);
  bool PasswordIsTrue(string& password);
private:
  const char * userName;  
};

#endif // ARHIVE7ZIP_H