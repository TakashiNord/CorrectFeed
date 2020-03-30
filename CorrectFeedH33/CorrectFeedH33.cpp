//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("CorrectFeedH33.res");
USEFORM("main.cpp", MainDialog);
USEFORM("stat.cpp", Form2);
USEUNIT("TimeCount.cpp");
USEFORM("about.cpp", Form5);
USEUNIT("UpToUp.cpp");
USEUNIT("algobase_m.cpp");
USEFORM("FileSelect.cpp", FileSelectDialog);
USEFORM("LengthLimitDialog.cpp", LengthLimit);
USEUNIT("razbivka.cpp");
USEUNIT("ptp_to_up.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainDialog), &MainDialog);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->CreateForm(__classid(TForm5), &Form5);
                 Application->CreateForm(__classid(TFileSelectDialog), &FileSelectDialog);
                 Application->CreateForm(__classid(TLengthLimit), &LengthLimit);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
