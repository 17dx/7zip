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
class ex_error_Word_not_found: public std::exception{};
class ex_error_Word_not_running: public std::exception{};
class ex_error_Word_not_get_propperty: public std::exception{};
class ex_error_Word_not_property_Quit: public std::exception{};
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

typedef int (__stdcall *FUnzip)( const char * , const char *);
typedef void (__stdcall *FOpenArhive)( const char * );

struct SInvokeOpenInfo{
  DISPID dispid[3];
  DISPPARAMS dp;
  EXCEPINFO excepInfo;
  UINT uArgErr; // индекс аргумента структуры DISPPARAMS,вызвавшего ошибку
};

class CFindPassword {
public:
  string truePassword;
  bool workFinished;
  CFindPassword( bool verbose_);
  virtual bool PasswordIsTrue(SPasswordInfo& passwordInfo)=0;
  bool DoFind(CAbstractGenPassword& genPassword);
  virtual ~CFindPassword();
  bool FindOK();
  CRITICAL_SECTION printStdOut;
protected:
  bool verbose;
  bool findOK;

};

class CArhiveConsole7z:public CFindPassword {
public:

  CArhiveConsole7z( string& pArhiveName,string& path7zip_,bool verbose_);
  virtual bool PasswordIsTrue(SPasswordInfo& passwordInfo);
protected:
  string path7zip;
  string arhiveName;
  string textout ;
  string sFind  ;
};

class CArhiveWith_Dll7z :public CFindPassword{
public:

  CArhiveWith_Dll7z( string& pArhiveName,bool verbose_);
  bool PasswordIsTrue(SPasswordInfo& passwordInfo);
  ~CArhiveWith_Dll7z();
private:
  string arhiveName;
  FUnzip fUnzip;
  HINSTANCE hDllInstance ;
};

class CUserLogon :public CFindPassword{
public:
  CUserLogon( string& pUserName,bool verbose_);
  bool PasswordIsTrue(SPasswordInfo& passwordInfo);
private:
  const char * userName;
};

class CMSWord :public CFindPassword{
public:
  CMSWord( string& pDocName,bool verbose_);
  bool PasswordIsTrue(SPasswordInfo& passwordInfo);
  ~CMSWord();
protected:
  bool IsCreateWord;
  const char * docName;
  IDispatch * pWordApp;
  IDispatch * pDocuments ;
  SInvokeOpenInfo invokeOpenInfo ;
  IDispatch * PropertyGet(IDispatch * dispObj,LPOLESTR nameProperty);
  //IDispatch * GetObject(IDispatch * Obj, std::wstring  pathToObj);
  void Word_Quit();
  void InitObjWord_ForInvokeOpen() ;
  void ConnectToWord();
};

#endif // ARHIVE7ZIP_H
