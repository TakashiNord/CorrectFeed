//---------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <dir.h>

#include <vcl.h>
#pragma hdrstop

#include "main.h" // MainDialog - '������� �����'

#include "stat.h" // Form2 - '���� ����������'

#include "TimeCount.h"  //������� ������� ��� H33 - ��������
#include "UpToUp.h" //������������� �� ��� H33 - ��������

#include "algobase_m.h" //��������� ���������� - �����

#include "about.h" // Form5 - � ��������� � �����������

#define bStr 512

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainDialog *MainDialog;
//---------------------------------------------------------------------------
__fastcall TMainDialog::TMainDialog(TComponent* Owner)
        : TForm(Owner)
{
}

//---------------------------------------------------------------------------

void __fastcall TMainDialog::ButtonExitClick(TObject *Sender)
{
    Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::N0noteClick(TObject *Sender)
{
// notepad
  char info[bStr];
  AnsiString fn ;
  fn=FileListBox1->FileName ;
  /*ShowMessage(fn);*/

  info[0]='\0';
  strcpy(info,"notepad ");//strcpy(info,"uedit32.exe \"%f\" ");
  strcat(info, fn.c_str() );
  try
  {
    WinExec(info,1);
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }

}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::N2proverkaClick(TObject *Sender)
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
            sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",fn.c_str() );
            Application->MessageBox(info,"�������� ��������� �� ������ H33 - > ������ ...", MB_OK);
            break ;
   case 0 :
            info[0]='\0';
            sprintf(info,"���� �� ->\n %s \n\n ����� ������ ���� H33 (Ataka)",fn.c_str() );
            MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);
            break ;
   default :
            info[0]='\0';
            sprintf(info,"���� �� ->\n %s \n\n �� ����� ������ ���� H33 (Ataka)",fn.c_str() );
            MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
            break ;
  }

}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::Timer1Timer(TObject *Sender)
{
    int i;
    i=MainDialog->FileListBox1->ItemIndex;
    MainDialog->DirectoryListBox1->Update();
    MainDialog->FileListBox1->Update();
    MainDialog->FileListBox1->ItemIndex=i;
}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::N3newClick(TObject *Sender)
{
    int i;
    i=MainDialog->FileListBox1->ItemIndex;
    MainDialog->FileListBox1->Update();
    MainDialog->FileListBox1->ItemIndex=i;
}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::N4deleteClick(TObject *Sender)
{
    int i; char info[bStr];
    AnsiString fn ;

    fn=FileListBox1->FileName ;
    /*ShowMessage(fn);*/
    if (fn=="") return ;

    info[0]='\0';
    sprintf(info,"������� ���� ->\n %s \n\t\t ? ....\n �������� ! ���� ����� ������ ������������ ! ",fn.c_str() );
    i=MessageDlg(info, mtError , TMsgDlgButtons() << mbNo << mbYes ,0);
    if (i==mrYes) {
     if (FileExists(fn.c_str() )) { DeleteFile(fn.c_str()); }
     i=MainDialog->FileListBox1->ItemIndex;
     MainDialog->FileListBox1->Update();
     MainDialog->FileListBox1->ItemIndex=i;
    }

}
//---------------------------------------------------------------------------



void __fastcall TMainDialog::SpeedLimitFieldChange(TObject *Sender)
{
        int tmp ;
        tmp=abs(atoi(SpeedLimitField->Text.c_str()));
        if (tmp<=0) tmp=20 ;
        SpeedLimitField->Text=tmp;
}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::FeedMaxFieldChange(TObject *Sender)
{
       int tmp ;
       tmp=abs(atoi(FeedMaxField->Text.c_str()));
       if (tmp<=0) tmp=240 ;
       FeedMaxField->Text=tmp;
}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::AFieldChange(TObject *Sender)
{
       int tmp ;
       tmp=abs(atoi(AField->Text.c_str()));
       if (tmp<=0) tmp=5 ;
       AField->Text=tmp;
}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::BitBtn1Click(TObject *Sender)
{
   Form5->Visible=True;        
}
//---------------------------------------------------------------------------


void __fastcall TMainDialog::ComboBoxMachineChange(TObject *Sender)
{
  int p1;
  p1=ComboBoxMachine->ItemIndex;

  switch (p1) {
  case 1 :
    // ��-9
    SpeedLimitField->Text="50";
    FeedMaxField->Text="900";
    AField->Text="5";
    RadioGroupSeq->ItemIndex=1;
    CheckCircleSlowdn->Checked=False;
   break;
  case 2 :
    // ��-17
    SpeedLimitField->Text="100";
    FeedMaxField->Text="1200";
    AField->Text="50";
    RadioGroupSeq->ItemIndex=0;
    CheckCircleSlowdn->Checked=False;
   break;
  case 3 :
    // 6�13��
    SpeedLimitField->Text="50";
    FeedMaxField->Text="1000";
    AField->Text="5";
    RadioGroupSeq->ItemIndex=0;
    CheckCircleSlowdn->Checked=False;
   break;
  case 4 :
    // ��655�2
    SpeedLimitField->Text="100";
    FeedMaxField->Text="1000";
    AField->Text="50";
    RadioGroupSeq->ItemIndex=0;
    CheckCircleSlowdn->Checked=False;
   break;
   default :
    // NONE
    SpeedLimitField->Text="100";
    FeedMaxField->Text="1200";
    AField->Text="50";
    RadioGroupSeq->ItemIndex=0;
    CheckCircleSlowdn->Checked=True;
   }

}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::FormCreate(TObject *Sender)
{
  ComboBoxMachine->ItemIndex=2;
  ComboBoxMachineChange(Sender);
  TabControlMain->TabIndex=0;
  TabControlMainChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::TabControlMainChange(TObject *Sender)
{
  int p1;
  p1=TabControlMain->TabIndex;

  switch (p1) {
  case 0 :
    // ptp-to-up
    SpeedLimitField->Enabled=True;
    FeedMaxField->Enabled=True;
    RadioGroupSeq->Enabled=True;
    CheckCircleSlowdn->Enabled=True;
   break;
  case 1 :
    // up-to-up
    SpeedLimitField->Enabled=True;
    FeedMaxField->Enabled=True;
    RadioGroupSeq->Enabled=False;
    CheckCircleSlowdn->Enabled=False;
   break;
  case 2 :
    // time
    SpeedLimitField->Enabled=False;
    FeedMaxField->Enabled=False;
    RadioGroupSeq->Enabled=False;
    CheckCircleSlowdn->Enabled=False;
   break;
   default :
    // NONE
    break;
   }

}
//---------------------------------------------------------------------------

void __fastcall TMainDialog::BitBtnCorrectH33Click(TObject *Sender)
{
 long int err ;
 char info[bStr];
 int i ;
 struct INFO_FILE_STAT infofile ;
 AnsiString fn , fnout;
 char PTP_file[255];
 double TIME ;
 int SPEED_LIMIT, FEED_MAX , a ;


 fn=FileListBox1->FileName ;
 /*ShowMessage(fn);*/
 if (fn=="") {
   ShowMessage("�������� ���� '���� H33' ��� ������������� �������������");
   return ;
 }

 if (!FileExists( fn.c_str() ))
 {
   info[0]='\0';
   sprintf(info,"���� �� ->\n %s \n\n �� ���������� \n ..",fn.c_str() );
   MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
   return ;
 }

  /* �������� �� ���������� ������ �� */
  err=program_format_H33(fn.c_str(),1);
  switch(err) {
    case -1 :
            info[0]='\0';
            sprintf(info,"���������� ������� ����:\n %s \n �� '������' ",fn.c_str() );
            Application->MessageBox(info,"�������� ��������� �� ������ H33 - > ������ ...", MB_OK);
    break ;
    case 0 :
    break ;
    default :
            info[0]='\0';
            sprintf(info,"���� �� ->\n %s \n\n �� �������� ���������� ��� H33 (Ataka)\n ���������� ��������....",fn.c_str() );
            MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
            return ;
   }

  SPEED_LIMIT = atoi(SpeedLimitField->Text.c_str());
  FEED_MAX = atoi(FeedMaxField->Text.c_str());
  a = atoi(AField->Text.c_str());

  PTP_file[0]='\0';
  UP_file[0]='\0';

  int p1;
  p1=TabControlMain->TabIndex;

  if (p1==0) {
   // ptp-to-up
   /* �������������� PTP-����� � UP */

        if (False==SaveDialog1->Execute() ) { return ; }
        fnout= SaveDialog1->FileName;
        strcpy( UP_file, fnout.c_str() );
        strcpy( PTP_file, fn.c_str() );
        if (strcmp(PTP_file, UP_file)==0) {
         info[0]='\0';
         sprintf(info,"��� ��������� ����� �� ->\n %s \n ��������� �  ����� �����������\n �������������� ��������..",PTP_file );
         MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
         return ;
        }

        /* ����� */
        mode_machine=ComboBoxMachine->ItemIndex;
        /* ������� ��������� ������ */
        mode_seq=0;
        if (RadioGroupSeq->ItemIndex==1) mode_seq=1;
        /* ������� �������������� �������� ����� �������� �� ���������� */
        mode_circle_slowdn=1;
        if (CheckCircleSlowdn->State!=cbGrayed)
           if (CheckCircleSlowdn->Checked==False) mode_circle_slowdn=0; else mode_circle_slowdn=1;
         else { ; }
        /* ������� ������ �������� ��������� */
        mode_output=0;
        circle_dist=100;
        PtpToUp(PTP_file,UP_file);

        TTimeCounter *tc=new TTimeCounter(UP_file,a);
        TIME=(double) tc->count();
        tc->TTimeCounter::~TTimeCounter(); // ����������

  }

  if (p1==1) {
   // up-to-up
   /* �������������� UP-����� � UP */

        if (False==SaveDialog1->Execute() ) { return ; }
        fnout= SaveDialog1->FileName;
        strcpy( UP_file, fnout.c_str() );
        strcpy( PTP_file, fn.c_str() );
        if (strcmp(PTP_file, UP_file)==0) {
         info[0]='\0';
         sprintf(info,"��� ��������� ����� �� ->\n %s \n ��������� �  ����� �����������\n �������������� ��������..",PTP_file );
         MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
         return ;
        }

        TUpToUp *tup=new TUpToUp(PTP_file,UP_file,SPEED_LIMIT,a,FEED_MAX);
        i=tup->korrect();
        tup->TUpToUp::~TUpToUp(); // ����������

        if (i<0) {
          info[0]='\0';
          sprintf(info,"������ ��������������� ����� ��:\n %s \n - ��������� ��������....",fn.c_str() );
          MessageDlg(info, mtError, TMsgDlgButtons() << mbYes ,0);
          return ;
        }

        TTimeCounter *tc=new TTimeCounter(UP_file,a);
        TIME=(double) tc->count();
        tc->TTimeCounter::~TTimeCounter(); // ����������
        TIME/=60.0;
        info[0]='\0';
        sprintf(info,"���� �� ->\n %s \n ����� ���������� �� = %.2lf (���)",fn.c_str(),TIME );
        MessageDlg(info, mtInformation, TMsgDlgButtons() << mbYes ,0);

  }

  if (p1==2) {
   // time
   // ����� ����������
        UP_file[0]='\0';
        strcpy( UP_file, fn.c_str() );

        TTimeCounter *tc=new TTimeCounter(UP_file,a);
        TIME=(double)tc->count();
        tc->TTimeCounter::~TTimeCounter(); // ����������

        // ������ ������� - ����� � �����
        float  tmpTime , tm2;
        long int tm1 ;
        tmpTime =TIME/60.0 ;
        tm1=(int) tmpTime/60.0;
        tm2=(tmpTime/60.0 - tm1) * 60.0 ;
        info[0]='\0';
        if (tm1==0) {
           sprintf(info,"\n����� ���������� ->\n %10.0f ������ = %.2f �����",TIME,TIME/60);
         } else {
            sprintf(info,"\n����� ���������� ->\n %10.0f ������ = %.2f ����� = [ %d � %.1f ��� ]",TIME,tmpTime, tm1 ,tm2);
        }
        /*�������� ���������*/
        MessageDlg(info, mtConfirmation, TMsgDlgButtons() << mbOK , 0);

        TIME/=60.0;
  }

  if (CheckBoxStat->State!=cbGrayed)
   if (CheckBoxStat->Checked==True) {
        if (p1==0) return ;
        /*������� ���� ����������*/
          i=program_statistics(UP_file,FEED_MAX,&infofile);

          Form2->Type->Caption="H33 (ATAKA)";
          Form2->Type->Font->Color=clRed;
          //FloatToStr(TIME); IntToStr(numbers_of_kadrs); FormatFloat("0.00",TIME);
          //Form2->FileInfo->Caption=info;
          Form2->FileInfo->Lines->Clear();    Form2->FileInfo->Lines->SetText(infofile.info);
          Form2->Time->Text=FormatFloat("0.00",TIME);
          Form2->Dist->Text=FormatFloat("0.00",infofile.DIST);
          Form2->NumKadrs->Text=FormatFloat("0",infofile.numbers_of_kadrs);
          Form2->Minfeed->Text=FormatFloat("0.00",infofile.min_feed);
          Form2->Maxfeed->Text=FormatFloat("0.00",infofile.max_feed);
          Form2->NumLine->Text=FormatFloat("0",infofile.numbers_of_kadrs_line);
          Form2->NumCircle->Text=FormatFloat("0",infofile.numbers_of_kadrs_circle);

          Form2->StringGrid1->Cells[0][0]=" �������� ";
          Form2->StringGrid1->Cells[1][0]=" X ";
          Form2->StringGrid1->Cells[2][0]=" Y ";
          Form2->StringGrid1->Cells[3][0]=" Z ";
          Form2->StringGrid1->Cells[0][1]=" min ";
          Form2->StringGrid1->Cells[0][2]=" max ";
          Form2->StringGrid1->Cells[0][3]=" ����� ";
          for (i = 1; i< Form2->StringGrid1->ColCount; i++)
             Form2->StringGrid1->Cells[i][1] = FormatFloat("0.00",infofile.min_points[i-1]);
          for (i = 1; i< Form2->StringGrid1->ColCount; i++)
             Form2->StringGrid1->Cells[i][2] = FormatFloat("0.00",infofile.max_points[i-1]);
          for (i = 1; i< Form2->StringGrid1->ColCount; i++)
             Form2->StringGrid1->Cells[i][3] = FormatFloat("0.00",infofile.SUMXYZ[i-1]);

         //Form2->ShowModal(); /* Form2->ModalResult = mrOk; */
          Form2->Visible=True;
  }

 /*===================================================================*/
 try
 {

 }
  __finally
  {
    i=MainDialog->FileListBox1->ItemIndex;
    MainDialog->FileListBox1->Update();
    MainDialog->FileListBox1->ItemIndex=i;
  }

}
//---------------------------------------------------------------------------

