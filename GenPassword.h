#ifndef GENPASSW_H
#define GENPASSW_H

#include <string>
#include <vector>

using std::string;
using std::vector;

const int  THROW_RANGE_CHAR_NOT_VALID    =-1;
const int  THROW_RANGE_LENGTH_NOT_VALID  =-2;
const int  THROW_OPTION_NO_DIGITAL_ARGUMENT=-3;
const int  THROW_LITTLE_LENGTH_PASSWORD =-4;

class CGenPassword{
public:
  string password;
  CGenPassword();
  CGenPassword(string& lenRange, string& range);
  ~CGenPassword();
  void Init(int len, string& range);
  bool Next();
  virtual void CreatePassword();
  long CountPasswords();
  string  msgErr;
  long numbPassword;
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

#endif // GENPASSW_H