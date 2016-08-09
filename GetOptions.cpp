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
    msg = msg+  "List of options:\n";
    propOptions.GetHelpMessages(msg);
    PrintMSG(msg.c_str(),"");
    throw ex_little_option() ;
	//exit(0);
}

/*int CGetOptions::StringToInt(string & s,const char * option){
  int length=s.length();

  for (int i=0; i<length; i++){
    if (not isdigit(s[i]))
    {
        PrintMSG("no digital value by ",option);
        throw ex_option_no_digital_argument() ;
        //exit(0);
    }
  }
  return atoi(s.c_str());
}*/


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
                      " -l length  - length password, not compatible with -m\n",
                      &IsFindOpitonL);
                      
  propOptions.registerOption( "-m",
                      &mask,
                      " -m mask    - mask for password, not compatible with -l, \n"
                      "              for example: \"ma*k\"\n",
                      &IsFindOpitonM); 
                      
   propOptions.registerOption( "-z",
                      &path7zip,
                      " -z path    - 7 zip program path, default: \n"
                      "              \"C:\\Program Files\\7-Zip\\7z.exe\"\n",
                      NULL);                       
                      
   propOptions.registerOption( "-a",
                      &arhiveName,
                      " -a name    - archive name,option not compatible with -u,\n"
                      "              default the option is enabled with \n"
                      "              [archive name]=test.zip \n",                      
                      &IsFindOpitonA);  

   propOptions.registerOption( "-u",
                      &userName,
                      " -u name   - User name from local system,\n"
                      "             option not compatible with -a"
                      "             default the option is disabled",
                      &IsFindOpitonU);                        
                      
   propOptions.registerOption( "-v",
                      NULL,
                      " -v         - verbose\n",
                      &IsFindOpitonV);                       

}

void CGetOptions::TestResultParse(){
        if (IsFindOpitonL && IsFindOpitonM ) {
              PrintMSG( "error option -l not compatible with  option -m","");
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

        if (not IsFindOpitonL && not IsFindOpitonM ) {
              PrintMSG( "error required option -l or -m","");
              throw ex_required_option_l_or_m ();
              //throw THROW_REQUIRED_OPTION_L_OR_M;
              //exit(0);
        }
        if (IsFindOpitonA && IsFindOpitonU ) {
              PrintMSG( "error option -a not compatible with  option -u","");
              throw ex_option_a_with_u();

        }
        
        if (not IsFindOpitonU){
            CheckExistFile(path7zip);
            CheckExistFile(arhiveName);
        }

}

CGetOptions::CGetOptions(int argc, char* argv_[])
{        
        registerAllOptions();
        argCount=argc;
        argv=argv_;

        /*IsFindOpitonL = false;
        IsFindOpitonM = false;
        IsFindOpitonV = false;
        IsFindOpitonA = false;
        IsFindOpitonU = false;  инициализируются в регистраторе  */    

        range="0-9";
        arhiveName="test.zip" ;
        path7zip="C:\\Program Files\\7-Zip\\7z.exe"; 
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
        
        /*if (IsFindOpitonL){
           lengthPassword=StringToInt(sLengthPassword, "length password");
        }*/

        TestResultParse();

}
