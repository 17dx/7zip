#ifndef GENPASSW_H
#define GENPASSW_H

#include <string>
#include <vector>
#include <fstream> //istream

#include <cmath> // для pow
#include <cstdlib> //Для atoi

#include "RangeChar.h"
#include "noerrors.h"

const bool DO_TRANSLIT=true;

using std::string;
using std::vector;


class CAbstractGenPassword{
public:
//пустой пароль приводит к ошибке в dll
  string password;
  string msgErr;

  double numbPassword;
  virtual bool Next()=0;
  virtual double CountPasswords()=0; 
  int LastError();
  CAbstractGenPassword();
  virtual ~CAbstractGenPassword(){};
protected:  
    int codeError;
};

class CGenPassword:public CAbstractGenPassword{
public:
  CGenPassword();
  CGenPassword(string& lenRange, string& range);
  ~CGenPassword();  
  virtual bool Next(); //overload
  virtual double CountPasswords(); //overload
protected:

  int lenPassword;
  CRangeChar ** chars;
  bool Inc(int index);
  void ReInit(int len);
  int  StringToInt(string  s);
  void ParseRangeLength(string& range);
  void Init(int len,string& range);//, string& range
  void Init(int len);//,CRangeChar** ranges, string& range
  virtual void CreatePassword();
private:
  int startLen;
  int endLen;
  int maxLen;
  int stepOnLen;  
};


class CGenPasswordOnMask:public CGenPassword{
public:
  CGenPasswordOnMask(string& mask,string& range);
  CGenPasswordOnMask(string& ext_mask);
  virtual void CreatePassword();

protected:
  vector<std::string::size_type> posPointInsert;

};


class CGenPasswordFromDict: public CAbstractGenPassword{
 public:
  string dicPath;
  CGenPasswordFromDict(string dicPath_,int translitType_);
  virtual bool Next(); //overload
  virtual double CountPasswords(); //overload
  ~CGenPasswordFromDict();
private:  
  std::ifstream   fr;
  double countPasswords;
  bool queueToTranslit;
  int translitType;
  unsigned char tr[256];
  void  CalcCountPassword();
  void TranslitPassword();
  bool  ReadNextPassword();
  void trInit();
  
};

#endif // GENPASSW_H