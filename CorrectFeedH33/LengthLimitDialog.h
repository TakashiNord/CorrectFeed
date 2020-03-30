//---------------------------------------------------------------------------

#ifndef LengthLimitDialogH
#define LengthLimitDialogH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
class TLengthLimit : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *Length;
        TButton *OKButton;
        TButton *ButtonCancel;
        void __fastcall ButtonCancelClick(TObject *Sender);
        void __fastcall OKButtonClick(TObject *Sender);
private:	// User declarations

public:		// User declarations
        __fastcall TLengthLimit(TComponent* Owner);
        long LEN_LIMIT;
};
//---------------------------------------------------------------------------
extern PACKAGE TLengthLimit *LengthLimit;
//---------------------------------------------------------------------------
#endif
