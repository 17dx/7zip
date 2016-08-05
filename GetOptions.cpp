#include "GetOptions.h"

#include <iostream> //для  cout, cin
#include <io.h>  //для access
#include <cstdlib> // для exit
#include <cstring> //для strcmp

using std::cout;

void CGetOptions::PrintMSG(const char  * cMsg1, const char  * cMsg2){
   cout<<cMsg1<<cMsg2<< "\n" ;
}


void CGetOptions::printHelp()
{    
    string msg;
    msg = msg+  "List of options:\n"
         + " -r range   - range of characters, default: \"0-9\" \n"
         + " -l length  - length password, not compatible with -m\n"
         + " -m mask    - mask for password, not compatible with -l, \n"
         + "              for example: \"ma*k\"\n"
         + " -z path    - 7 zip program path, default: \n"
         + "              \"C:\\Program Files\\7-Zip\\7z.exe\"\n"
         + " -a name    - archive name, default: test.zip\n";
    PrintMSG(msg.c_str(),"");
    throw THROW_LITTLE_OPTION;
     
	//exit(0);
}



int CGetOptions::getNextArgAsInt(int numArg){
  string ArgAsString=getNextArgAsString(numArg);

  int length=ArgAsString.length();

  for (int i=0; i<length; i++){
    if (not isdigit(ArgAsString[i]))
    {
        PrintMSG("no digital argument by ",argv[numArg]);
        throw THROW_OPTION_NO_DIGITAL_ARGUMENT;
        //exit(0);
    }
  }
  return atoi(ArgAsString.c_str());
}


string CGetOptions::getNextArgAsString(int numArg){
   string result;
   int nextNumArg=numArg+1;
   if ( nextNumArg < argCount ){
        result=argv[nextNumArg];
        if (result.length()==2){
          if (result[0]=='-' 
              && (  result[1]=='r' 
                 || result[1]=='l' 
                 || result[1]=='m' 
                 || result[1]=='z' 
                 || result[1]=='a' 
               )
             ) {
               PrintMSG("skipt arguments for option ",argv[numArg]);
               throw THROW_SKIP_ARGUMENT;
           }
        }

   }
   else{
       PrintMSG("invalid arguments by option  ",argv[numArg]);
       throw THROW_INVALID_ARGUMENTS;
       //exit(0);
   }
   return result;
}

void CGetOptions::CheckExistFile(string& fileName){
 bool fexist = (access(fileName.c_str(), 0)==0) ;
 if (not fexist) {
    string msg;
    msg= msg + "Error file: " + fileName + " not found!\n";
    PrintMSG(msg.c_str(),"");
    throw THROW_FILE_NOT_FOUND;
    //exit(0);
 }

}


CGetOptions::CGetOptions(int argc, char* argv_[])
{       argCount=argc;
        argv=argv_;// new char* [argc];

        bool ExistOpitonL = false;
        bool ExistOpitonM = false;

        range="0-9";
        arhiveName="test.zip" ;
        path7zip="C:\\Program Files\\7-Zip\\7z.exe"; 
        lengthPassword=0;
        
        if (argCount < 2){
	         printHelp(); // will not return
        }

        for (int i=0; i < argc; i+=1){
              const char* arg = argv[i];
              if (strcmp(arg, "-r") == 0){
                  range=getNextArgAsString(i);
              }
              if (strcmp(arg, "-l") == 0){
                  lengthPassword=getNextArgAsInt(i);
                  ExistOpitonL = true;
              }
              if (strcmp(arg, "-m") == 0){
                  mask=getNextArgAsString(i);
                  ExistOpitonM = true;
              }
              if (strcmp(arg, "-z") == 0){
                  path7zip=getNextArgAsString(i);
              }
              if (strcmp(arg, "-a") == 0){
                  arhiveName=getNextArgAsString(i);
              }
        }
        
        if (ExistOpitonL && ExistOpitonM ) {
              PrintMSG( "error option -l not compatible with  option -m","");
              throw THROW_OPTION_L_WITH_M;
              //exit(0);
        }
        
        if (ExistOpitonL && (lengthPassword==0 )) {
              PrintMSG( "little length password","");
              throw THROW_LITTLE_LENGTH_PASSWORD;
              //exit(0);
        }

        if (not ExistOpitonL && not ExistOpitonM ) {
              PrintMSG( "error required option -l or -m","");
              throw THROW_REQUIRED_OPTION_L_OR_M;
              //exit(0);
        }

        CheckExistFile(path7zip);
        CheckExistFile(arhiveName);
	
}
