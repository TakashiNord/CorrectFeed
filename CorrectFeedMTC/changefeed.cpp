//---------------------------------------------------------------------------
#include <dir.h>
#include <math.h>

#include <vcl.h>
#pragma hdrstop

#include "algobase.h"

#define bStr 512

#include "changefeed.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button3Click(TObject *Sender)
{
 ModalResult = mrCancel;
 //Form4->Visible=False;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
 ModalResult = mrCancel;
 //Form4->Visible=False;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FeedmaxChange(TObject *Sender)
{
        double tmp;
        tmp= fabs(atof(Feedmax->Text.c_str()) );
        if (tmp<0.0) tmp=5000;
        if (tmp>=999999) tmp=5000;
        Feedmax->Text=tmp;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FeedminChange(TObject *Sender)
{
        double tmp;
        tmp= fabs(atof(Feedmin->Text.c_str()) );
        if (tmp<0.0) tmp=0;
        if (tmp>=999999) tmp=0;
        Feedmin->Text=tmp;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormActivate(TObject *Sender)
{
    Feedmax->Text=5000;
    Feedmin->Text=0;
    Procent->Text=0;
    RadioChange->ItemIndex=-1;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button2Click(TObject *Sender)
{
 long int i;
 int methods, err;
 double minfeed, maxfeed, coef;
 char info[bStr];
 char szFileName[MAXFILE+4];
 AnsiString fn , ext ,fns;

 /*Начальные условия*/

 fn=Form4->FileName->Text;
 /*ShowMessage(fn);*/
 if (fn=="") {
   ShowMessage("Выберите файл для осуществления преобразования");
   return ;
 }

 methods=RadioChange->ItemIndex;
 if (methods==-1) {
   MessageDlg("Выберите метод преобразования .....", mtWarning, TMsgDlgButtons() << mbYes ,0);
   return ;
 }

 maxfeed= fabs(atof(Feedmax->Text.c_str()) );
 if (maxfeed<0.0) maxfeed=5000;
 if (maxfeed>=999999) maxfeed=5000;
 Feedmax->Text=maxfeed;

 minfeed= fabs(atof(Feedmin->Text.c_str()) );
 if (minfeed<0.0) minfeed=0;
 if (minfeed>=999999) minfeed=0;
 Feedmin->Text=minfeed;

 coef= fabs(atof(Procent->Text.c_str()) );

/*
 info[0]='\0'; sprintf(info," index=%ld \n min= %lf \n max= %lf \n coef= %lf ",methods,minfeed, maxfeed,coef );
 MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
 return ;
*/ 

 try
 {
    if (FileExists( fn.c_str() ))
    {

      /* 0 */
      /* Проверка на правильный формат УП */
      err=program_format_H33(fn.c_str(),0);
      switch(err) {
       case -1 :
            info[0]='\0';
            sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",fn.c_str() );
            Application->MessageBox(info,"Проверка программы на формат H33 - > ошибка ...", MB_OK);
            break ;
       case 0 :
            info[0]='\0';
            sprintf(info,"Файл УП ->\n %s \n\n имеет формат УЧПУ H33 (Ataka)\n Корректировка прервана....",fn.c_str() );
            MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
            return ;
       default : break ;
       }

      /* 1 */
      strcpy( szFileName,fn.c_str() ); // set the file name
      strcat( szFileName, ".bak" ); // save to backup
      /* 2
      ext = ExtractFileExt(fn);
      ext=ext+".bak" ;
      fns=ChangeFileExt(fn, ext) ;
      RenameFile(fn.c_str(), fns.c_str());
      //ShowMessage(fns);
      */
      if (FileExists(szFileName)) {
        info[0]='\0';
        sprintf(info,"Копия файла УП с расширением '*.bak' ->\n %s \n %s \n уже существует\n Корректировка прервана....",fn.c_str(), szFileName );
        MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
        return ;
      }
      i=RenameFile(fn.c_str(), szFileName);
      /*ShowMessage(szFileName);*/
      /* */
      switch (methods) {
       case 0 : /*- удаление всех F:F>maxfeed*/
                i=program_correct_feed(szFileName,fn.c_str(),2, 0.0, maxfeed, 1.0);
       break;
       case 1 : /*- удаление всех F:F<minfeed*/
                i=program_correct_feed(szFileName,fn.c_str(),3, minfeed, 12000.0, 1.0);
       break;
       case 2 : /*- удаление всех F*/
                i=program_correct_feed(szFileName,fn.c_str(),1, 0.0, 5000.0, 1.0);
       break;
       case 3 : /*- преобразование всех F на коэфф.*/
                i=program_correct_feed(szFileName,fn.c_str(),5, 0.0, 5000.0, coef);
       break;
       default : i=0;  break;
      }
     /* */
      switch (i) {
       case -1 :
            info[0]='\0';
            sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",szFileName );
            Application->MessageBox(info,"Корректировка подачи - > ошибка ...", MB_OK);
            break ;
       case -2 :
            info[0]='\0';
            sprintf(info,"Невозможно открыть файл:\n %s \n на 'запись' \n",fn.c_str() );
            Application->MessageBox(info,"Корректировка подачи - > ошибка ...", MB_OK);
            break ;
       case -3 :
            Application->MessageBox("Неправильные параметры функции....","Корректировка подачи - > ошибка ...", MB_OK);
            break ;
       default :
       break;
      }


      if (i>=0) {
        info[0]='\0';
        sprintf(info," Преобразование файла УП :\n %s \n\t\t - ЗАВЕРШЕНО\n Общее Число скорректированных адресов N=%ld ", fn.c_str(), i );
        MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
      }


    } else {
       info[0]='\0';
       sprintf(info,"Проверьте наличие Файла УП ->\n %s \n Корректировка прервана....",fn.c_str() );
       MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
    }

  }
  __finally
  {

  }

}
//---------------------------------------------------------------------------

