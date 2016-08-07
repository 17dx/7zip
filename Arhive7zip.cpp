#include "Arhive7zip.h"

#include <iostream> //для  cout, cin
#include <fstream> //для ifstream
#include <stdlib.h> //для system


using std::cout;
using std::endl;

// конструктор с параметром
CArhive7zip::CArhive7zip( string& pArhiveName,string& path7zip_){
    //path7zip = "C:\\Program Files\\7-Zip\\7z.exe";
    path7zip = path7zip_;
    arhiveName = pArhiveName; 
    
    textout = "out.txt";
    sFind = "Wrong password"; //искомая строка в выводе
}

// функция запуска 7zip и проверки правильности пароля
bool CArhive7zip::Unzip(string& password){
    bool result = true; 
    string cmdLine = "\"" + path7zip + "\""  + " e " + arhiveName + 
                     " -p" + password + " -y >" +textout  + " 2>&1";;
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

bool CArhive7zip::FindPassword(CGenPassword& genPassword){
  do{
     cout<< genPassword.password<< endl; 
     if ( Unzip(genPassword.password) ){
        cout<< "password found: "<< genPassword.password << endl;
        return true; 
     };
  } 
  while( genPassword.Next() );
  return false;
}


CArhiveWrapper7zip::CArhiveWrapper7zip( string& pArhiveName,string& path7zip_):CArhive7zip(pArhiveName, path7zip_){
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

bool CArhiveWrapper7zip::Unzip(string& password){
   int result = fUnzip(password.c_str());
   if (result==ERROR_ARHIVE_NOT_OPEN){
      throw ex_logical_error_ArhiveNotOpen_in_7zdll();
   }
   return (result==SUCCESS_UNZIP);
}

CArhiveWrapper7zip::~CArhiveWrapper7zip(){
   FreeLibrary(hDllInstance);
}

