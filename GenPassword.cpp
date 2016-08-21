#include "GenPassword.h"
#include <sstream> // для stringstream
#include <cmath> // для pow
#include <cstdlib> //Для atoi
#include "GetOptions.h"

CAbstractGenPassword::CAbstractGenPassword(){
     codeError=ERROR_NONE;
}

int CAbstractGenPassword::LastError(){
  return codeError;
};

//конструктор
CGenPassword::CGenPassword(){
  minLen=0;
  maxLen=0;
  stepOnLen=0; 
}

CGenPassword::CGenPassword(string& lenRange, string& range){
  ParseRangeLength (lenRange); 
  Init(minLen,range);
  numbPassword=0;
  CreatePassword(); //создать начальный пароль  
}


void CGenPassword::ReInit(int len){ 
  lenPassword=len;  
  delete[] arr ;
  arr = new char [lenPassword]; //выделяем память под массив
  for(int i=0; i < lenPassword; i++){
    arr[i]=minValue;
  }  
};

//проинициализировать переменные
// ::эта функция пригодится наследникам
void CGenPassword::Init(int len, string& range){ 
  ParseRangeChar(range);
  lenPassword=len; 
  if (len==0) {
       msgErr="little length password";
       codeError=ERROR_LITTLE_LENGTH_PASSWORD;
       return ;
       //throw THROW_LITTLE_LENGTH_PASSWORD;
  }
  arr = new char [lenPassword]; //выделяем память под массив
  for(int i=0; i < lenPassword; i++){
    arr[i]=minValue;
  }  
};
    
void CGenPassword::AppendRange(char firstChar,char lastChar){  
    char step = (firstChar<lastChar) ? 1 : -1;
    char ch=firstChar-step;
    do {
       ch+=step;
       charRange.push_back(ch);       
    }  while(ch-lastChar !=0);
}


int CGenPassword::StringToInt(string  s){
  int length=s.length();

  for (int i=0; i<length; i++){
    if (not isdigit(s[i]))
    {
       msgErr="no digital value by length password";
       codeError=ERROR_OPTION_NO_DIGITAL_ARGUMENT;
       return 0;
       //throw THROW_OPTION_NO_DIGITAL_ARGUMENT ;
    }
  }
  return atoi(s.c_str());
}

void CGenPassword::ParseRangeLength(string& range){
  minLen=0;
  maxLen=0;
  stepOnLen=0;
  std::size_t pos= range.find("-"); 
  if (pos == string::npos ){
    minLen= StringToInt(range);
    if(LastError() != ERROR_NONE) return;
    maxLen= minLen;
  }
  else{
    if (pos == range.size()){
       msgErr="no correct value range length password";
       codeError=ERROR_RANGE_LENGTH_NOT_VALID;
       return ;
       //throw THROW_RANGE_LENGTH_NOT_VALID ;
    }
    else{
        minLen= StringToInt(range.substr(0,pos));
        if(LastError() != ERROR_NONE) return;
        
        maxLen= StringToInt(range.substr(pos+1));
        if(LastError() != ERROR_NONE) return;
        
        stepOnLen=(maxLen-minLen > 0 ) ? 1: -1;        
    }     
  }  
  
  if (( minLen==0) && ( maxLen==0)) {
       msgErr="little length password";
       codeError=ERROR_LITTLE_LENGTH_PASSWORD;
       return ;
       //throw THROW_LITTLE_LENGTH_PASSWORD;
  }
  return ;
};
    
void CGenPassword::ParseRangeChar(string& range){
  std::stringstream ss(range);
  string item; 
  char firstChar;
  char lastChar;
  while (std::getline(ss, item, ',')){
    if(item.length()==3){
      if(item[1]=='-'){
          firstChar = item[0]; // с какого символа начать
          lastChar = item[2]; //каким символом закончить   
          AppendRange (firstChar,lastChar);         
      }
    } 
    else if(item.length()==1)  {
       charRange.push_back(item[0]); 
    }
    
  }
  
  if (charRange.size()==0){
     msgErr= "range char not valid";
     codeError=ERROR_RANGE_CHAR_NOT_VALID;
     return ;
     //throw THROW_RANGE_CHAR_NOT_VALID;
  }
  else{
    minValue=0;
    maxValue=charRange.size()-1;
  }
};

//деструктор
CGenPassword::~CGenPassword(){
  delete [] arr; //освобождаем память
}

//создать следующий пароль
bool CGenPassword::Next(){
  if ( not Inc(lenPassword-1) ){
    if (maxLen !=0 && lenPassword != maxLen){      
      lenPassword+=stepOnLen;
      ReInit(lenPassword);
      CreatePassword(); //проверить кажется был пропущен что приводит к двойно проверке последнего пароля
      return true;
    }
    else{
      return false;
    }  
  }
  CreatePassword();  
  return true;  
}

 void CGenPassword::CreatePassword(){
   password="";
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password+=charRange[arr[i]];
   }
   //password.append(arr, lenPassword);
 }
 
 double CGenPassword::CountPasswords(){
   if (maxLen !=0){
       double count=0;
       int i = minLen-stepOnLen;
       do{
           i+=stepOnLen;
           count+=  pow(charRange.size(),i);       
       }while (i != maxLen);
       return  ceil(count);
   }
    return  ceil(pow(charRange.size(),lenPassword)); 
   
 }

//увеличить символ
bool CGenPassword::Inc(int index){

  if ( arr[index] == maxValue  ){
     // если максимальный номер символа в текущей ячейке
     // то в текущей ячейке начать с начала     
     arr[index] = minValue;
     
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

//////////////////////////////////////////////////////////////////
//////////////////////наследник класса GenPassword //////////////
//////////////////////////////////////////////////////////////////

 CGenPasswordOnMask::CGenPasswordOnMask(string mask,string& range){
   int lenMask=mask.length();
   int countStar=0;
   for(int i = 0; i < lenMask; i++){
        if(mask[i] == '*'){
          posStar.push_back(i);
          countStar++;
        }
   }   
   Init(countStar, range); // создаем и инициализируем массив
   password=mask; 
   numbPassword=0;
   CreatePassword();   
 }
 
 void CGenPasswordOnMask::CreatePassword(){
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password[posStar[i]]=charRange[arr[i]];
   }
 }
 
 //////////////////////////////////////////////////////////////////
//////////////////////наследник класса CAbstractGenPassword //////////////
//////////////////////////////////////////////////////////////////
 
 CGenPasswordFromDict::CGenPasswordFromDict(string dicPath_,int translitType_){
   dicPath= dicPath_;
   translitType=translitType_;
   
   trInit();
   CalcCountPassword();  
   
   numbPassword=0;
   queueToTranslit=not DO_TRANSLIT;
   fr.open( dicPath.c_str());//раньше открывать файл нельзя  
   //todo fr.is_open?
   Next(); // todo обработать результат

 }
 
void  CGenPasswordFromDict::CalcCountPassword(){
   countPasswords=0;
   string tmp;
   
   fr.open( dicPath.c_str());
   
   while(!fr.eof()){
     std::getline(fr,tmp);
     if (not tmp.empty()){
        countPasswords++;
     }   
   }  
   fr.close();   
   //fr.seekg (0, fr.beg); //
   if (translitType==TYPE_TRANSLIT_BOTH){
      countPasswords*=2;
   }
   
}


void CGenPasswordFromDict::trInit(){
   for (int i=0 ;i<256;i++ ){
     tr[i]=0;
   }
   tr['z']='я';  tr['Z']='Я';
   tr['x']='ч';  tr['X']='Ч';
   tr['c']='с';  tr['C']='С';
   tr['v']='м';  tr['V']='М';
   tr['b']='и';  tr['B']='И';
   tr['n']='т';  tr['N']='Т';
   tr['m']='ь';  tr['M']='Ь';
   tr[',']='б';  tr['<']='Б';
   tr['.']='ю';  tr['>']='Ю';
   tr['/']='.';  tr['?']=',';
   tr['a']='ф';  tr['A']='Ф';
   tr['s']='ы';  tr['S']='Ы';
   tr['d']='в';  tr['D']='В';
   tr['f']='а';  tr['F']='А';
   tr['g']='п';  tr['G']='П';
   tr['h']='р';  tr['H']='Р';
   tr['j']='о';  tr['J']='О';
   tr['k']='л';  tr['K']='Л';
   tr['l']='д';  tr['L']='Д';
   tr[';']='ж';  tr[':']='Ж';
   tr['\'']='э'; tr['"']='Э';
   tr['q']='й';  tr['Q']='Й';
   tr['w']='ц';  tr['W']='Ц';
   tr['e']='у';  tr['E']='У';
   tr['r']='к';  tr['R']='К';
   tr['t']='е';  tr['T']='Е';
   tr['y']='н';  tr['Y']='Н';
   tr['u']='г';  tr['U']='Г';
   tr['i']='ш';  tr['I']='Ш';
   tr['o']='щ';  tr['O']='Щ';
   tr['p']='з';  tr['P']='З';
   tr['[']='х';  tr['{']='Х';
   tr[']']='ъ';  tr['}']='Ъ';
   tr['`']='ё';  tr['~']='Ё';
                 tr['@']='"'; 
                 tr['#']='№';  
                 tr['$']=';';   
                 tr['^']=':';
                 tr['&']='?'; 
                 tr['|']='/'; 
 
   for (int i=0 ;i<256;i++ ){
     if (tr[i]==0){
        tr[i]=i;
        //сами на себя
     }
     else { 
        tr[ tr[i]]=i;
        //обратная замена
     }
     
   }  
}

void CGenPasswordFromDict::TranslitPassword(){
   int lenPassword=password.size();
   for(int i=0 ; i<lenPassword; i++){
      password[i]= tr[static_cast<unsigned char> (password[i])] ; 
   } 
 }
//#include <iostream> 
bool  CGenPasswordFromDict::ReadNextPassword(){
  if (!fr.eof()){
         std::getline(fr,password);
         //if (fr.tellg()==-1) return false;;
         if (password.empty()){
           return ReadNextPassword();
         }
         numbPassword++;
         return true;
  } 
  return false;
 }
 
 bool  CGenPasswordFromDict::Next(){
   if (translitType==TYPE_TRANSLIT_BOTH ){ 
           bool result =true;   
           if (queueToTranslit==DO_TRANSLIT){                 
                 TranslitPassword();
                 numbPassword++;
           }      
           else{
                result = ReadNextPassword();
           }                 
           queueToTranslit= not queueToTranslit;
           //осуществляем чередование
           return result;
   }
   else if (translitType==TYPE_TRANSLIT_ONLY ){
      bool result = ReadNextPassword();
      TranslitPassword();          
      return result;      
   }
   
   else if (translitType==TYPE_TRANSLIT_NONE){
     return ReadNextPassword();
   }
 
   return false;
 }

 double CGenPasswordFromDict::CountPasswords(){

   return countPasswords;

 }
 
  CGenPasswordFromDict::~CGenPasswordFromDict(){
    fr.close(); 
  }