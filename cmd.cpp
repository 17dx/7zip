#include <iostream> //для  cout, cin
#include <string>
#include <vector>
#include <fstream> //для ifstream
#include <stdlib.h> //для system
#include <ctime> //  для time
#include <io.h>  //для access
#include "GetOptions.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

class CGenPassword{
public:
  string password;
  CGenPassword();
  CGenPassword(int len);
  ~CGenPassword();
  void Init(int len);
  bool Next();
  virtual void CreatePassword();
protected:
  char minChar;
  char maxChar;
  int lenPassword;
  char* arr; //динамический массив из  чисел
  bool Inc(int index);

};

//конструктор
CGenPassword::CGenPassword(){
}

CGenPassword::CGenPassword(int len){
  Init(len);
  CreatePassword(); //создать начальный пароль
}

//проинициализировать переменные
// ::эта функция пригодится наследникам
void CGenPassword::Init(int len){
  minChar='0'; // с какого символа начать
  maxChar='9'; //каким символом закончить
  lenPassword=len;
  arr = new char [lenPassword]; //выделяем память под массив
  for(int i=0; i < lenPassword; i++){
    arr[i]=minChar;
  }  
};

//деструктор
CGenPassword::~CGenPassword(){
  delete [] arr; //освобождаем память
}

//создать следующий пароль
bool CGenPassword::Next(){
  if ( not Inc(lenPassword-1) ){
    return false;
  }
  CreatePassword();  
  return true;  
}

 void CGenPassword::CreatePassword(){
   password="";
   password.append(arr, lenPassword);
 }

//увеличить символ
bool CGenPassword::Inc(int index){

  if ( arr[index] == maxChar  ){
     // если максимальный номер символа в текущей ячейке
     // то в текущей ячейке начать с начала     
     arr[index] = minChar;
     
     if (index ==0){
       //более старшего разряда нет, больше увеличивать нельзя
       return false;
     } 
     else{
       //а в предыдущей ячейке(в старшем разряде) увеличить символ
       return Inc(index-1);
     }     
  }
  // если до сих пор функция не завершина,
  //  то можно просто увеличить символ
  arr[index]++;
  return true;

}


class CGenPasswordOnMask:public CGenPassword{
public:
  CGenPasswordOnMask(string mask);
  virtual void CreatePassword();
protected:
  vector<std::string::size_type> posStar;

};

 CGenPasswordOnMask::CGenPasswordOnMask(string mask){
   int lenMask=mask.length();
   int countStar=0;
   for(int i = 0; i < lenMask; i++){
        if(mask[i] == '*'){
          posStar.push_back(i);
          countStar++;
        }
   }   
   Init(countStar); // создаем и инициализируем массив
   password=mask; 
   CreatePassword();
 }
 
 void CGenPasswordOnMask::CreatePassword(){
   for(int i = 0; i < lenPassword; i++){
     password[posStar[i]]=arr[i];
   }
 }


/*
*
*
*
класс для работы с архиватором 7 zip
*
*
*
*/
class CArhive7zip {
public:
  
  CArhive7zip( string& pArhiveName,string& path7zip_);
  bool Unzip(string& password);
  bool FindPassword(CGenPassword& genPassword);
private:
  string path7zip;
  string arhiveName;  
  string textout ; 
  string sFind  ;
};

// конструктор с параметром
CArhive7zip::CArhive7zip( string& pArhiveName,string& path7zip_){
    //path7zip = "\"C:\\Program Files\\7-Zip\\7z.exe\"";
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


int  main(int argc, char* argv[])
{   CGetOptions * options;
    try{
      options=new CGetOptions(argc,  argv); 
    }
    catch(int){
      cin.get();  
	  return 0;
    }
    CArhive7zip arhive7zip(options->arhiveName,options->path7zip);
    CGenPassword * genPassword;
    if (options->lengthPassword >0){
       genPassword=new CGenPassword(options->lengthPassword);
    }
    else {
       genPassword=new CGenPasswordOnMask(options->mask);
    }   
    time_t tStart, tEnd;
    time(&tStart); // получаем время начала работы программы

    if (not arhive7zip.FindPassword(*genPassword)){
       cout<< "password not found "<< endl;
    };
    time(&tEnd);  // получаем время конца работы программы
    double seconds = difftime(tEnd, tStart);
    cout<< "elapsed time: "<< seconds << " sec"<<endl;
    cin.get();  
	return 0;
}