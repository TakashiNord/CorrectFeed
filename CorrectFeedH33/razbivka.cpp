#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

#include <vcl.h>
#pragma hdrstop

#include "LengthLimitDialog.h"
#include "FileSelect.h"
#include "razbivka.h"
#include "UpToUp.h"
#include "TimeCount.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// Конструктор
__fastcall TRazbivka::TRazbivka(char *up_file,int sl,int A,int fm)
{
        SPEED_LIMIT=sl; a=A; FEED_MAX=fm;
        i=1;   /*Счетчик файлов*/
        SUMX=0,SUMY=0,SUMZ=0;
        byte_cnt=0;             /* Счетчик байтов */
        j=0;               /* Счетчик конечных строк */
        flag=0;  /* 0 - <LEN_LIMIT  ;  1 - >LEN_LIMIT */
        flag1=0; /* 0 - во второй файл еще не записан ни один кадр с перемещением */
        G01_2_3=1;
        G17_18_19=0;
        MAX_FZ=300;

        long f_len; /* Длина up-файла */
        struct stat buf;
        stat(up_file,&buf);
        f_len=buf.st_size;
        char lab[300];
        sprintf(lab,"Размер файла %s - %ld байт(а).\n\
Есть ограничение на его длину?",up_file,f_len);

        // Если "Нет" - выход
        if(MessageDlg(lab,mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo,0)!=mrYes)
          {  /*Application->Terminate();*/
             return ;
          }
        // Если "Да" - вывести форму для задания длины файлов.
        try
        {
        TLengthLimit *dlg = new TLengthLimit(Application);
        if( dlg->ShowModal() == mrOk)
                {LEN_LIMIT = dlg->LEN_LIMIT;
                 razbivka(up_file);}
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }

}
/* Эта функция разбивает ленту, длина которой превышает LEN_LIMIT,
   на части */
void __fastcall TRazbivka::razbivka(char *f_name)
{
long f_len; /* Длина файла f_name */
char ch,str[200];
struct stat buf;
char s_curr[200],*curr;
char f_name2[100];

stat(f_name,&buf);
f_len=buf.st_size;
if(f_len<LEN_LIMIT)  /* ВЫХОД ИЗ РЕКУРСИИ */
{ShowMessage("Разбивка закончена");
 /*Application->Terminate();*/
 return ;
 }

strcpy(word_l,"");
while(1)
{
        char lab[300];
        sprintf(lab,"Размер файла %s - %ld байт(а).\n\
Разбить его на два?\n\
(Размер первой части файла будет примерно %ld байт(а))",
f_name,f_len,LEN_LIMIT+200);

        // Если "Нет" - выход
        if(MessageDlg(lab,mtConfirmation,TMsgDlgButtons()<<mbYes<<mbNo,0)!=mrYes)
            break;
        // Если "Да" - вывести форму для задания имени файла
        TFileSelectDialog *fsd = new TFileSelectDialog(Application);
        if( fsd->ShowModal() == mrOk)
        {
                 sprintf(f_name2,"%s",fsd->FileName->Text.c_str());

                 if(strcmp(f_name,f_name2)==0)
                 {
                        char s[100];
                        sprintf(s,"Нельзя затирать файл %s",f_name);
                        ShowMessage(s);
                        razbivka(f_name);
                 }

		 f=fopen(f_name,"r"); /* Открытие разделяемого файла */
		 f1=fopen("c:\\temp\\tmp.ptp","w");/* Открытие файла 1-ой части разделяемого файла */

        	 f2=fopen(f_name2,"w"); /* Открытие файла 2-ой части разделяемого файла*/
		 fputs("%\n",f2);
		 curr=fgets(s_curr,199,f);
		 while(curr!=NULL)
			  {
			  byte_cnt+=strlen(curr)+1;
			  if(flag==0 && byte_cnt>LEN_LIMIT) {next_file();flag=1;}
			  if(flag==0)razbivka1(curr);         /* Писать в 1-ый файл */
			  else /* flag==1 */ razbivka2(curr); /* Писать во 2-ой файл */
			  curr=fgets(s_curr,199,f);
			  } /*Конец while (curr!=NULL)*/
		 fclose(f1);fclose(f2);fclose(f);

                 TUpToUp *up1=new TUpToUp("c:\\temp\\tmp.ptp",f_name,SPEED_LIMIT,
                                         a,FEED_MAX);
                 up1->korrect();
                 up1->TUpToUp::~TUpToUp(); // деструктор
                 TTimeCounter *t1=new TTimeCounter(f_name,a);
                 t1->count();
                 t1->TTimeCounter::~TTimeCounter(); // деструктор
                 TUpToUp *up2=new TUpToUp(f_name2,"c:\\temp\\tmp.ptp",SPEED_LIMIT,
                                         a,FEED_MAX);
                 up2->korrect();
                 up2->TUpToUp::~TUpToUp(); // деструктор
                 TTimeCounter *t2=new TTimeCounter("c:\\temp\\tmp.ptp",a);
                 t2->count();
                 t2->TTimeCounter::~TTimeCounter(); // деструктор

		 remove(f_name2);
                 rename("c:\\temp\\tmp.ptp",f_name2); 
		 //printf("\nФайл tmp.ptp переименован в %s\n",f_name2);
		 flag=0; flag1=0;
		 SUMX=0;SUMY=0;SUMZ=0;
		 G01_2_3=1; G17_18_19=0;
		 byte_cnt=0; j=0;
                 razbivka(f_name2);
       }// end of if( fsd->ShowModal() == mrOk)
}//end while(1)
  /*Application->Terminate();*/
  return ;
}

/* Эта функция анализирует строку разбиваемого файла.flag1=0;
	Считает абсолютные координаты и количество байт. */
void __fastcall TRazbivka::razbivka1(char *curr)
{
long x,y,z;
char	word_x[50],word_y[50],word_z[50];
char str[50];

/*int read_word(char *s,char w,char *sw);*/

if (read_word(curr,'X',word_x)) { sscanf(word_x,"%D",&x);
											 SUMX=SUMX+x;}
else x=0;
if (read_word(curr,'Y',word_y)) { sscanf(word_y,"%D",&y);
											 SUMY=SUMY+y;}
else y=0;
if (read_word(curr,'Z',word_z)) { sscanf(word_z,"%D",&z);
											 SUMZ=SUMZ+z;}
else z=0;
read_word(curr,'F',word_f);
read_word(curr,'L',word_l);
if(read_word(curr,'S',word_x)){
				read_word(curr,'M',word_y);
				sprintf(str,"N000S%sM%s\n",word_x,word_y);
				fputs(str,f2);
			      }

if(strstr(curr,"M03")!=NULL){fputs("N000M03\n",f2);
			     strcpy(end[j++],"N000M05\n");}
else if(strstr(curr,"M04")!=NULL){fputs("N000M04\n",f2);
				  strcpy(end[j++],"N000M05\n");}
if(strstr(curr,"M08")!=NULL){fputs("N000M08\n",f2);
			     strcpy(end[j++],"N000M09\n");}

if(read_word(curr,'T',word_x)){
				sprintf(str,"N000M06T%s\n",word_x);
				fputs(str,f2);
			      }
if(strstr(curr,"G01")!=NULL)G01_2_3=1;
else if(strstr(curr,"G02")!=NULL)G01_2_3= 2;
	  else if(strstr(curr,"G03")!=NULL)G01_2_3=3;
if(strstr(curr,"G17")!=NULL)G17_18_19=17;
else if(strstr(curr,"G18")!=NULL)G17_18_19=18;
	  else if(strstr(curr,"G19")!=NULL)G17_18_19=19;

/* Вывод строки в файл */
fputs(curr,f1);
}

/* Эта функция выводит строку разбиваемого файла во 2-ой файл */

void __fastcall TRazbivka::razbivka2(char *curr)
{
char	word_x[50],word_y[50],word_z[50],word[10];

/* Если это первое перемещение во втором файле */
if(flag1==0 && (read_word(curr,'X',word_x) || read_word(curr,'Y',word_y) ||
					 read_word(curr,'Z',word_z)))
					 {
					 flag1=1;
					 /* Добавить подачу */
					 if(strchr(curr,'F')==NULL) {sprintf(word,"F%s",word_f);
														  add_word(curr,word);}
					 /* Добавить функцию круговой интерполяции */
					 if(strstr(curr,"G01")==NULL && strstr(curr,"G02")==NULL &&
						 strstr(curr,"G03")==NULL && G01_2_3!=1)
						 {sprintf(word_x,"N000G%2.2d\n",G01_2_3);fputs(word_x,f2);}
					 }
fputs(curr,f2);
}

/* Эта функция вызывается при достижении первым файлом предельных размеров.
	Она формирует конец первого файла и переход в нужную точку во втором файле */

void __fastcall TRazbivka::next_file(void)
{
int k; char  new_feed[10];
char s_x[10],s_y[10],s_z[10],str[70];
char * XYZ_format(long X);
for(k=0;k<j;k++)
	fputs(end[k],f1);
/* GO HOME */
sprintf(s_x,"X%+6.6ld",-SUMX);
sprintf(s_y,"Y%+6.6ld",-SUMY);
sprintf(s_z,"Z%+6.6ld",-SUMZ);
if(SUMZ!=0)
	/* Не вставлять отмену коррекции на длину инструмента */
  if(strcmp(word_l,"")==0){sprintf(str,"N000G01%sF0660\n",s_z);
                           feed_convert(MAX_FZ,0,new_feed);
		           replace_word(str,'F',new_feed);
                           fputs(str,f1);}
	/* Вставить отмену коррекции на длину инструмента */
  else {sprintf(str,"N000G01G40%sF0660L%s\n",s_z,word_l);
        feed_convert(MAX_FZ,0,new_feed); 
	replace_word(str,'F',new_feed);
        fputs(str,f1);}
if(SUMX!=0 && SUMY!=0){sprintf(str,"N000G01%s%sF0712\n",s_x,s_y);fputs(str,f1);}

fputs("N000M30\n",f1);
fputs("$\n",f1);
fclose(f1);

/* Подход к нужной точке во втором файле */
sprintf(s_x,"X%+6.6ld",SUMX);
sprintf(s_y,"Y%+6.6ld",SUMY);
sprintf(s_z,"Z%+6.6ld",SUMZ);
if(SUMX!=0 && SUMY!=0){sprintf(str,"N000G01%s%sF0712\n",s_x,s_y);fputs(str,f2);}
if(SUMZ!=0)
 if(strcmp(word_l,"")==0){sprintf(str,"N000G01%sF0660\n",s_z);
                          feed_convert(MAX_FZ,0,new_feed);
		          replace_word(str,'F',new_feed);
                          fputs(str,f2);}
 else  {sprintf(str,"N000G01%sF0660L%s\n",s_z,word_l);
        feed_convert(MAX_FZ,0,new_feed);
        replace_word(str,'F',new_feed);
        fputs(str,f2);}
/* Задание плоскости интерполяции */
if(G17_18_19!=0){sprintf(str,"N000G%d\n",G17_18_19);fputs(str,f2);}

}


/* This function read the numeric string after key letter
	in success it return 1, otherwice 0
*/
int __fastcall TRazbivka::read_word(char *s,char w,char *sw)
{   char *word,sym;
	 int  n;
	 if(s==NULL) return(0);
	 if ((word=strchr(s,w))==NULL) return(0);
	 n=0;
	 do
	 {  sym=*(word+(n+1));
		 if (isdigit(sym)||sym==' '||sym=='+'||sym=='-'||sym=='.') n++;
		 else break;
	 } while (1);
	 if (n>0){ strncpy(sw,word+1,n); sw[n]='\0'; return(n); }
	 else                                        return(0);
}

/* Эта функция дописывает слово в конец кадра curr */
void __fastcall TRazbivka::add_word(char *curr,char *word)
{
char new_kadr[70]; int i=0;
while(*(curr+i)!='\n')
 {
*(new_kadr+i)=*(curr+i);
i++;
 }
*(new_kadr+i)='\0';
strcat(new_kadr,strcat(word,"\n"));
strcpy(curr,new_kadr);
}

/* This function replace the numeric string after key letter
	if there isn't a key letter in a string funktion adds it
	at the end of a string
*/
void __fastcall TRazbivka::replace_word(char *s,char w,char *sw)
{
  int n;
  char old_sw[20],save[100],*p;

  if ((n=read_word(s,w,old_sw))!=0)
  {
	 /* replace word */
	 p=strchr(s,w);
	 *(p+1)='\0'; strcpy(save,p+n+1);
	 strcat(strcat(s,sw),save);
  }
  else
  {
	 /* add word at the end of string */
       n=read_word(s,'L',old_sw);
       if(n==0)
        {
			p=strchr(s,'<') ; /* string have <CR> or <LF> as text */
	 if (p==NULL)   p=strchr(s,'\r'); /* string have \r\n */
	 if (p==NULL)   p=strchr(s,'\n'); /* string have \n   */
	 if (p!=NULL)
	 { strcpy(save,p); *p=w; *(p+1)='\0'; strcat(strcat(s,sw),save); }
	 else
	 {
		save[0]=w; save[1]='\n'; save[2]='\0'; strcat(strcat(s,save),sw); }
        }
       else
        {
        p=strchr(s,'L'); *p=w;
	*(p+1)='\0'; strcpy(save,p+n+1);
	strcat(strcat(s,sw),save);         
        strcat(strcat(s,"L"),strcat(old_sw,"\n"));
        }
		
  }
}


/*  This function code the feed value for N33 numeric control
*/
void __fastcall TRazbivka::feed_convert(int f,int mode,char *new_feed)
{ int k;
if(f>0)
  {
  k=log10((double)f);
  if(k==1) f=f;
  else if(k==0) f=f*10;
  else if(k>=1 && k<=2) f=f/10;
  else if(k>2 && k<=3) f=f/100;
  sprintf(new_feed,"%1d%1d%2.2d",mode, k+4, f);
  k=0;
  }
  else strcpy(new_feed,"0000");
}







