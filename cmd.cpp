//g++ ParseRangesInMask.cpp RangeChar.cpp PropOption.cpp GenPassword.cpp FindPassword.cpp GetOptions.cpp cmd.cpp -o cmd2
 //-mric* -uIF -r "a-z" -v
#include <iostream> //для  cout, cin
#include <cstdlib> // для exit
#include <ctime> //  для time
#include <cmath> // для %
#include "GetOptions.h"
#include "FindPassword.h"
#include "GenPassword.h"

using std::cout;
using std::endl;
using std::cin;

time_t tStart, tEnd;
CAbstractGenPassword * genPassword =NULL;

void ExitProg(){
  cin.get(); 
  exit(0);
}

CAbstractGenPassword * GetObjGenPassword(CGetOptions * options){
    CAbstractGenPassword * genPassword;
    
        if (options->IsFindOpitonL){
           genPassword=new CGenPassword(options->sLengthPassword,options->range);
        }
        else if (options->IsFindOpitonM){
           if (options->IsFindOpitonE){
              genPassword=new CGenPasswordOnMask(options->mask);
           }
           else{    
              genPassword=new CGenPasswordOnMask(options->mask,options->range);
           }
           
        }  
        else {
           genPassword=new CGenPasswordFromDict(options->dicPath,options->translitType);
        } 
    if (genPassword->LastError() != ERROR_NONE){
       cout<< genPassword->msgErr<<  endl;
	   ExitProg();
    }
    return genPassword;
}

CFindPassword * GetObjFindPassword(CGetOptions * options){
    CFindPassword * findPassword;

        if (options->IsFindOpitonU ){
           findPassword=new CUserLogon(options->userName,options->IsFindOpitonV);
        }
        else {
           findPassword=new CArhiveWith_Dll7z(options->arhiveName,options->IsFindOpitonV);;
        }

    return findPassword;
}

void ShowStat(){
        time(&tEnd);  // получаем время конца работы программы
        double seconds = difftime(tEnd, tStart);
        cout<< "elapsed time: ";
        if (seconds>60){
            cout<< int (seconds/60) << " min ";
        }
        cout<< (int(seconds) % 60 )<< " sec"<<endl;
        
        if (genPassword==NULL) return;
        float speed=genPassword->numbPassword;
        if (seconds!=0){
            speed/=seconds ;
        }
   
        cout<< "speed: "<< speed << " pasw/sec"<<endl;
}

void ShowLastPassword(){
        cout<<"last password: \"" << genPassword->password<<"\"\n";
}


BOOL WINAPI HandlerRoutine (DWORD dwCtrlType){
  if (dwCtrlType==CTRL_C_EVENT){  
     cout<<"interrupted by the user\n";
     ShowLastPassword();
     ShowStat();     
     exit(0);
     return true;
  } 
  return false;
}


int  main(int argc, char* argv[])
{       

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, true);

    try{
        CGetOptions  options(argc,  argv); 
        CFindPassword * findPassword = GetObjFindPassword(&options);
        genPassword = GetObjGenPassword(&options);
                        
        time(&tStart); // получаем время начала работы программы
        if (not findPassword->DoFind(*genPassword)){
           cout<< "password not found                       "<< endl;
        };
        ShowStat();
    }
    catch(...){
      ExitProg();
    }
    cin.get();  
	return 0;
}

