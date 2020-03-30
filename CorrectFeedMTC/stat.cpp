//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "stat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
   //Form2->ModalResult = mrCancel;
   Form2->Visible=False;
}
//---------------------------------------------------------------------------
