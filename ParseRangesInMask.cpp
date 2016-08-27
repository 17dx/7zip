
#include "ParseRangesInMask.h"



CParseRangesInMask::CParseRangesInMask(
                     string& mask,
                     vector <string>* ranges_,
                     vector <std::string::size_type>* posPointInsert_){
  ext_mask=mask;   
  ranges=ranges_;
  posPointInsert=posPointInsert_;
  codeError=ERROR_NONE;
}

int CParseRangesInMask::LastError(){
  return codeError;
};

bool CParseRangesInMask::RepeatRange(string & range){
          string value;
          bool res=GetSubRange(value,'{','}');
          if (LastError()!=ERROR_NONE){ return false;}
          if (res){
              int countRepeat=StringToInt(value);
              countRepeat--;
              if (LastError()!=ERROR_NONE){ return false;} 
              for (int j=0;j<countRepeat;j++){
                  AppendSubRange(range);                
              }
              return true;
           }  
           return false;
}

bool CParseRangesInMask::Parse( ){  
    string subrange; 
    bool last_char_is_end_range=false; 
    existEscapeCharacter=false;    
    lenMask=ext_mask.length();
    bool res;
    for(pos = 0; pos < lenMask; pos++){ 
        if( last_char_is_end_range )  {
            if (RepeatRange(subrange)){
              continue;
            }
        }
        
        res=GetSubRange(subrange,'[',']');
        if (LastError()!=ERROR_NONE){ return false;}        
        
        if( res){              
              AppendSubRange(subrange);
              last_char_is_end_range=true;
        }
        else{
            AppendCharToNewMask(ext_mask[pos]);
        }
        existEscapeCharacter = (ext_mask[pos]=='\\');
   }   
 }
 
void CParseRangesInMask::AppendStarToNewMask(){
    newMask+='*';
} 
 
void CParseRangesInMask::AppendCharToNewMask(char fromMask){
  if (fromMask== '\\' && not (existEscapeCharacter)){
     return;
   } 
   newMask+=fromMask;  
}

bool CParseRangesInMask::GetSubRange( string& subrange,
                                      char markStart, 
                                      char markEnd){   
    subrange="";
    int lenMask=ext_mask.length();
    bool markEsist=(ext_mask[pos] == markStart);
    if(not existEscapeCharacter && markEsist){
              pos++;
              size_t start_range=pos;
              while ( not ((ext_mask[pos] == markEnd) 
                       || (pos == lenMask) )){
                pos++;
              } ;
              if (ext_mask[pos] != markEnd){
                     msgErr="extended mask not valid";
                     codeError=ERROR_EXT_MASK_NOT_VALID;
                     return false;
              }
              subrange=ext_mask.substr (start_range,pos-start_range); 
              return true;               
    }
       
    return false;
 }
 
 bool CParseRangesInMask::AppendSubRange(string & range){
   ranges->push_back( range );    
   posPointInsert->push_back(newMask.size());//.size() =eq= position next char
   AppendStarToNewMask(); 
 }

 int CParseRangesInMask::StringToInt(string & s){
//избавиться от копии
  int length=s.length();

  for (int i=0; i<length; i++){
    if (not isdigit(s[i]))
    {
       msgErr="no digital value";
       codeError=ERROR_OPTION_NO_DIGITAL_ARGUMENT;
       return 0;
    }
  }
  return atoi(s.c_str());
}