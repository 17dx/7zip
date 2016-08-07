#ifndef PROPOPTION_H
#define PROPOPTION_H
#include <map>
#include <string>
using std::map;
using std::string;
using std::pair;

struct SPropOption{
  //const char * option;
  //bool NoneArg;
  string* valueString;
  //int* valueInt;
  const char * helpMsg;
  bool* isFind;
};

class CPropOptions{
public:
  map <string,SPropOption> items; 
  
  /*void registerOption( const char * option,
                      int typeArgum,
                      string* valueString,
                      int* valueInt,
                      const char * helpMsg,
                      bool* exist);*/
  void registerOption( const char * option,
                      string* valueString,
                      const char * helpMsg,
                      bool* isFind);                      
  bool NeedArgum(string& option);
  bool OptionIsRegistered(string& option);
  void SetArgum(string&  option,string& value);
  void SetIsFind(string&  option);
  void GetHelpMessages(string& msg);
};


#endif // PROPOPTION_H