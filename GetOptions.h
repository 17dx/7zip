#ifndef GETOPTIONS_H
#define GETOPTIONS_H
#include <string>
#include <exception> //для  exception
#include "PropOption.h"

using std::string;


class ex_file_not_found : public std::exception{};
class ex_invalid_arguments : public std::exception{};
class ex_option_l_with_m: public std::exception{};
class ex_option_a_with_u: public std::exception{};
class ex_option_no_digital_argument : public std::exception{};
class ex_little_option : public std::exception{};
class ex_skip_argument  : public std::exception{};
class ex_required_option_l_or_m : public std::exception{};
class ex_little_length_password : public std::exception{};
class ex_range_char_not_valid : public std::exception{};


class CGetOptions{
 public:
   CGetOptions(int argc, char* argv_[]);
   
   string range;
   string sLengthPassword;
   //int lengthPassword;
   string mask;
   string path7zip;// без кавычек
   string arhiveName;
   string userName;

   bool IsFindOpitonL;
   bool IsFindOpitonM;
   bool IsFindOpitonV;
   bool IsFindOpitonA;
   bool IsFindOpitonU;

 private:
   int argCount;
   char** argv;
   CPropOptions propOptions;
   void CheckExistFile(string& fileName);
   //void PrintMSG(string & msg);
   void PrintMSG(const char  * cMsg1, const char  * cMsg2);   
   void printHelp();
   string getNextArgAsString(int numArg);
   //int StringToInt(string & s,const char * option);
   void registerAllOptions();
   void TestResultParse();
};

#endif // GETOPTIONS_H