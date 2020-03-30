//---------------------------------------------------------------------------

#ifndef statH
#define statH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *Time;
        TButton *Button1;
        TLabel *Label2;
        TLabel *Label3;
        TGroupBox *GroupBox1;
        TLabel *Label6;
        TLabel *Label7;
        TStringGrid *StringGrid1;
        TEdit *Dist;
        TEdit *NumKadrs;
        TEdit *NumLine;
        TEdit *NumCircle;
        TGroupBox *GroupBox2;
        TLabel *Label4;
        TLabel *Label8;
        TEdit *Minfeed;
        TEdit *Maxfeed;
        TBevel *Bevel1;
        TStaticText *StaticText1;
        TLabel *Type;
        TMemo *FileInfo;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
