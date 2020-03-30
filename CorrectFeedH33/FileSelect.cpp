//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FileSelect.h"
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFileSelectDialog *FileSelectDialog;
//---------------------------------------------------------------------------
__fastcall TFileSelectDialog::TFileSelectDialog(TComponent* Owner)
        : TForm(Owner)
{
        FileSelectDialog->Caption = "123";
}
//---------------------------------------------------------------------------
void __fastcall TFileSelectDialog::BrowseClick(TObject *Sender)
{
  TSaveDialog* ASaveDialog;
  bool ResultAccepted;
  AnsiString ResultFileName;

  ASaveDialog = new TSaveDialog(NULL);
  try
  {
    ASaveDialog->Title = "Задайте путь и имя файла";
    ASaveDialog->DefaultExt = "up";
    ASaveDialog->Filter = "All files (*.*)|*.*";

    ASaveDialog->Options = TOpenOptions() << ofPathMustExist << ofFileMustExist
                                          << ofHideReadOnly << ofEnableSizing;

    ResultAccepted = ASaveDialog->Execute();
    if (ResultAccepted)
    {
      ResultFileName = ExpandFileName(ASaveDialog->FileName);
    }
  }
  __finally
  {
    delete ASaveDialog;
  }

  FileName->Text = ResultFileName;

}
//---------------------------------------------------------------------------

void __fastcall TFileSelectDialog::OKButtonClick(TObject *Sender)
{
    // Если не введено имя файла
    if(strcmp(FileName->Text.c_str(),"")==0)
    {
        ShowMessage("Не задано имя файла");
        return;
    }

    // Если такой файл уже существует, то спросить, переписывать ли его
    if(access(FileName->Text.c_str(),0)==0)
      if(MessageDlg("Файл с таким именем уже существует\nПереписать его?",
         mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo,0)==mrNo)
                ShowMessage("Введите другое имя");
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------



