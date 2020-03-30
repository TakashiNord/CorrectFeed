//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "showfiles.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm6::Button1Click(TObject *Sender)
{
      long int i ,j;
      j=StringGridFiles->RowCount;
      for (i=0;i<j;i++)
      {
           StringGridFiles->Cells[0][i]="";
           StringGridFiles->Cells[1][i]="";
      }
      Num->Text="0";
      Form6->Close();

}
//---------------------------------------------------------------------------

