//---------------------------------------------------------------------------

#ifndef FileSelectH
#define FileSelectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
class TFileSelectDialog : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *FileName;
        TButton *OKButton;
        TButton *Browse;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        void __fastcall BrowseClick(TObject *Sender);
        void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
        AnsiString ResultFileName;
        __fastcall TFileSelectDialog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFileSelectDialog *FileSelectDialog;
//---------------------------------------------------------------------------
#endif
