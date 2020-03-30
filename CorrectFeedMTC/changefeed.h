//---------------------------------------------------------------------------

#ifndef changefeedH
#define changefeedH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
        TRadioGroup *RadioChange;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TEdit *Feedmax;
        TEdit *Feedmin;
        TEdit *Procent;
        TUpDown *UpDown1;
        TEdit *FileName;
        TLabel *Label1;
        TStaticText *StaticText1;
        TImage *Image1;
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FeedmaxChange(TObject *Sender);
        void __fastcall FeedminChange(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm4(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
