//g++ directThread.cpp common.cpp ParseRangesInMask.cpp RangeChar.cpp PropOption.cpp GenPassword.cpp FindPassword.cpp GetOptions.cpp cmd.cpp -o cmd2
 //-mric* -uIF -r "a-z" -v
#include <iostream> //для  cout, cin
#include <cstdlib> // для exit
#include <ctime> //  для time
#include <cmath> // для %
#include "GetOptions.h"
#include "FindPassword.h"
#include "GenPassword.h"
#include "directThread.h"

using std::cout;
using std::endl;
using std::cin;


time_t tStart, tEnd;
CDirectThread * directThread=NULL;

void ExitProg(){
  cin.get(); 
  exit(0);
}

void ShowEndStat(){
        time(&tEnd);  // получаем время конца работы программы
        double seconds = difftime(tEnd, tStart);
        cout<< "elapsed time: ";
        if (seconds>60){
            cout<< int (seconds/60) << " min ";
        }
        cout<< (int(seconds) % 60 )<< " sec"<<endl;
        
        if (directThread==NULL) return;
        float speed=directThread->CalcSum();
        if (seconds!=0){
            speed/=seconds ;
        }
   
        cout<< "speed: "<< speed << " pasw/sec"<<endl;
}

void ShowLastPassword(){
        cout<<"last password: " ;
        directThread->PrintLastPassword();
        cout<<endl<<endl;
}


BOOL WINAPI HandlerRoutine (DWORD dwCtrlType){
  if (dwCtrlType==CTRL_C_EVENT){  
     cout<<"interrupted by the user\n";
     ShowLastPassword();
     ShowEndStat();     
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
        directThread = new CDirectThread(&options);
        time(&tStart); // получаем время начала работы программы
        directThread->Run();
        //CFindPassword * findPassword = GetObjFindPassword(&options);
        //genPassword = GetObjGenPassword(&options);      
        //time(&tStart); // получаем время начала работы программы
        /*if (not findPassword->DoFind(*genPassword)){
           cout<< "password not found                       "<< endl;
        };*/
        ShowEndStat();
    }
    catch(...){
      ExitProg();
    }
    cin.get();  
	return 0;
}

