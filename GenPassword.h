#ifndef GENPASSW_H
#define GENPASSW_H

#include <string>
#include <vector>
#include <fstream> //istream

const int TYPE_TRANSLIT_NONE=1;
const int TYPE_TRANSLIT_ONLY=2;
const int TYPE_TRANSLIT_BOTH=3;
const bool DO_TRANSLIT=true;

using std::string;
using std::vector;

const int  ERROR_NONE    = 0;
const int  ERROR_RANGE_CHAR_NOT_VALID    = 1;
const int  ERROR_RANGE_LENGTH_NOT_VALID  = 2;
const int  ERROR_OPTION_NO_DIGITAL_ARGUMENT= 3;
const int  ERROR_LITTLE_LENGTH_PASSWORD = 4;
const int  ERROR_OPTION_NOT_VALID_KEYWORD = 5;

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
  char minValue;
  char maxValue;  
  vector<char> charRange;
  int lenPassword;
  char* arr; //динамический массив из  чисел
  bool Inc(int index);
  void ParseRangeChar(string& range);
  void AppendRange(char firstChar,char lastChar);
  void ReInit(int len);
  int  StringToInt(string  s);
  void ParseRangeLength(string& range);
  void Init(int len, string& range);
  virtual void CreatePassword();
private:
  int minLen;
  int maxLen;
  int stepOnLen;  
};


class CGenPasswordOnMask:public CGenPassword{
public:
  CGenPasswordOnMask(string mask,string& range);
  virtual void CreatePassword();
protected:
  vector<std::string::size_type> posStar;

};

class CGenPasswordFromDict: public CAbstractGenPassword{
 public:
  string dicPath;
  CGenPasswordFromDict(string dicPath_,string translitType_);
  virtual bool Next(); //overload
  virtual double CountPasswords(); //overload
  ~CGenPasswordFromDict();
private:  
  std::ifstream   fr;
  double countPasswords;
  bool queueToTranslit;
  int translitType;
  char tr[256];
  void  CalcCountPassword();
  void TranslitPassword();
  bool  ReadNextPassword();
  void trInit();
  
};




#endif // GENPASSW_H