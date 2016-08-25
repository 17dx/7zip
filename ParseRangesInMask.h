#ifndef PARSERANGESINMASK_H
#define PARSERANGESINMASK_H

#include "noerrors.h"
#include <string>
#include <vector>

#include <cstdlib> //Для atoi 

using std::string;
using std::vector;

class CParseRangesInMask{
public:
  string newMask;
  string msgErr;
  CParseRangesInMask(string& mask,
                     vector <string>* ranges_,
                     vector<std::string::size_type>* posPointInsert_);
  
  bool Parse( ); 
  int LastError();  
protected:
  size_t pos;
  int lenMask;
  bool existEscapeCharacter;
  string ext_mask;
  vector <string>* ranges;
  vector<std::string::size_type>* posPointInsert;
  
  int codeError;
  bool RepeatRange(string & range);
  void AppendStarToNewMask();
  void AppendCharToNewMask(char fromMask);
  bool GetSubRange( string& subrange,
                    char markStart, 
                    char markEnd) ;
  bool AppendSubRange(string & range);
  int StringToInt(string & s);
};

#endif // PARSERANGESINMASK_H
