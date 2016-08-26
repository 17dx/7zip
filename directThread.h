#ifndef DIRECTTHREAD_H
#define DIRECTTHREAD_H

#include "noerrors.h"
#include "common.h"

#include <cmath> // для %
#include "GetOptions.h"
#include "FindPassword.h"
#include "GenPassword.h"

class CDirectThread{
public:
  CGetOptions * options;
  static CAbstractGenPassword ** genPasswordArr;
  static int currThread;
  static bool lockRunNewThread;
  static CFindPassword * findPassword;
  CEventError eventError;
  int countThread; 
  TCount total_count;
  CDirectThread(CGetOptions * options_);

  void Run();
  TCount CalcSum();
  void PrintLastPassword();
protected:
  HANDLE hCout;
  CONSOLE_SCREEN_BUFFER_INFO binfo;
  
  CFindPassword * GetObjFindPassword();
  CAbstractGenPassword * GetObjGenPassword(); 
  void SplitTaskOnThread();
  void PrintStat(TCount sum); 
  void EraseLinesCOut()  ;
};

#endif // DIRECTTHREAD_H