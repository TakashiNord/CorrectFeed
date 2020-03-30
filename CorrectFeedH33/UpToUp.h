//---------------------------------------------------------------------------

#ifndef uptoupH
#define uptoupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TUpToUp
{
private:
        int SPEED_LIMIT;    /* ����������� ���������� ������
			       ��� ������ ������� (��/���)*/
        int FEED_MAX;       /* ����������� ���������� ������
			       ��������� ���� (��/���)*/
        int a;              /* ��������� (�� � ���*���) */
        int acenter;        /* ������������������� ��������� */
        char PLANE[10];     /* ��������� �������� ������������ */
        int G;      /*  G=2 - �������� ������������ �� ������� �������;
			G=3 - �������� ������������ ������ ������� ������� */
        int flag; int f2;
        struct post{char kadr[70]; /* ���� */
		    int feed;   /* ������ */
		    float span; /* ����������� */
		    }P[20];
        int i;   /* ������ ��� ������� �������� P */
        int k; /* ������� ������ */
        int f; /* ������ �������� ����� curr */
        int mode; /* ���� */
        float SUMX,SUMY,SUMZ;
        char s_curr[200],*curr,s_next[200],*next;
        FILE *input,*out;
        char UP_file1[100], UP_file2[100];

        int  __fastcall read_word(char *p1, char p2, char *p3);
        void __fastcall replace_word(char *p1,char p2,char *p3);
        void __fastcall feed_convert(int p1,int p2,char *p3);
        void __fastcall feed_del(char *kadr);
        int  __fastcall otkat(int j);
        int  __fastcall feed_convert1(int ff);
        void __fastcall tangent(float p1,float p2,float *p3,float *p4);
        void __fastcall analiz1(char *curr,char *next);

public:		// User declarations
        __fastcall TUpToUp(char *file1,char *file2,int SpeedLimit,
                int A,int FeedMax);
        int __fastcall TUpToUp::korrect();
        ~ TUpToUp();
        
};
//---------------------------------------------------------------------------
extern PACKAGE TUpToUp *UpToUp;
//---------------------------------------------------------------------------
#endif


 