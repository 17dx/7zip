#ifndef GENPASSW_H
#define GENPASSW_H

#include <string>
#include <vector>

using std::string;
using std::vector;

const int  THROW_RANGE_CHAR_NOT_VALID    =-1;

class CGenPassword{
public:
  string password;
  CGenPassword();
  CGenPassword(int len, string& range);
  ~CGenPassword();
  void Init(int len, string& range);
  bool Next();
  virtual void CreatePassword();
  string  msgErr;
protected:
  char minValue;
  char maxValue;
  vector<char> charRange;
  int lenPassword;
  char* arr; //динамический массив из  чисел
  bool Inc(int index);
  void ParseRangeChar(string& range);
  void AppendRange(char firstChar,char lastChar);
};


class CGenPasswordOnMask:public CGenPassword{
public:
  CGenPasswordOnMask(string mask,string& range);
  virtual void CreatePassword();
protected:
  vector<std::string::size_type> posStar;

};

#endif // GENPASSW_H