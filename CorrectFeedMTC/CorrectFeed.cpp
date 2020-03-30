//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("CorrectFeed.res");
USEFORM("main.cpp", Form1);
USEFORM("stat.cpp", Form2);
USEFORM("Htabl.cpp", Form3);
USEFORM("changefeed.cpp", Form4);
USEUNIT("algobase.cpp");
USEFORM("about.cpp", Form5);
USEFORM("showfiles.cpp", Form6);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TForm2), &Form2);
                 Application->CreateForm(__classid(TForm3), &Form3);
                 Application->CreateForm(__classid(TForm4), &Form4);
                 Application->CreateForm(__classid(TForm5), &Form5);
                 Application->CreateForm(__classid(TForm6), &Form6);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
