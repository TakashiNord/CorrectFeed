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
        int SPEED_LIMIT;    /* Максимально допустимая подача
			       при полном реверсе (мм/мин)*/
        int FEED_MAX;       /* Максимально допустимая подача
			       холостого хода (мм/мин)*/
        int a;              /* Ускорение (мм в сек*сек) */
        int acenter;        /* Центростремительное ускорение */
        char PLANE[10];     /* Плоскость круговой интерполяции */
        int G;      /*  G=2 - круговая интерполяция по часовой стрелке;
			G=3 - круговая интерполяция против часовой стрелки */
        int flag; int f2;
        struct post{char kadr[70]; /* кадр */
		    int feed;   /* подача */
		    float span; /* перемещение */
		    }P[20];
        int i;   /* Индекс для массива структур P */
        int k; /* Счетчик кадров */
        int f; /* Подача текущего кадра curr */
        int mode; /* Флаг */
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


 