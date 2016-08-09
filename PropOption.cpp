#include "PropOption.h"

void CPropOptions::registerOption( const char * option,
                      string* valueString,
                      const char * helpMsg,
                      bool* isFind){
  
    SPropOption  propOption;
    propOption.valueString= valueString;
    propOption.helpMsg=     helpMsg;
    propOption.isFind=       isFind;
    if (isFind != NULL){
       *(isFind)=false;  // по умолчанию
    }
    
    items.insert(pair<string,SPropOption> (string(option),propOption));
}

void CPropOptions::GetHelpMessages(string& msg){
    map <string,SPropOption>::iterator the_iterator = items.begin();
    map <string,SPropOption >::iterator the_end = items.end();
    while (the_iterator != the_end) {
         SPropOption& option = the_iterator->second;
         msg = msg + option.helpMsg;
         the_iterator++;
    }

}

void CPropOptions::SetIsFind(string&  option){  
  if (items[option].isFind != NULL){
    *(items[option].isFind)=true;  
  }
}

void CPropOptions::SetArgum(string&  option,string& value){  
  *(items[option].valueString )=value;

}

bool CPropOptions::NeedArgum(string&  option){
  return (items[option].valueString != NULL);
}

bool CPropOptions::OptionIsRegistered(string& option){
  return (items.find(option) != items.end());
}