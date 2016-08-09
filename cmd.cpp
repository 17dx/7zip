//g++ PropOption.cpp GenPassword.cpp Arhive7zip.cpp GetOptions.cpp cmd.cpp -o cmd2
 //-mric* -uIF -r "a-z" -v
#include <iostream> //для  cout, cin
#include <cstdlib> // для exit
#include <ctime> //  для time
#include "GetOptions.h"
#include "Arhive7zip.h"
#include "GenPassword.h"

using std::cout;
using std::endl;
using std::cin;


void ExitProg(){
  cin.get(); 
  exit(0);
}

CGenPassword * GetObjGenPassword(CGetOptions * options){
    CGenPassword * genPassword;
    try {
        if (options->lengthPassword >0){
           genPassword=new CGenPassword(options->lengthPassword,options->range);
        }
        else {
           genPassword=new CGenPasswordOnMask(options->mask,options->range);
        }   
    }
    catch(int){
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

int  main(int argc, char* argv[])
{   
    
    try{
        CGetOptions  options(argc,  argv); 
        CFindPassword * findPassword = GetObjFindPassword(&options);
        CGenPassword * genPassword = GetObjGenPassword(&options);
                
        time_t tStart, tEnd;
        time(&tStart); // получаем время начала работы программы
        if (not findPassword->DoFind(*genPassword)){
           cout<< "password not found "<< endl;
        };
        time(&tEnd);  // получаем время конца работы программы
        double seconds = difftime(tEnd, tStart);
        cout<< "elapsed time: "<< seconds << " sec"<<endl;
    }
    catch(...){
      ExitProg();
    }
    cin.get();  
	return 0;
}