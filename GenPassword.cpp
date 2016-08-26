#include "GenPassword.h"
#include "ParseRangesInMask.h"
#include <algorithm> // для find

#include "GetOptions.h"

CAbstractGenPassword::CAbstractGenPassword(){
  upperBoundaryCountPassword=-1;    
}


void CAbstractGenPassword::SetNewUpperBoundaryAsMax(TCount maxCountPassword){
   upperBoundaryCountPassword=maxCountPassword;
}

//конструктор
CGenPassword::CGenPassword(){
  startLen=0;
  endLen=0;
  maxLen=0;
  stepOnLen=0; 
}

CGenPassword::CGenPassword(string& lenRange, string& range){
  ParseRangeLength (lenRange);
  EXIT_IF_ERROR();
  //ошибок небыло значит endLen>0
  Init(startLen,range);
   
  numbPassword=0;
  CreatePassword(); //создать начальный пароль  
}


void CGenPassword::ReInit(int len){ 
  lenPassword=len;  
  for(int i=0; i < lenPassword; i++){
    chars[i]->ValueToFloor();
  }  
};

void CGenPassword::SetNewLowerBoundary(string & start){ 
  EXIT_IF_ERROR();
  if (lenPassword!=start.size()){
     string msg_error= " len_password (";
     msg_error+=to_string(lenPassword)+") != len_start_password ("+to_string(start.size())+")";
     eventError.CreateEventError(msg_error,ERROR_SETTING_VALUE);
     return;
  }  
  for(int i=0; i < lenPassword; i++){
    chars[i]->SetValue(start[i]);
    EXIT_IF_ERROR();
  }  
};

void CGenPassword::SetNewLowerBoundary(TCount num_passw){ 
  EXIT_IF_ERROR();
  
  TCount TotalPasswords= CountPasswords();
  if ( num_passw > TotalPasswords){
     string msg_error= " start_number_password (";
     msg_error+=to_string(num_passw)+") > total_count_password ("+to_string(TotalPasswords)+")";
     eventError.CreateEventError(msg_error,ERROR_SETTING_VALUE);
     return;
  }  
  for(int i=lenPassword; i != 0; i--){
    TIndexInRange value = num_passw % chars[i-1]->GetRangeSize();
    chars[i-1]->SetValueAsIndex(value);
    num_passw/= chars[i-1]->GetRangeSize();
    EXIT_IF_ERROR();
  }  
};

void CGenPassword::ReCreateFirstPassword(){
  numbPassword=0;
  CreatePassword(); 
}

//upper

//проинициализировать переменные
// ::эта функция пригодится наследникам
void CGenPassword::Init(int len,string& range){
  if (maxLen==0 && startLen==0 && endLen==0){ 
       maxLen=len;
       startLen=len;
       endLen=len;
  } //не надежно???
  chars=new CRangeChar* [maxLen] ; // todo запомнить что я его создал а не пришел с наружи
  chars[0]=new CRangeChar(range); 
  EXIT_IF_ERROR();

  for(int i=1; i < maxLen; i++){
     chars[i] = new CRangeChar(*chars[0]) ;
  } 
  Init(len);//startLen!! почему было ,chars
}

// todo объединить
void CGenPassword::Init(int len){ //,CRangeChar** ranges//, string& range
  lenPassword=len; 
  if (maxLen==0 && startLen==0 && endLen==0){ 
       maxLen=len;
       startLen=len;
       endLen=len;
  } //не надежно???
  if (len==0) {
       eventError.CreateEventError("little length password",ERROR_LITTLE_LENGTH_PASSWORD) ;
       return ;

  }

  for(int i=0; i < lenPassword; i++){
    chars[i]->ValueToFloor();
  }  
};
    

void CGenPassword::ParseRangeLength(string& range){
  startLen=0;
  endLen=0;
  stepOnLen=0;
  maxLen=0;
  std::size_t pos= range.find("-"); 
  if (pos == string::npos ){
    startLen= StringToInt(range);
    EXIT_IF_ERROR();
    endLen= startLen;
  }
  else{
    if (pos == range.size()){
       eventError.CreateEventError("no correct value range length password",
                                    ERROR_RANGE_LENGTH_NOT_VALID);
       return ;
    }
    else{
        startLen= StringToInt(range.substr(0,pos));
        EXIT_IF_ERROR();
        
        endLen= StringToInt(range.substr(pos+1));
        EXIT_IF_ERROR();
        
        stepOnLen=(endLen-startLen > 0 ) ? 1: -1;        
    }     
  }  
  
  if (( startLen==0) || ( endLen==0)) {
       eventError.CreateEventError("little length password",
                                   ERROR_LITTLE_LENGTH_PASSWORD);
       return ;
  }
  maxLen=(startLen>endLen)? startLen:endLen;
  return ;
};
    


//деструктор
CGenPassword::~CGenPassword(){
  delete [] chars;//arr; //освобождаем память
}

//создать следующий пароль
bool CGenPassword::Next(){
  if (upperBoundaryCountPassword<numbPassword){
     return false;
  }
  if ( not Inc(lenPassword-1) ){
    if (endLen !=0 && lenPassword != endLen){   
     // endLen !=0 на случай инициализации не через опцию -l    
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
     password+=chars[i]->GetValue();
   }
   passwordInfo.password=password.c_str();
 }
 
 TCount CGenPassword::CountPasswords(){
   //if (endLen !=0){
       TCount count=0;
       int i = startLen-stepOnLen;
       do{
           i+=stepOnLen;
           TCount combination_count=1;
           for(int j=0;j<i;j++){
              combination_count*=chars[j]->GetRangeSize();
           }
           count+=  combination_count;//pow(charRange.size(),i);       
       }while (i != endLen);
       return  ceil(count);
   //}
    //return  ceil(pow(charRange.size(),lenPassword)); 
   
 }

//увеличить символ
bool CGenPassword::Inc(int index){

  if ( not chars[index]->Inc()/*arr[index] == maxValue*/  ){
     // если максимальный номер символа в текущей ячейке
     // то в текущей ячейке начать с начала 
     chars[index]->ValueToFloor();     
     
     if (index ==0){
       //более старшего разряда нет, больше увеличивать нельзя
       return false;
     } 
     else{
       //а в предыдущей ячейке(в старшем разряде) увеличить символ
       return Inc(index-1);
     }     
  }
  return true;
}

//////////////////////////////////////////////////////////////////
//////////////////////наследник класса GenPassword //////////////
//////////////////////////////////////////////////////////////////

 CGenPasswordOnMask::CGenPasswordOnMask(string& mask,string& range){
   int lenMask=mask.length();
   int countPointInsert=0;
   for(int i = 0; i < lenMask; i++){
        if(mask[i] == '*'){
          posPointInsert.push_back(i);
          countPointInsert++;
        }
   }   
   Init(countPointInsert, range); // создаем и инициализируем массив
   password=mask; 
   numbPassword=0;
   CreatePassword();   
 }
 
 void CGenPasswordOnMask::SetNewLowerBoundary(string & start){
      EXIT_IF_ERROR();
      size_t len= password.size();
      if (len != start.size()){
         string msg_error= " len_password (";
         msg_error+=to_string(len)+") != len_start_password ("+to_string(start.size())+")";
         
         eventError.CreateEventError( msg_error,
                                    ERROR_SETTING_VALUE);
         return;
      }  
      size_t j=0;
      for(size_t i=0 ; i < len; i++){
            vector<std::string::size_type>::iterator it=std::find(
                               posPointInsert.begin(), 
                               posPointInsert.end(), 
                               i);
            if (it != posPointInsert.end())  { 
              chars[j]->SetValue(start[i]); 
              EXIT_IF_ERROR();           
              j++;
            }    
            else{           
               if (password[i]!=start[i]) { 
                 string msg_error= "error set start value for password (";
                 msg_error+=to_string(password[i])+" != "+to_string( start[i] )+" )";
                 eventError.CreateEventError( msg_error ,
                                   ERROR_SETTING_VALUE);
                 return ;
              }
            }        
       } 
 }
 
 void CGenPasswordOnMask::CreatePassword(){
   numbPassword++;
   for(int i = 0; i < lenPassword; i++){
     password[posPointInsert[i]]=chars[i]->GetValue();//charRange[arr[i]];
   }
   passwordInfo.password=password.c_str();
 }
 


CGenPasswordOnMask::CGenPasswordOnMask(string& ext_mask){
   int lenMask=ext_mask.length();
   
   vector <string> ranges;
   CParseRangesInMask rangesInMask=CParseRangesInMask(ext_mask,&ranges, &posPointInsert);
   rangesInMask.Parse();
   EXIT_IF_ERROR();

   password=rangesInMask.newMask;
   if (password.empty()){
     eventError.CreateEventError( "little length password, extended mask not valid",
                                   ERROR_EXT_MASK_NOT_VALID);
     return ;
     
   }
   int countPointInsert = ranges.size();
   chars=new CRangeChar* [countPointInsert] ; // todo запомнить что я его создал а не пришел с наружи
   for(int i=0; i < countPointInsert; i++){
     chars[i] = new CRangeChar(ranges[i]) ;
     EXIT_IF_ERROR();
   } 
   Init(countPointInsert);
   numbPassword=0;
   CreatePassword();
}



 
 //////////////////////////////////////////////////////////////////
//////////////////////наследник класса CAbstractGenPassword //////////////
//////////////////////////////////////////////////////////////////
 
 CGenPasswordFromDict::CGenPasswordFromDict(string dicPath_,int translitType_){
   dicPath= dicPath_;
   translitType=translitType_;
   
   trInit();
   CalcCountPassword();  
   if (countPasswords==0){
      eventError.CreateEventError(  "error dictonary empty",
                                   ERROR_DICTONARY_EMPTY);
      return;
   }
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

 TCount CGenPasswordFromDict::CountPasswords(){

   return countPasswords;

 }
 
  CGenPasswordFromDict::~CGenPasswordFromDict(){
    fr.close(); 
  }
  
  void  CGenPasswordFromDict::SetNewLowerBoundary(string & start){
    //todo доделать
  };
  
  void CGenPasswordFromDict::SetNewLowerBoundary(TCount num_passw){ 
     //todo доделать
  }; 
  
  void  CGenPasswordFromDict::ReCreateFirstPassword(){
    //todo доделать
  };  