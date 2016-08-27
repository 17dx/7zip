#include "GetOptions.h"

#include <iostream> //для  cout, cin
#include <io.h>  //для access
#include <cstdlib> // для exit
#include <cstring> //для strcmp
#include <windows.h> //DWORD

#define ANY_TWO_OPTION_TOGETHER(opt1,opt2,opt3)\
  (opt1 && opt2) || (opt1 && opt3 ) || (opt2 && opt3)

using std::cout;

void CGetOptions::PrintMSG(const char  * cMsg1, const char  * cMsg2){
   cout<<cMsg1<<cMsg2<< "\n" ;
}


void CGetOptions::printHelp()
{
    string msg;
    msg = msg+  "List of options:\n";
    propOptions.GetHelpMessages(msg);
    PrintMSG(msg.c_str(),"");
    PrintMSG("example work with arhive:\n",
             " pget -a arh.zip -l4 -r \"A-Z\"");
    PrintMSG("example work with local user:\n",
             " pget -u ADMIN -m \"1**1\" -r \"1-4,a-e\"");
    PrintMSG("example work with document:\n",
             " pget -w secret.docx -e -m \"[1-2,5]{2}[\\{,*,+]\"" );
    throw ex_little_option() ;
	//exit(0);
}


string CGetOptions::getNextArgAsString(int numArg){
   string result;
   int nextNumArg=numArg+1;
   if ( nextNumArg < argCount ){
        result=argv[nextNumArg];
        if (result.length()==2){
          if (propOptions.OptionIsRegistered(result)
             ) {
               PrintMSG("skipt arguments for option ",argv[numArg]);
               throw ex_skip_argument()  ;
           }
        }

   }
   else{
       PrintMSG("skipt arguments for option  ",argv[numArg]);
       throw ex_skip_argument()  ;
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
    throw ex_file_not_found ();
    //exit(0);
 }

}

void CGetOptions::registerAllOptions(){
  propOptions.registerOption( "-r",
                      &range,
                      " -r range   - range of characters, default: \"0-9\" \n",
                      NULL);

  propOptions.registerOption( "-l",
                      &sLengthPassword,
                      " -l length  - length password, not compatible with -m and -d \n",
                      &IsFindOpitonL);

  propOptions.registerOption( "-m",
                      &mask,
                      " -m mask    - mask for password, not compatible with -l and -d, \n"
                      "              for example: \"ma*k\"\n"
                      "              behavior modification using option -e \n",
                      &IsFindOpitonM);



  propOptions.registerOption( "-d",
                      &dicPath,
                      " -d path    - path to dictionary, not compatible with -l and -m end -n, \n"
                      "              for example: \"dic.txt\"\n"
                      "              behavior modification using option -t \n",
                      &IsFindOpitonD);

  propOptions.registerOption( "-s",
                      &startValue,
                      " -s value   - start value for password\n",
                      &IsFindOpitonS);

   propOptions.registerOption( "-z",
                      &path7zip,
                      " -z path    - 7 zip program path, default: \n"
                      "              \"C:\\Program Files\\7-Zip\\7z.exe\"\n",
                      NULL);

   propOptions.registerOption( "-a",
                      &arhiveName,
                      " -a name    - archive name,option not compatible with -u and -w,\n"
                      "              default the option is enabled with \n"
                      "              [archive name]=test.zip \n",
                      &IsFindOpitonA);

   propOptions.registerOption( "-u",
                      &userName,
                      " -u name    - User name from local system,\n"
                      "              option not compatible with -a and -w\n",
                      &IsFindOpitonU);

  propOptions.registerOption( "-w",
                      &docName,
                      " -w name    - doc name, not compatible with -u and -a\n",
                      &IsFindOpitonW);

  propOptions.registerOption( "-t",
                      &sTranslitType,
                      " -t type    - translit word from dictionary, used with option -d\n"
                      "              available types: both and only\n",
                      &IsFindOpitonT);

  propOptions.registerOption( "-n",
                      &countThread,
                      " -n count   - count thread, not compatible with option -d\n"
                      "              default count = 1\n",
                      &IsFindOpitonN);

   propOptions.registerOption( "-v",
                      NULL,
                      " -v         - verbose\n",
                      &IsFindOpitonV);

   propOptions.registerOption( "-e",
                      NULL,
                      " -e         - extend mask\n",
                      &IsFindOpitonE);


}

void CGetOptions::TestResultParse(){
        if (ANY_TWO_OPTION_TOGETHER(IsFindOpitonL , IsFindOpitonM , IsFindOpitonD)) {
              PrintMSG( "error options incompatible: -l and -m and -d","");
              throw ex_option_l_with_m();
              //throw THROW_OPTION_L_WITH_M;
              //exit(0);
        }

       /* if (IsFindOpitonL && (lengthPassword==0 )) {
              PrintMSG( "little length password","");
              throw ex_little_length_password() ;
              //throw THROW_LITTLE_LENGTH_PASSWORD;
              //exit(0);
        }*/

        if (not IsFindOpitonL && not IsFindOpitonM && not IsFindOpitonD) {
              PrintMSG( "error required option -l or -m or -d","");
              throw ex_required_option_l_or_m ();
              //throw THROW_REQUIRED_OPTION_L_OR_M;
              //exit(0);
        }
        //if (IsFindOpitonA && IsFindOpitonU && IsFindOpitonW) {
        if ( ANY_TWO_OPTION_TOGETHER( IsFindOpitonA, IsFindOpitonU, IsFindOpitonW)){
              PrintMSG( "error not compatible with option -a and -u and -w","");
              throw ex_option_a_with_u();

        }

        if (IsFindOpitonN && IsFindOpitonD ) {
              PrintMSG( "error option -n not compatible with  option -d","");
              throw ex_option_n_with_d();

        }


        if (not IsFindOpitonU && not IsFindOpitonW){
            CheckExistFile(path7zip);
            CheckExistFile(arhiveName);
        }
        else if (IsFindOpitonW) {
            bool docNameAsFullPath=false;
            if (docName.size()>2){
              docNameAsFullPath = (docName[1]==':');
              if ( (!docNameAsFullPath) && docName.size()>4){
                  docNameAsFullPath = ( docName.substr(0,4) == "\\??\\" );
              }
            }

            if (! docNameAsFullPath){
                DWORD sizeBuff = GetCurrentDirectoryA(0, NULL);
                char buffer[sizeBuff];
                GetCurrentDirectoryA(sizeBuff, buffer);
                docName= string(buffer) +"\\"+ docName;
            }
            CheckExistFile(docName);
        }

        translitType=TYPE_TRANSLIT_NONE;
        if ( IsFindOpitonT){
            if (sTranslitType== ""){
               translitType=TYPE_TRANSLIT_NONE;
            }
            else if (sTranslitType== "both"){
                translitType=TYPE_TRANSLIT_BOTH;
            }
            else if (sTranslitType== "only"){
               translitType=TYPE_TRANSLIT_ONLY;
            }
            else{
               PrintMSG( "invalid keyword by option -t","");
               throw ex_option_not_valid_keyword();
            }
        }

}

CGetOptions::CGetOptions(int argc, char* argv_[])
{
        registerAllOptions();
        argCount=argc;
        argv=argv_;


        range="0-9";
        arhiveName="test.zip" ;
        path7zip="C:\\Program Files\\7-Zip\\7z.exe";

        countThread="1";
        //lengthPassword=0;

        if (argCount < 2){
	         printHelp(); // will not return
        }

        for (int i=0; i < argc; i+=1){
              string opt = argv[i];
              string argValue;
              bool OptAndArgumTogether = (opt.length() >2);

              if (OptAndArgumTogether){
                  argValue=opt.substr(2,opt.length()-2);
                  opt=opt.substr(0, 2);
              }
              if (propOptions.OptionIsRegistered(opt) ){
                 propOptions.SetIsFind(opt);
                 if (propOptions.NeedArgum(opt) ) {
                   if (not OptAndArgumTogether){
                      argValue=getNextArgAsString(i);
                   }
                   propOptions.SetArgum(opt,argValue) ;
                 }
              }
        }


        TestResultParse();

}
