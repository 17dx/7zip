#include "FindPassword.h"

#include <iostream> //для  cout, cin
#include <iomanip>      // std::setfill, std::setw
#include <fstream> //для ifstream
#include <stdlib.h> //для system
#include <codecvt> //для wstring_convert

const int indexFileName=0;
const int indexPassword=1;

using std::cout;
using std::endl;

using std::setfill;
using std::setw;

const int LEN_BUFFER=256;

OLECHAR * to_OLECHAR(const char* inp, int len_buffer, OLECHAR * pOut)
{
    MultiByteToWideChar(CP_ACP, 0, inp, -1, pOut, len_buffer);
	/*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(text);*/
	return pOut;

}

CFindPassword::CFindPassword(bool verbose_){
    verbose = verbose_;
    findOK=false;
    workFinished=false;
    InitializeCriticalSection(&printStdOut);
}

 CFindPassword::~CFindPassword(){
}

bool CFindPassword::FindOK(){
    return findOK;
}

bool CFindPassword::DoFind(CAbstractGenPassword& genPassword){

  do{

     if (verbose){
       EnterCriticalSection(&printStdOut) ;
         cout<< genPassword.password<< endl;
       LeaveCriticalSection(&printStdOut) ;
     }

     if ( PasswordIsTrue(genPassword.passwordInfo) ){
        truePassword= genPassword.password  ;
        findOK=true;
        workFinished =true;
        return true;
     };
     if (workFinished){
         ExitThread(0);
     }
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
               LOGON32_LOGON_INTERACTIVE,
               LOGON32_PROVIDER_DEFAULT,
               &hToken) !=0);
  CloseHandle(hToken);
  return result;
}

CMSWord::CMSWord(string& pDocName,bool verbose_):CFindPassword( verbose_)
{
    docName=pDocName.c_str();
    IsCreateWord=false;
    ConnectToWord();
    InitObjWord_ForInvokeOpen();
}

void CMSWord::ConnectToWord(){

    //CoInitialize(NULL); //CoUninitialize
    CoInitializeEx(NULL,COINIT_MULTITHREADED);
    CLSID clsid;
    HRESULT hr = CLSIDFromProgID(L"Word.Application", &clsid);
    if(FAILED(hr)) {
      cout << "MS Word not found" << endl;
      throw ex_error_Word_not_found();
    }

    hr = CoCreateInstance (clsid,
                    NULL,
                    CLSCTX_LOCAL_SERVER,
                    IID_IDispatch,
                    (void **) &pWordApp);
    if(FAILED(hr)) {
        cout << "error run MS Word. Error: " <<hr<< endl;
        throw ex_error_Word_not_running();
    }
    else{
        IsCreateWord=true;
    }


 }

void CMSWord::InitObjWord_ForInvokeOpen(){
    IDispatch * pApp = PropertyGet(pWordApp,L"Application");
    pDocuments = PropertyGet(pApp,L"Documents");
    pApp->Release();

    LPOLESTR rgszNames[3];
    rgszNames[0] = L"Open"; //метод
    rgszNames[indexFileName+1] = L"FileName"; //далее именованные аргументы
    rgszNames[indexPassword+1] = L"PasswordDocument";
    //todo удалить hr после отлидки
    HRESULT hr =pDocuments->GetIDsOfNames(IID_NULL,
                            rgszNames,// массив указателей на запрашиваемые методы объекта
                            3,        // количество элементов массива
                            LOCALE_USER_DEFAULT,
                            invokeOpenInfo.dispid //массив указателей на запрашиваемые методы объекта,
                            // каждый элемент которого ( ID ) соответствует элементу массива rgszNames
                            );
    if(FAILED(hr)) {
        cout << "error get Word property in InitObjWord_ForInvokeOpen"  << endl;
        throw ex_error_Word_not_get_propperty();
    }

    invokeOpenInfo.dp = {NULL,NULL,0,0};
    invokeOpenInfo.dp.cArgs = 2;
    invokeOpenInfo.dp.cNamedArgs = 2; // Число аргументов
    invokeOpenInfo.dp.rgdispidNamedArgs = &invokeOpenInfo.dispid[1];// DISPID для именованных аргументов

    //rgvarg это Массив аргументов
    invokeOpenInfo.dp.rgvarg = new VARIANT[invokeOpenInfo.dp.cArgs];
    invokeOpenInfo.dp.rgvarg[indexFileName].vt = VT_BSTR;
    OLECHAR * pOut=new OLECHAR [LEN_BUFFER];
    invokeOpenInfo.dp.rgvarg[indexFileName].bstrVal = SysAllocString( to_OLECHAR(docName,LEN_BUFFER,pOut));
    delete [] pOut;
    invokeOpenInfo.dp.rgvarg[indexPassword].vt = VT_BSTR;
    //инициализация invokeOpenInfo.dp.rgvarg[indexPassword].bstrVal будет в PasswordIsTrue

   //todo      delete [] dp.rgvarg;

}

void CMSWord::Word_Quit(){

    IDispatch * pApp = PropertyGet(pWordApp,L"Application");
    DISPID dispid;
    LPOLESTR rgszNames= L"Quit"; //метод
    HRESULT hr =pApp->GetIDsOfNames(IID_NULL,
                            &rgszNames,// массив указателей на запрашиваемые методы объекта
                            1,        // количество элементов массива
                            LOCALE_USER_DEFAULT,
                            &dispid //массив указателей на запрашиваемые методы объекта,
                            // каждый элемент которого ( ID ) соответствует элементу массива rgszNames
                            );
    if(FAILED(hr)) {
        cout << "error get Word property Quit"  << endl;
        throw ex_error_Word_not_property_Quit();
    }

    DISPPARAMS dp = {NULL,NULL,0,0};
    hr = pApp->Invoke(dispid,
                      IID_NULL,
                      LOCALE_SYSTEM_DEFAULT,
                      DISPATCH_METHOD,
                      &dp,
                      NULL,
                      NULL,
                      NULL);

    pApp->Release();

}

bool CMSWord::PasswordIsTrue(SPasswordInfo& passwordInfo){
        OLECHAR * pOut=new OLECHAR [LEN_BUFFER];
        invokeOpenInfo.dp.rgvarg[indexPassword].bstrVal =
                  SysAllocString( to_OLECHAR(passwordInfo.password,LEN_BUFFER,pOut) );
        delete [] pOut;
        VARIANT varResult;
        VariantInit(&varResult);
        ZeroMemory(&invokeOpenInfo.excepInfo, sizeof(invokeOpenInfo.excepInfo));

        HRESULT hr=pDocuments->Invoke(invokeOpenInfo.dispid[0],
                                 IID_NULL,
                                 LOCALE_SYSTEM_DEFAULT,
                                 DISPATCH_METHOD,
                                 &invokeOpenInfo.dp,
                                 &varResult,
                                 &invokeOpenInfo.excepInfo,
                                 &invokeOpenInfo.uArgErr);

       SysFreeString(invokeOpenInfo.dp.rgvarg[indexPassword].bstrVal);
       if(FAILED(hr)) {
            return false;
        }
       return true;
}


 /*IDispatch * CMSWord::GetObject(IDispatch * Obj, std::wstring  pathToObj) {
	 std::wstringstream ss(pathToObj);
	 std::wstring item;

	 IDispatch * dispObj = Obj;
	 while (std::getline(ss, item, L'.')) {
		 dispObj = PropertyGet(dispObj, &item[0u]);
	 }
	 return dispObj;
 }*/

 IDispatch * CMSWord::PropertyGet(IDispatch * dispObj,LPOLESTR nameProperty){
    DISPID dispid;
    HRESULT hr=dispObj->GetIDsOfNames(IID_NULL,&nameProperty,1,LOCALE_SYSTEM_DEFAULT,&dispid);
    if(FAILED(hr)) {
        cout << "error get Word property: " << nameProperty << endl;
        throw ex_error_Word_not_get_propperty();
    }
    VARIANT varResult;
    DISPPARAMS dp = {NULL,NULL,0,0};
    hr = dispObj->Invoke(dispid,
                         IID_NULL,
                         LOCALE_SYSTEM_DEFAULT,
                         DISPATCH_PROPERTYGET,
                         &dp,
                         &varResult,
                         NULL,
                         NULL);
    return varResult.pdispVal;
 }

CMSWord::~CMSWord(){
   if (IsCreateWord) {
       Word_Quit();
       pWordApp->Release();
       pDocuments->Release();
   }

}

