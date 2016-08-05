#include <string>

using std::string;

const int  THROW_FILE_NOT_FOUND             =1;
const int  THROW_INVALID_ARGUMENTS          =2;
const int  THROW_OPTION_L_WITH_M            =3;
const int  THROW_OPTION_NO_DIGITAL_ARGUMENT =4;
const int  THROW_LITTLE_OPTION              =5;
const int  THROW_SKIP_ARGUMENT              =6;
const int  THROW_REQUIRED_OPTION_L_OR_M     =7;
const int  THROW_LITTLE_LENGTH_PASSWORD     =8;
const int  THROW_RANGE_CHAR_NOT_VALID       =9;


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