#include "FindPassword.h"

#include <iostream> //для  cout, cin
#include <iomanip>      // std::setfill, std::setw
#include <fstream> //для ifstream
#include <stdlib.h> //для system


using std::cout;
using std::endl;

using std::setfill;
 using std::setw;

CFindPassword::CFindPassword(bool verbose_){
    verbose = verbose_;
    findOK=false;
}

 CFindPassword::~CFindPassword(){
}

bool CFindPassword::FindOK(){
    return findOK;
}

bool CFindPassword::DoFind(CAbstractGenPassword& genPassword){
  /*TCount countPasswords= genPassword.CountPasswords();
  cout<<"count passwords:" << std::fixed<<std::setprecision(0)<< countPasswords<< endl; 
  int lastPercent =0;
  int percent=0;*/
  do{
     
     if (verbose){
         cout<< genPassword.password<< endl; //todo
     }
     /*else{
        percent = (static_cast<double> (genPassword.numbPassword)/countPasswords*100);
        if (percent>lastPercent){
           lastPercent=percent;
           cout << setfill(' ') << setw(3)<< percent;
           cout << "% :: " << genPassword.password << "\r";
        }
     }*/
     
     if ( PasswordIsTrue(genPassword.passwordInfo) ){  
        truePassword= genPassword.password  ;
        findOK=true;        
        //cout<< "\npassword found: \""<< genPassword.password <<"\"                  "<< endl;
        return true; 
     };
  } 
  while( genPassword.Next() );
  return false;
}


// конструктор с параметром
CArhiveConsole7z::CArhiveConsole7z( string& pArhiveName,string& path7zip_,bool verbose_):
  CFindPassword( verbose_)
{
    //path7zip = "C:\\Program Files\\7-Zip\\7z.exe";
    path7zip = path7zip_;
    arhiveName = pArhiveName; 
    textout = "out.txt";
    sFind = "Wrong password"; //искомая строка в выводе
}

// функция запуска 7zip и проверки правильности пароля
bool CArhiveConsole7z::PasswordIsTrue(SPasswordInfo& passwordInfo){
    bool result = true; 
    string cmdLine = "\"" + path7zip + "\""  + " e " + arhiveName + 
                     " -p" + passwordInfo.password + " -y >" +textout  + " 2>&1";;
    //cout<<cmdLine<< endl;
    
    system( cmdLine.c_str());
    string line;
    std::ifstream file(textout.c_str() );
    while (std::getline(file, line))
    {
         //cout<< line<< endl;  // что прочитали из файла  textout
         if (line.find(sFind) != string::npos ){
             result = false;
             break; // разрушаем цикл
         };
    }   
    file.close();   
    return result;
}



CArhiveWith_Dll7z::CArhiveWith_Dll7z( string& pArhiveName, bool verbose_):
 CFindPassword( verbose_)
{
    arhiveName = pArhiveName; 
    HINSTANCE hDllInstance = LoadLibrary("wrapper7z.dll");
 
    if (hDllInstance == NULL){
        cout << "error load wrapper7z.dll" << endl;
        throw ex_error_load_7zdll();
    }
 
    fUnzip = (FUnzip)GetProcAddress(hDllInstance, "Unzip");
 
    if (fUnzip == NULL){
        cout << "error load function Unzip from wrapper7z.dll" << endl;
        throw ex_error_load_Unzip_from_7zdll();
    }
    
    FOpenArhive OpenArhive = (FOpenArhive)GetProcAddress(hDllInstance, "OpenArhive");
    if (OpenArhive == NULL){
        cout << "error load function OpenArhive from wrapper7z.dll" << endl;
        throw ex_error_load_OpenArhive_from_7zdll();
    }
    else {
      OpenArhive(pArhiveName.c_str());
    }

}

bool CArhiveWith_Dll7z::PasswordIsTrue(SPasswordInfo& passwordInfo){
   int result = fUnzip(passwordInfo.password, passwordInfo.extraInfo); 
   if (result==ERROR_ARHIVE_NOT_OPEN){
      cout << "logical error in wrapper7z.dll: Arhive not open" << endl;
      throw ex_logical_error_ArhiveNotOpen_in_7zdll();
   }
   return (result==SUCCESS_UNZIP);
}

CArhiveWith_Dll7z::~CArhiveWith_Dll7z(){
   FreeLibrary(hDllInstance);
}

CUserLogon::CUserLogon(string& pUserName,bool verbose_):CFindPassword( verbose_)
{
    userName=pUserName.c_str();
}

bool CUserLogon::PasswordIsTrue(SPasswordInfo& passwordInfo){
  //PHANDLE phToken;
  HANDLE  hToken;
  
  bool result=(LogonUser((LPSTR)userName,NULL,(LPSTR)passwordInfo.password,
               LOGON32_LOGON_SERVICE,
               LOGON32_PROVIDER_DEFAULT,
               &hToken) !=0);
  CloseHandle(hToken);             
  return result;
}
