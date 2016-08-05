#ifndef GETOPTIONS_H
#define GETOPTIONS_H
#include <string>
#include <exception> //для  exception

using std::string;


class ex_file_not_found : public std::exception{};
class ex_invalid_arguments : public std::exception{};
class ex_option_l_with_m: public std::exception{};
class ex_option_no_digital_argument : public std::exception{};
class ex_little_option : public std::exception{};
class ex_skip_argument  : public std::exception{};
class ex_required_option_l_or_m : public std::exception{};
class ex_little_length_password : public std::exception{};
class ex_range_char_not_valid : public std::exception{};

class CGetOptions{
 public:
   CGetOptions(int argc, char* argv_[]);
   int getNextArgAsInt(int numArg);
   void printHelp();

   string getNextArgAsString(int numArg);
   string range;
   int lengthPassword;
   string mask;
   string path7zip;// без кавычек
   string arhiveName;

   bool ExistOpitonL;
   bool ExistOpitonM;

 private:
   int argCount;
   char** argv;
   void CheckExistFile(string& fileName);
   //void PrintMSG(string & msg);
   void PrintMSG(const char  * cMsg1, const char  * cMsg2);
};

#endif // GETOPTIONS_H