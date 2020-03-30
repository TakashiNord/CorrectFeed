//---------------------------------------------------------------------------

#ifndef razbivkaH
#define razbivkaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TRazbivka
{
private:
        int SPEED_LIMIT,a,FEED_MAX;
        int i;             /*Счетчик файлов*/
        long LEN_LIMIT; /*байт. Максимальная длина up-файла до запуска
                        up_to_up*/
        FILE *f,*f1,*f2;
        int byte_cnt;             /* Счетчик байтов */
        char end[20][70];      /* Массив конечных строк первого файла */
        int j;               /* Счетчик конечных строк */
        int flag;  /* 0 - <LEN_LIMIT  ;  1 - >LEN_LIMIT */
        int flag1; /* 0 - во второй файл еще не записан ни один кадр
                      с перемещением */
        char word_f[10],word_l[10];
        int G01_2_3;
        int G17_18_19;
        int MAX_FZ;
        long SUMX,SUMY,SUMZ; /* Контрольные суммы */
        void __fastcall razbivka(char *f_name);
        void __fastcall razbivka1(char *curr);
        void __fastcall razbivka2(char *curr);
        void __fastcall next_file(void);
        void __fastcall feed_convert(int f,int mode,char *new_feed);
        void __fastcall replace_word(char *s,char w,char *sw);
        void __fastcall add_word(char *curr,char *word);
        int  __fastcall read_word(char *s,char w,char *sw);
public:		// User declarations
        __fastcall TRazbivka(char *file,int SpeedLimit,
                int A,int FeedMax);
};
//---------------------------------------------------------------------------
extern PACKAGE TRazbivka *Razbivka;
//---------------------------------------------------------------------------
#endif

 