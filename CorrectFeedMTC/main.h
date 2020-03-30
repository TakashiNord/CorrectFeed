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
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TDriveComboBox *DriveComboBox1;
        TDirectoryListBox *DirectoryListBox1;
        TFilterComboBox *FilterComboBox1;
        TFileListBox *FileListBox1;
        TButton *ButtonExit;
        TGroupBox *GroupBox1;
        TPopupMenu *PopupMenu1;
        TMenuItem *N0note;
        TMenuItem *N1stat;
        TMenuItem *N2proverka;
        TRichEdit *RichEdit1;
        TRichEdit *RichEdit2;
        TTimer *Timer1;
        TMenuItem *N3new;
        TMenuItem *N4delete;
        TGroupBox *GroupBox2;
        TButton *ButtonChangeFeed;
        TButton *ButtonTimeCalc;
        TStaticText *StaticText1;
        TImage *Image1;
        TPageControl *PageControl1;
        TTabSheet *TabSheetMTC200;
        TLabel *Label8;
        TLabel *Label3;
        TLabel *Label6;
        TLabel *Label2;
        TLabel *Label4;
        TBevel *Bevel1;
        TLabel *Label9;
        TComboBox *ComboBoxFeed;
        TComboBox *acc;
        TBitBtn *BitBtnHelp;
        TEdit *accconstslowdn;
        TEdit *mintimeworking;
        TEdit *accvalue;
        TBitBtn *BitBtnCorrectMtc200;
        TCheckBox *CheckBoxShowFile;
        TStaticText *StaticText2;
        void __fastcall accChange(TObject *Sender);
        void __fastcall ButtonExitClick(TObject *Sender);
        void __fastcall N0noteClick(TObject *Sender);
        void __fastcall N1statClick(TObject *Sender);
        void __fastcall N2proverkaClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall N3newClick(TObject *Sender);
        void __fastcall N4deleteClick(TObject *Sender);
        void __fastcall ButtonTimeCalcClick(TObject *Sender);
        void __fastcall BitBtnHelpClick(TObject *Sender);
        void __fastcall ButtonChangeFeedClick(TObject *Sender);
        void __fastcall Image1Click(TObject *Sender);
        void __fastcall BitBtnCorrectMtc200Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
