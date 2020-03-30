//---------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <dir.h>

#include <vcl.h>
#pragma hdrstop

#include "main.h" // Form1 - 'главная форма'

#include "stat.h" // Form2 - 'окно статистики'

#include "algobase.h" //алгоритмы (MTC200)+H33

#include "Htabl.h" // Form3 - 'таблица с режимами'

#include "changefeed.h" // Form4 - 'изменение подачи'

#include "about.h" // Form5 - о программе

#include "showfiles.h" // Form6 - 'вывод 2х файлов для сравнения'

#define bStr 512

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}

//---------------------------------------------------------------------------

void __fastcall TForm1::accChange(TObject *Sender)
{
        long int tmp;
        tmp= abs(atoi(acc->Text.c_str()) );
        if (tmp<=0.0) tmp=10;
        acc->Text=tmp;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonExitClick(TObject *Sender)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N0noteClick(TObject *Sender)
{
// notepad
  int i ;
  char info[bStr];
  AnsiString fn ;

  fn=FileListBox1->FileName ;
  /*ShowMessage(fn);*/

  info[0]='\0';
  strcpy(info,"notepad "); // strcpy(info,"uedit32.exe \"");
  strcat(info, fn.c_str() );
  try
  {
    i=WinExec(info,1);
    if (i < 32) {
      	info[0]='\0';
        sprintf(info, "WinExec failed; error code = %d", i);
        Application->MessageBox(info,"Редактировать УП - > ошибка ...", MB_OK);
     }
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1statClick(TObject *Sender)
{
  int i,err ; char info[bStr];
  AnsiString fn ;

  fn=FileListBox1->FileName ;
  /*ShowMessage(fn);*/

  if (fn=="") {
   ShowMessage("Укажите файл для расчета статистики УП");
   return ;
  }

  struct INFO_FILE_STAT infofile ;
  err=program_statistics(fn.c_str(),100000.0,&infofile);

  if (err==-1) {
    info[0]='\0';
    sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",fn.c_str() );
    Application->MessageBox(info,"Статистика УП - > ошибка ...", MB_OK);
    return ;
  }

  Form2->Type->Caption="MTC200 (INDRAMAT)";
  Form2->Type->Font->Color=clBlue;
  if (infofile.fl_n33 != 0) {
     Form2->Type->Caption="H33 (ATAKA)";
     Form2->Type->Font->Color=clRed;
   }
  //FloatToStr(TIME); IntToStr(numbers_of_kadrs); FormatFloat("0.00",TIME);
  //Form2->FileInfo->Caption=info;
  Form2->FileInfo->Lines->Clear();    Form2->FileInfo->Lines->SetText(infofile.info);
  Form2->Time->Text=FormatFloat("0.00",infofile.TIME);
  Form2->Dist->Text=FormatFloat("0.00",infofile.DIST);
  Form2->NumKadrs->Text=FormatFloat("0",infofile.numbers_of_kadrs);
  Form2->Minfeed->Text=FormatFloat("0.00",infofile.min_feed);
  Form2->Maxfeed->Text=FormatFloat("0.00",infofile.max_feed);
  Form2->NumLine->Text=FormatFloat("0",infofile.numbers_of_kadrs_line);
  Form2->NumCircle->Text=FormatFloat("0",infofile.numbers_of_kadrs_circle);

  Form2->StringGrid1->Cells[0][0]=" Габариты ";
  Form2->StringGrid1->Cells[1][0]=" X ";
  Form2->StringGrid1->Cells[2][0]=" Y ";
  Form2->StringGrid1->Cells[3][0]=" Z ";
  Form2->StringGrid1->Cells[0][1]=" min ";
  Form2->StringGrid1->Cells[0][2]=" max ";
  Form2->StringGrid1->Cells[0][3]=" Сумма ";
  for (i = 1; i< Form2->StringGrid1->ColCount; i++)
     Form2->StringGrid1->Cells[i][1] = FormatFloat("0.00",infofile.min_points[i-1]);
  for (i = 1; i< Form2->StringGrid1->ColCount; i++)
     Form2->StringGrid1->Cells[i][2] = FormatFloat("0.00",infofile.max_points[i-1]);
  for (i = 1; i< Form2->StringGrid1->ColCount; i++)
     Form2->StringGrid1->Cells[i][3] = FormatFloat("0.00",infofile.SUMXYZ[i-1]);

  // Form2->ModalResult = mrOk;// Form2->ShowModal();
  Form2->Visible=True;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2proverkaClick(TObject *Sender)
{
  short int err ;
  char info[bStr];
  AnsiString fn ;

  fn=FileListBox1->FileName ;
  /*ShowMessage(fn);*/
  if (fn=="") return ;

  err=program_format_H33(fn.c_str(),0);

  switch(err) {
   case -1 :
            info[0]='\0';
            sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",fn.c_str() );
            Application->MessageBox(info,"Проверка программы на формат H33 - > ошибка ...", MB_OK);
            break ;
   case 0 :
            info[0]='\0';
            sprintf(info,"Файл УП ->\n %s \n\n имеет формат УЧПУ H33 (Ataka)",fn.c_str() );
            MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
            break ;
   default :
            info[0]='\0';
            sprintf(info,"Файл УП ->\n %s \n\n НЕ имеет формат УЧПУ H33 (Ataka)",fn.c_str() );
            MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
            break ;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    int i;
    i=Form1->FileListBox1->ItemIndex;
    Form1->DirectoryListBox1->Update();
    Form1->FileListBox1->Update();
    Form1->FileListBox1->ItemIndex=i;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3newClick(TObject *Sender)
{
    int i;
    i=Form1->FileListBox1->ItemIndex;
    Form1->FileListBox1->Update();
    Form1->FileListBox1->ItemIndex=i;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4deleteClick(TObject *Sender)
{
    int i; char info[bStr];
    AnsiString fn ;

    fn=FileListBox1->FileName ;
    /*ShowMessage(fn);*/
    if (fn=="") return ;

    info[0]='\0';
    sprintf(info,"Удалить Файл ->\n %s \n\t\t ? ....\n Внимание ! Файл будет удален безвозвратно ! ",fn.c_str() );
    i=MessageDlg(info, mtError , TMsgDlgButtons() << mbNo << mbYes ,0);
    if (i==mrYes) {
     if (FileExists(fn.c_str() )) { DeleteFile(fn.c_str()); }
     i=Form1->FileListBox1->ItemIndex;
     Form1->FileListBox1->Update();
     Form1->FileListBox1->ItemIndex=i;
    }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtonTimeCalcClick(TObject *Sender)
{
 short int err ;
 int i ;
 char info[bStr];
 double TIME ;
 AnsiString fn ;

 fn=FileListBox1->FileName ;
 /*ShowMessage(fn);*/
 if (fn=="") {
   ShowMessage("Выберите файл для осуществления подсчета времени");
   return ;
 }

 try
 {
    if (FileExists( fn.c_str() ))
    {
     /**/
     err=program_format_H33(fn.c_str(),0);
     /**/
     if (err==0) {
        info[0]='\0';
        sprintf(info,"Файл УП ->\n %s \n\n имеет формат УЧПУ H33 (Ataka)",fn.c_str() );
        MessageDlg(info, mtWarning , TMsgDlgButtons() << mbYes ,0);
        /* Время выполнения*/
        TIME=program_time_H33( fn.c_str() );
        info[0]='\0';
        sprintf(info,"Файл УП ->\n %s \n Время выполнения УП = %.2lf (мин)",fn.c_str(),TIME );
        MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);

      } else {

         if (err==-1) {
               info[0]='\0';
               sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",fn.c_str() );
               Application->MessageBox(info,"Проверка программы на формат H33 - > ошибка ...", MB_OK);
          } else {
               /**/
               TIME=program_time(fn.c_str(),100000.0);
               /**/
               if (TIME<0.0) {
                  info[0]='\0';
                  sprintf(info,"Файл УП ->\n %s \n\n Невозможно вычислить время выполнения УП!\n Странный формат файла",fn.c_str() );
                  MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
                 } else {
                  info[0]='\0';
                  sprintf(info,"Файл УП ->\n %s \n Время выполнения УП = %.2lf (мин)",fn.c_str(),TIME );
                  MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
                }

           } // (err==-1)

         }

    }

  }
  __finally
  {
    i=Form1->FileListBox1->ItemIndex;
    Form1->FileListBox1->Update();
    Form1->FileListBox1->ItemIndex=i;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtnHelpClick(TObject *Sender)
{
  // Form3->ModalResult = mrOk;// Form3->ShowModal();
  Form3->Visible=True;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonChangeFeedClick(TObject *Sender)
{
 AnsiString fn ;
 fn=FileListBox1->FileName ;
 /*ShowMessage(fn);*/

 if (fn=="") {
   ShowMessage("Выберите файл для осуществления преобразования");
   return ;
 }

 Form4->FileName->Text=fn;
 //Form4->Visible=True;
 Form4->ShowModal();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1Click(TObject *Sender)
{
   Form5->ShowModal();
//   Form5->Visible=True;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtnCorrectMtc200Click(TObject *Sender)
{

 long int i; int err;   struct INFO_FILE_STAT infofile ;
 char info[bStr];
 char szFileName[MAXFILE+4];
 AnsiString fn , ext ,fns;

 FILE *rf,*ff;
 char curr1[256],curr2[256];

 if (0==ParamCount()) {
   ; //ShowMessage(System::CmdLine); //LPTSTR GetCommandLine()
 }

 fn=FileListBox1->FileName ;
 /*ShowMessage(fn);*/
 if (fn=="") {
   ShowMessage("Выберите файл для осуществления преобразования");
   return ;
 }

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
        sprintf(info,"Копия файла УП:\n %s \n с расширением '*.bak' -> %s \n\t\t УЖЕ СУЩЕСТВУЕТ\n\n Корректировка прервана....",fn.c_str(), szFileName );
        MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
        return ;
      }
      i=RenameFile(fn.c_str(), szFileName);
      /*ShowMessage(szFileName);*/

      /*  */
      err=program_correct(szFileName,fn.c_str(),12000.0,10.0);
      /*  */
      switch (err) {
       case -1 :
            info[0]='\0';
            sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",szFileName );
            Application->MessageBox(info,"Корректировка подачи - > ошибка ...", MB_OK);
       break;
       case -2 :
            info[0]='\0';
            sprintf(info,"Невозможно открыть файл:\n %s \n на 'запись' \n",fn.c_str() );
            Application->MessageBox(info,"Корректировка подачи - > ошибка ...", MB_OK);
       break;
       case -3 : break;
       default :
          info[0]='\0';
          sprintf(info,"Преобразование Файла УП ->\n %s \n\t\t ЗАВЕРШЕНО.\n\n Общее число преобразованных кадров=%ld ",fn.c_str() , err );
          MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);

          /*Если преобразования исходного файла не было - удаляем *.bak*/
          if (err==0) {
          	if (FileExists(szFileName)) { DeleteFile(szFileName); }
          }

          /*опционально - не доделано*/
          /*вывод формы с 2-мя файлами */
          if (CheckBoxShowFile->Checked==True ) {
               Form6->StringGridFiles->Cells[0][0]=TrimRight(AnsiString( szFileName ));
               Form6->StringGridFiles->Cells[1][0]=TrimRight(fn);
               /*Открываем файл*/
               rf=fopen(szFileName,"r");
               ff=fopen(fn.c_str(),"r");
               curr1[0]='\0'; fgets(curr1,255,rf);
               curr2[0]='\0'; fgets(curr2,255,ff);
               i=1;
               while(!feof(rf))
               {
                  Form6->StringGridFiles->Cells[0][i]=TrimRight(AnsiString(curr1));
                  Form6->StringGridFiles->Cells[1][i]=TrimRight(AnsiString(curr2));

                  curr1[0]='\0'; fgets(curr1,255,rf);
                  curr2[0]='\0'; fgets(curr2,255,ff);
                  i++;
                } // end while
               fclose(rf);fclose(ff);
               CheckBoxShowFile->Checked=False;
               Form6->Num->Text=IntToStr(i);
               Form6->Visible=True;
           }  //CheckBoxShowFile

          i=program_statistics(fn.c_str(),100000.0,&infofile);

          Form2->Type->Caption="MTC200 (INDRAMAT)";
          Form2->Type->Font->Color=clBlue;
          if (infofile.fl_n33  !=0) {
             Form2->Type->Caption="H33 (ATAKA)";
             Form2->Type->Font->Color=clRed;
           }
          //FloatToStr(TIME); IntToStr(numbers_of_kadrs); FormatFloat("0.00",TIME);
          //Form2->FileInfo->Caption=info;
          Form2->FileInfo->Lines->Clear();    Form2->FileInfo->Lines->SetText(infofile.info);
          Form2->Time->Text=FormatFloat("0.00",infofile.TIME);
          Form2->Dist->Text=FormatFloat("0.00",infofile.DIST);
          Form2->NumKadrs->Text=FormatFloat("0",infofile.numbers_of_kadrs);
          Form2->Minfeed->Text=FormatFloat("0.00",infofile.min_feed);
          Form2->Maxfeed->Text=FormatFloat("0.00",infofile.max_feed);
          Form2->NumLine->Text=FormatFloat("0",infofile.numbers_of_kadrs_line);
          Form2->NumCircle->Text=FormatFloat("0",infofile.numbers_of_kadrs_circle);

          Form2->StringGrid1->Cells[0][0]=" Габариты ";
          Form2->StringGrid1->Cells[1][0]=" X ";
          Form2->StringGrid1->Cells[2][0]=" Y ";
          Form2->StringGrid1->Cells[3][0]=" Z ";
          Form2->StringGrid1->Cells[0][1]=" min ";
          Form2->StringGrid1->Cells[0][2]=" max ";
          Form2->StringGrid1->Cells[0][3]=" Сумма ";
          for (i = 1; i< Form2->StringGrid1->ColCount; i++)
             Form2->StringGrid1->Cells[i][1] = FormatFloat("0.00",infofile.min_points[i-1]);
          for (i = 1; i< Form2->StringGrid1->ColCount; i++)
             Form2->StringGrid1->Cells[i][2] = FormatFloat("0.00",infofile.max_points[i-1]);
          for (i = 1; i< Form2->StringGrid1->ColCount; i++)
             Form2->StringGrid1->Cells[i][3] = FormatFloat("0.00",infofile.SUMXYZ[i-1]);

          // Form2->ModalResult = mrOk;// Form2->ShowModal();
          Form2->Visible=True;

       break;
      }

    } else {
       info[0]='\0';
       sprintf(info,"Проверьте наличие Файла УП ->\n %s \n Корректировка прервана....",fn.c_str() );
       MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
    }

  }
  __finally
  {
    i=Form1->FileListBox1->ItemIndex;
    Form1->FileListBox1->Update();
    Form1->FileListBox1->ItemIndex=i;
  }

}
//---------------------------------------------------------------------------

