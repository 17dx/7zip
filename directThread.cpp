#include <iostream> //для  cout, cin
#include <iomanip>      // std::setfill, std::setw
#include  "directThread.h"
#include <windows.h>

using std::cout;
using std::endl;

using std::setfill;
using std::setw;
 
CAbstractGenPassword ** CDirectThread::genPasswordArr;
int CDirectThread::currThread;
bool CDirectThread::lockRunNewThread;

CFindPassword * CDirectThread::findPassword;

DWORD WINAPI ThreadStart(LPVOID){
   int currThread=CDirectThread::currThread;
   CDirectThread::lockRunNewThread=false;
   CDirectThread::findPassword->DoFind(
             *CDirectThread::genPasswordArr[currThread]);
         
}

CDirectThread::CDirectThread(CGetOptions * options_){
  options= options_;
  countThread=0;
  findPassword = GetObjFindPassword();
}

void CDirectThread::EraseLinesCOut(){
   for(int i=0; i<=countThread; i++){
       cout<<"                                          \n";
   }  
}

void CDirectThread::Run(){
  SplitTaskOnThread();
  EXIT_IF_ERROR() ; 
  cout<<"count passwords:" << std::fixed<<std::setprecision(0)<< total_count<< endl; 
  
  for(currThread=0; currThread<countThread; currThread++) {
   lockRunNewThread=true;     
   CreateThread(NULL, 0, ThreadStart, NULL, 0, NULL);
   while (lockRunNewThread){
     Sleep((DWORD)1*1000);
   }
  }
  hCout=GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hCout, &binfo);
  binfo.dwCursorPosition.Y-=countThread; //не знаю почему нужно вычитать
  for(;;)  {
   Sleep((DWORD)1*1000);
   if (findPassword->FindOK()){
       EraseLinesCOut();
       SetConsoleCursorPosition(hCout,binfo.dwCursorPosition);
       cout<< "\npassword found: \""<< findPassword->truePassword <<"\"                  "<< endl;
       return ;//true; 
   }
   else{
      TCount sum=CalcSum();
      PrintStat(sum);
      PrintLastPassword();
      SetConsoleCursorPosition(hCout,binfo.dwCursorPosition);
      if (sum>=total_count){
         EraseLinesCOut();
         SetConsoleCursorPosition(hCout,binfo.dwCursorPosition);
         cout<< "\npassword not found                          "<< endl;
         break;
      }
   }   
  } 
  return ;//false
}

void CDirectThread::SplitTaskOnThread(){  
  countThread=StringToInt(options->countThread);
  EXIT_IF_ERROR() ;
  
  if (countThread>0){
    CAbstractGenPassword* genPassword=GetObjGenPassword();
    EXIT_IF_ERROR() ;
    total_count = genPassword->CountPasswords();
   
    TCount count_for_each=total_count/countThread;
    TCount remain=total_count - count_for_each*countThread;
   //остаток достанется последнему потоку  
    genPasswordArr= new CAbstractGenPassword* [countThread];
    
    for(int i=0; i<countThread; i++)  {
       string extraInfo="test";
       extraInfo+=to_string(i);
       genPasswordArr[i]=GetObjGenPassword();
       genPasswordArr[i]->SetNewLowerBoundary(count_for_each*i);
       genPasswordArr[i]->SetNewUpperBoundaryAsMax(count_for_each);
       genPasswordArr[i]->passwordInfo.extraInfo=extraInfo.c_str();
       genPasswordArr[i]->ReCreateFirstPassword();
    } 
    //добавляем остатки к последнему
    genPasswordArr[countThread-1]->upperBoundaryCountPassword+= remain;
     

  }
  else{
    eventError.CreateEventError("error count thread less or equal zero",
                                ERROR_LITTLE_COUNT_THREAD);
    return ;
  }
}

TCount CDirectThread::CalcSum(){
    TCount sum=0;
    for(int i=0; i<countThread; i++){
       sum+=genPasswordArr[i]->numbPassword;
    }
    return sum;
}

void CDirectThread::PrintLastPassword(){
     for(int i=0; i<countThread; i++)  {
           cout << "\n:: \"" << genPasswordArr[i]->password <<"\"";
    } 
  
}

void CDirectThread::PrintStat(TCount sum){
    int percent = (static_cast<double> (sum)/total_count*100);
    cout << setfill(' ') << setw(3)<< percent <<"%                                ";
    
           
}

CFindPassword * CDirectThread::GetObjFindPassword(){
    CFindPassword * findPassword;

        if (options->IsFindOpitonU ){
           findPassword=new CUserLogon(options->userName,options->IsFindOpitonV);
        }
        else {
           findPassword=new CArhiveWith_Dll7z(options->arhiveName,options->IsFindOpitonV);;
        }

    return findPassword;
}


CAbstractGenPassword * CDirectThread::GetObjGenPassword(){
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
    if (options->IsFindOpitonS){
      genPassword->SetNewLowerBoundary(options->startValue);           
      genPassword->ReCreateFirstPassword();
    }
    
   
    return genPassword;
}