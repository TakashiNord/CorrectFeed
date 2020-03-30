//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LengthLimitDialog.h"
#include "FileSelect.h"
#include "ctype.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLengthLimit *LengthLimit;
//---------------------------------------------------------------------------
__fastcall TLengthLimit::TLengthLimit(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLengthLimit::ButtonCancelClick(TObject *Sender)
{
     //Close();
     ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TLengthLimit::OKButtonClick(TObject *Sender)
{
        if(strcmp(Length->Text.c_str(),"")==0)
        {
                ShowMessage("¬ведите значение");
                return;
        }
        char s[32];
        sprintf(s,"%s",Length->Text.c_str());
        for(int i=0; i<(int)strlen(s); i++)
        {
                if(isdigit(s[i])==0)
                {
                   ShowMessage("¬веден неверный символ");
                   Length->Clear(); return;
                }
        }

        // —читать из пол€ Length значение в переменную LEN_LIMIT
        sscanf( Length->Text.c_str(),"%D",&LEN_LIMIT);
        LEN_LIMIT-=200;
        ModalResult = mrOk;
}
//---------------------------------------------------------------------------

