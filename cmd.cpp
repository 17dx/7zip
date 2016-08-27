//g++ directThread.cpp common.cpp ParseRangesInMask.cpp RangeChar.cpp PropOption.cpp GenPassword.cpp FindPassword.cpp GetOptions.cpp cmd.cpp -o cmd2 -lole32  -luuid

// -l -lole32 для: CLSIDFromProgID CoCreateInstance
// -luuid для: IID_IUnknown  IID_IDispatch
//-loleaut32 для: SysAllocString

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
bool programFinished=false;
CRITICAL_SECTION cs_DoStop;


bool DoStopTheProgram(){
     bool result=false;
     EnterCriticalSection(&cs_DoStop) ;
         if (!programFinished ){
             programFinished=true;
             result=true;
         }
     LeaveCriticalSection(&cs_DoStop) ;
     return result;
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

void ExitProg(){
  if (DoStopTheProgram() && (directThread!=NULL) ){
         ShowEndStat();
         directThread->~CDirectThread();
  }

  cin.get();
  exit(0);
}

void ShowLastPassword(){
        cout<<"last password: " ;
        directThread->PrintLastPassword();
        cout<<endl<<endl;
}


BOOL WINAPI HandlerRoutine (DWORD dwCtrlType){
  if (dwCtrlType==CTRL_C_EVENT){
     if (DoStopTheProgram() && (directThread!=NULL) ) {
         directThread->FinishThreads();
         cout<<"interrupted by the user\n";
         ShowLastPassword();
         ShowEndStat();
         directThread->~CDirectThread();
     }
     exit(0);
     return true;
  }
  return false;
}

int  main(int argc, char* argv[])
{
    InitializeCriticalSection(&cs_DoStop);
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, true);
    try{
        CGetOptions  options(argc,  argv);
        directThread = new CDirectThread(&options);
        time(&tStart); // получаем время начала работы программы
        directThread->Run();
    }
    catch(...){
      ExitProg();
      return 0;
    }
    ExitProg();
	return 0;
}

