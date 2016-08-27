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
  static CFindPassword * findPassword;
  int * argThread;

  CEventError eventError;
  int countThread;
  TCount total_count;
  CDirectThread(CGetOptions * options_);
  ~CDirectThread();
  void Run();
  TCount CalcSum();
  void PrintLastPassword();
  void FinishThreads();
protected:
  HANDLE hCout;
  CONSOLE_SCREEN_BUFFER_INFO binfo;
  HANDLE * hThreats;
  CFindPassword * GetObjFindPassword();
  CAbstractGenPassword * GetObjGenPassword();
  void SplitTaskOnThread();
  void PrintStat(TCount sum);
  void EraseLinesCOut()  ;
  bool HaveFinishedAllThread();
  void PrepareConsoleScreen();

};

#endif // DIRECTTHREAD_H
