library wrapper7z;

{$mode objfpc}{$H+}

uses
  Classes,sevenzip,windows;
const SUCCESS_UNZIP=1;
      NOT_SUCCESS_UNZIP=0;
      ERROR_ARHIVE_NOT_OPEN=2;

var Archivery:I7zInArchive ;
    lastArhiveName:string;
    ArhiveNameIsOpen:boolean=false;
function Unzip( pPassword: PChar):integer; stdcall;
begin
  if not ArhiveNameIsOpen then begin
      result:=ERROR_ARHIVE_NOT_OPEN;
      exit;
  end;
  with Archivery do
  begin
    (Archivery as IResultStatus).ClearStatusResult(); //сброс состояния ошибки
    SetPassword((pPassword));
    ExtractTo('test');
    if ((Archivery as IResultStatus).OperationIsSuccess()) then
     result:= SUCCESS_UNZIP
    else result:= NOT_SUCCESS_UNZIP ;
  end;
end;

procedure OpenArhive(pArhiveName: pChar); stdcall;
var ArhiveName:string;
begin
  ArhiveName:=string(pArhiveName );
  if (ArhiveName <> lastArhiveName) then begin
      if lastArhiveName<>'' then begin
         Archivery.Close;
      end;
      Archivery.OpenFile(arhiveName);
      lastArhiveName:=ArhiveName ;
  end;
  ArhiveNameIsOpen:=true;
end;




exports
  Unzip, OpenArhive;
begin
  Archivery:=CreateInArchive(CLSID_CFormatZip);
  //ArhiveNameIsOpen:=false;
end.

