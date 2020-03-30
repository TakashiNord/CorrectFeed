//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainDialog : public TForm
{
__published:	// IDE-managed Components
        TDriveComboBox *DriveComboBox1;
        TDirectoryListBox *DirectoryListBox1;
        TFilterComboBox *FilterComboBox1;
        TFileListBox *FileListBox1;
        TButton *ButtonExit;
        TPopupMenu *PopupMenu1;
        TMenuItem *N0note;
        TMenuItem *N2proverka;
        TTimer *Timer1;
        TMenuItem *N3new;
        TMenuItem *N4delete;
        TEdit *EditFile;
        TLabel *Label1;
        TBitBtn *BitBtn1;
        TTabControl *TabControlMain;
        TLabel *Label2;
        TComboBox *ComboBoxMachine;
        TLabel *Label3;
        TGroupBox *GroupBox1;
        TBitBtn *BitBtnCorrectH33;
        TEdit *SpeedLimitField;
        TEdit *FeedMaxField;
        TEdit *AField;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TBevel *Bevel1;
        TRadioGroup *RadioGroupSeq;
        TCheckBox *CheckBoxStat;
        TCheckBox *CheckCircleSlowdn;
        TSaveDialog *SaveDialog1;
        void __fastcall ButtonExitClick(TObject *Sender);
        void __fastcall N0noteClick(TObject *Sender);
        void __fastcall N2proverkaClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall N3newClick(TObject *Sender);
        void __fastcall N4deleteClick(TObject *Sender);
        void __fastcall SpeedLimitFieldChange(TObject *Sender);
        void __fastcall FeedMaxFieldChange(TObject *Sender);
        void __fastcall AFieldChange(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall ComboBoxMachineChange(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall TabControlMainChange(TObject *Sender);
        void __fastcall BitBtnCorrectH33Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        char UP_file[255];
        int SPEED_LIMIT;       /* ����������� ���������� ������
			          ��� ������ ������� (��/���)*/
        int FEED_MAX;          /* ����������� ���������� ������
			          ��������� ���� (��/���)*/
        int a;                 /* ��������� (�� � ���*���) */
        int acenter;           /* ������������������� ��������� */
        char PLANE[10];/* ��������� �������� ������������ */
        int G;      /*  G=2 - �������� ������������ �� ������� �������;
			G=3 - �������� ������������ ������ ������� ������� */
        int flag;

        struct post{char kadr[70]; /* ���� */
		    int feed;      /* ������ */
		    float span;    /* ����������� */
		    }P[20];
        int i;   /* ������ ��� ������� �������� P */
        long int k; /* ������� ������ */
        int feed; /* ������ �������� ����� curr */
        int old_f; /* ������ ����������� ����� */
        int mode; /* ���� */
        float SUMX,SUMY,SUMZ;
        char s_curr[200],*curr,s_next[200],*next;
        FILE *input,*out;
        //
        int mode_machine; /* ������-����� */
        int mode_seq; /* ��������� ������ */
        int mode_output;  /* ��������� ����������� ���� � �������� */
        int mode_circle_slowdn ; /* ��������������� �������� �� ���������� */
        float circle_dist;  /* ���������� �� ���������� �� �������� ����� ����������� */
        //
        void __fastcall PtpToUp(char *file1,char *file2);
        void __fastcall feed_convert(int a,int b,char *c);
        void __fastcall feed_del(char *kadr);
        int  __fastcall otkat(int j);
        void __fastcall tangent(float a,float b,float *c,float *d);
        void __fastcall analiz(char *curr,char *next);
        int  __fastcall read_word(char *a, char b, char *c);
        void __fastcall replace_word(char *a,char b,char *c);
        //
        void __fastcall print_error(int err,char *mes);
        float __fastcall tangentn(float i,float j,float *xk,float *yk,int tp);
        float __fastcall EQ_sign (float s);
        int __fastcall none_methods (void);
        //

        __fastcall TMainDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainDialog *MainDialog;
//---------------------------------------------------------------------------
#endif
