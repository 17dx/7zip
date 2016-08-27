#include <iostream> //для  cout, cin
#include <iomanip>      // std::setfill, std::setw
#include  "directThread.h"
#include <windows.h>

using std::cout;
using std::endl;

using std::setfill;
using std::setw;
 
CAbstractGenPassword ** CDirectThread::genPasswordArr;


CFindPassword * CDirectThread::findPassword;

DWORD WINAPI ThreadStart(LPVOID pcurrThread){
   int currThread= *static_cast<int*> (pcurrThread);
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

bool CDirectThread::HaveFinishedAllThread(){
  int countFinished=0;
  for(int i=0; i<countThread; i++)  {    
     if (WaitForSingleObject(hThreats[i],0)==WAIT_FAILED){
         countFinished++;
     };     
  }
  return (countFinished==countThread);
  
}

void CDirectThread::PrepareConsoleScreen(){
  if (! options->IsFindOpitonV){
      EraseLinesCOut();
      SetConsoleCursorPosition(hCout,binfo.dwCursorPosition);
  }
}

void CDirectThread::Run(){
  SplitTaskOnThread();
  EXIT_IF_ERROR() ; 
  cout<<"count passwords:" << std::fixed<<std::setprecision(0)<< total_count<< endl; 
  
  for(int i=0; i<countThread; i++) {  
   argThread[i]=i;  
   hThreats[i]=CreateThread(NULL, 0, ThreadStart, &argThread[i], 0, NULL);

  }
  
  hCout=GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hCout, &binfo);
  binfo.dwCursorPosition.Y-=countThread; //не знаю почему нужно вычитать
  
  for(;;)  {
   Sleep((DWORD)1*1000);
   if (findPassword->FindOK()){
       PrepareConsoleScreen();
       cout<< "\npassword found: \""<< findPassword->truePassword <<"\"                  "<< endl;
       return ;//true; 
   }
   else{      
      if (! options->IsFindOpitonV){
          TCount sum=CalcSum();
          PrintStat(sum);
          PrintLastPassword();
          SetConsoleCursorPosition(hCout,binfo.dwCursorPosition);
      }
      
      if ( HaveFinishedAllThread() ){ //todo исправить условие окончания работыsum>=total_count
         PrepareConsoleScreen();
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
    hThreats=new HANDLE [countThread];
    argThread=new int [countThread];
    for(int i=0; i<countThread; i++)  {       
       if (countThread==1){
         genPasswordArr[i]=genPassword;
         //если поток всего один управление настройками не требуется
         //и новый создавать нет смысла поэтому копируем genPassword
       }
       else{
           genPasswordArr[i]=GetObjGenPassword();
           genPasswordArr[i]->SetNewLowerBoundary(count_for_each*i);
           genPasswordArr[i]->SetNewUpperBoundaryAsMax(count_for_each);       
           genPasswordArr[i]->ReCreateFirstPassword();
       }
       string extraInfo="test";
       extraInfo+=to_string(i);
       genPasswordArr[i]->InitExtraInfo(extraInfo);
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
           findPassword=new CArhiveWith_Dll7z(options->arhiveName,options->IsFindOpitonV);
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