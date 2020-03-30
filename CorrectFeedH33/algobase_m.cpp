//---------------------------------------------------------------------------

#include <sys\stat.h>
#include <sys\timeb.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <io.h>
#include <dir.h>

/* Microsoft VC++6
#include <types.h>
#include <stat.h>
*/

#define mStr 128
#include "algobase_m.h"

//---------------------------------------------------------------------------

/* This function read the numeric string after key letter
	in success it return 1, otherwice 0
*/
int __fastcall read_word(char *s,char w,char *sw)
{
         char *word,sym;
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


//---------------------------------------------------------------------------

/*# Декодирует подачу Н33 в нормальное число*/
double decodeFeed (double ffeed )  {
/*# ffeed - общий случай , ГОСТ - обозначения*/
  int  fmode,intf ;
  double ffeed_tmp, kf ;
  if (ffeed<0.0) ffeed=0;

  fmode=0;
  if (ffeed>4000) { ffeed-=4000 ; fmode=4 ; }

  ffeed_tmp=ffeed/100.0 ;
  intf=(int)ffeed_tmp ;
  ffeed_tmp-=intf ;
  kf=intf - 3 ;

  ffeed_tmp*=pow(10,kf) ;

  return (ffeed_tmp);
}


/*################################################################################*/
/* Author: Gumen Olga, 7 nov 2003 */
/*
Проверяет - является ли УП - программой для Н33 (Атака)
Параметры :
    namefile - имя файла ,
    codefeed - 0 адреса F - проверять,
               1 адреса F - не проверять
Возвращаемое значение :
    0  -  УП является программой для Н33 (Атака)
    -1 -  Невозможно открыть файл 'чтение'
    n  -  число ошибок -> УП не является программой для Н33 (Атака)
*/
long int program_format_H33(char *namefile, int codefeed = 0)
{
/*локальные переменные*/
 FILE *rf; /*Указатель на файл*/
 long int numbers_of_kadrs;/* Число кадров*/
 long int error;
 char info[mStr*3];
 char curr[mStr], word[mStr];

 /*Открываем файл*/
 rf=fopen(namefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",namefile );
   /*Application->MessageBox(info,"Проверка программы на формат H33 - > ошибка ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

 /*Начальные условия*/
 error=0;
 /*if (codefeed!=1) codefeed=0;*/
 numbers_of_kadrs=0;

 curr[0]='\0'; fgets(curr,mStr,rf);
 while(!feof(rf))
 {

   /*Первичные признаки*/
   if (read_word(curr,'X',word)) {
      if (7!=strlen(word)) error++;
   }
   if (read_word(curr,'Y',word)) {
      if (7!=strlen(word)) error++;
   }
   if (read_word(curr,'Z',word)) {
      if (7!=strlen(word)) error++;
   }
   if (read_word(curr,'I',word)) {
      if (7!=strlen(word)) error++;
   }
   if (read_word(curr,'J',word)) {
      if (7!=strlen(word)) error++;
   }
   if (read_word(curr,'K',word)) {
      if (7!=strlen(word)) error++;
   }

   /*Вспомогательные признаки*/
   if (codefeed==0) {
      if (read_word(curr,'F',word)) {
         if (4!=strlen(word)) error++;
      }
   }

   /* if (read_word(curr,'M',word)) {
      if (3<=strlen(word)) error++;
   }  */
   if (read_word(curr,'L',word)) {
      if (3!=strlen(word)) error++;
   }
   /*if (read_word(curr,'N',word)) {
      if ((3!=strlen(word))||(5!=strlen(word))) error++;
   } */
   if (read_word(curr,'G',word)) {
      if (3<=strlen(word)) error++;
   }

   if (read_word(curr,'D',word)) error++;
   if (read_word(curr,'H',word)) error++;

   /*Вторичные признаки*/
   if (NULL!=strchr(curr,'.')) error++;
   if (NULL!=strchr(curr,';')) error++;
   if (NULL!=strchr(curr,'(')) error++;
   if (NULL!=strchr(curr,')')) error++;
   if (NULL!=strchr(curr,'"')) error++;
   if (NULL!=strchr(curr,'\'')) error++;

   curr[0]='\0'; fgets(curr,mStr,rf);
   numbers_of_kadrs++;
 } // end while

 fclose(rf);

 return(error);
}


/*################################################################################*/
/* Author: Gumen Olga, 17 oct 2004 */
/*
Статистика УП (УЧПУ MTC200 & Alpha & УЧПУ H33)
Замечание :

Параметры :
   namefile - имя файла (исходник),
   rapid_speed - значение ускоренной подачи (мм/мин),
   struct INFO_FILE_STAT *infofile - статистика УП

Возвращаемое значение :

   struct INFO_FILE_STAT {
      double  TIME;
      double  DIST;
      long int numbers_of_kadrs;
      long int numbers_of_kadrs_line;
      long int numbers_of_kadrs_circle;
      double min_feed,max_feed;
      double min_points[3],max_points[3];
      char info[mStr*3];
      short int fl_n33;
      int err;
      double  SUMXYZ[3];
    } ;

    0  -  ошибок нет
    -1 -  Невозможно открыть файл namefile - 'чтение'
*/

long int program_statistics(char *namefile,double rapid_speed, struct INFO_FILE_STAT *infofile)
{
 FILE *rf; /*Указатель на файл*/
 double  TIME;/* Время выполнения программы*/
 double  DIST;/* */
 long int numbers_of_kadrs;/* Число кадров*/
 long int numbers_of_kadrs_line;/* Число кадров с линейными перемещениями*/
 long int numbers_of_kadrs_circle;/* Число кадров с круговыми перемещениями*/
 double min_feed,max_feed;
 double min_points[3],max_points[3];
 char info[mStr*3];
 short int fl_n33;
 int err;
 double  SUMXYZ[3];

//локальные переменные
 struct stat statbuf;
 std::ftime ft;
 char curr[mStr],currtmp[mStr];
 char word[mStr];
 double curr_points[3],prev_points[3];
 double curr_feed,prev_feed,curr_feed_min;
 long int i,j,k ;
 double Gmode_coord, Gmotion ;
 double mv,tmp,gcode;
 char *jpd,*jpdg;
 short int fl_move;

 /*Открываем файл*/
 rf=fopen(namefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"Невозможно открыть файл:\n %s \n на 'чтение' \n",namefile );
   /*Application->MessageBox(info,"Статистика программы - > ошибка ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

 /*Информация о файле*/
 /*1 - Builder C++ v5.0 get information about the file */
 info[0]='\0';
 getftime(fileno(rf), &ft);
 fstat(fileno(rf), &statbuf);
 tmp=statbuf.st_size/1024.0;
 sprintf(info,"Файл: %s \n Дата изменения %u/%u/%u , %u:%u:%u\n Размер: %.2lf Kb ( %ld bytes )\n Время последнего открытия:\n\t\t %s ", \
                 namefile, \
                 ft.ft_month, ft.ft_day,ft.ft_year+1980, \
                 ft.ft_hour, ft.ft_min,ft.ft_tsec * 2, \
                 tmp, statbuf.st_size, \
                 ctime(&statbuf.st_ctime));

 /*2 - Microsoft VC++ v6.0 get information about the file */
//  struct __stat64 buf;
//  int fh, result;
//   if( (fh = _open( namefile, _O_RDONLY )) ==  -1 ) return(-1);
//   /* Get data associated with "fh": */
//   result = _fstat64( fh, &buf );
//   info[0]='\0';
//   /* Check if statistics are valid: */
/*   if( result != 0 ) {
       sprintf(info,"ERROR! \n  Файл: %s \n\t Bad file handle\n ",namefile );
       return(-1);
     }
     else
     {
       tmp=buf.st_size/1024.0;
       sprintf(info,"Файл: %s \n Размер: %.2lf Kb ( %ld bytes )\n Время последнего открытия:\n\t\t %s ", \
                 namefile, \
                 tmp, buf.st_size, \
                 _ctime64( &buf.st_ctime ));
     }
   _close( fh );
*/

/* Application->MessageBox(info,"Информация о файле", MB_OK); */

 /*Начальные условия*/
 TIME=0.0;
 DIST=0.0;
 numbers_of_kadrs=0;
 numbers_of_kadrs_line=0;
 numbers_of_kadrs_circle=0;
 min_feed=rapid_speed;
 max_feed=-rapid_speed;
 for(i=0;i<3;i++)
 {
   min_points[i]=999999.999;
   max_points[i]=-999999.999;
   SUMXYZ[i]=0;
   prev_points[i]=0;
   curr_points[i]=0;
 }
 curr_feed=-rapid_speed;
 curr_feed_min=rapid_speed;
 prev_feed=-rapid_speed;
 Gmode_coord=90;
 Gmotion=0;
 fl_move=0; /*флаг движения*/
 fl_n33=0;  /*признак формата Н33*/

 err=program_format_H33(namefile,0);
 if (err==0) {
      Gmode_coord=91;
      fl_n33=1;
 }


   /*Обнуляем структуру*/
   infofile->TIME=TIME;
   infofile->DIST=DIST;
   infofile->numbers_of_kadrs=numbers_of_kadrs;
   infofile->numbers_of_kadrs_line=numbers_of_kadrs_line;
   infofile->numbers_of_kadrs_circle=numbers_of_kadrs_circle;
   infofile->min_feed=min_feed;
   infofile->max_feed=max_feed;
   for(i=0;i<3;i++)
   {
      infofile->min_points[i]=min_points[i];
      infofile->max_points[i]=max_points[i];
      infofile->SUMXYZ[i]=SUMXYZ[i];
   }
   infofile->info[0]='\0';
   infofile->fl_n33=fl_n33;
   infofile->err=err;


 curr[0]='\0'; fgets(curr,mStr,rf);
 while(!feof(rf))
 {

   /*Кадры с G - взято из проекта Symmetry ( блок trans)*/
   if (read_word(curr,'G',word))  {
     currtmp[0]='\0';strcpy(currtmp,curr);
     jpd=strchr(currtmp,'G');
     if(jpd!=NULL) {
       while(strcmp(jpd,""))
       {
         if (read_word(jpd,'G',word))
         { sscanf(word,"%lf",&gcode);
           /*printf(" Gf==%f Gs=%s |" ,gcode,word);*/
	   if (gcode==0) {
	       curr_feed= rapid_speed ;
	       Gmotion=gcode;
	   }
	   if (gcode==1) {
	       Gmotion=gcode;
	   }
	   if ((gcode==2)||(gcode==3)) {
	       Gmotion=gcode;
	   }
	   if ((gcode==90)||(gcode==91)) {
	       Gmode_coord=gcode;
	   }
         }
         jpdg=strchr(jpd,'G');
         if(jpdg==NULL) strcpy(jpd,"");else strcpy(jpd,jpdg+1);
       }
     }

   }

   if (Gmode_coord==91) {
       for(i=0;i<3;i++) curr_points[i]=0.0;
   }

   fl_move=0;

   if (read_word(curr,'X',word)) { sscanf(word,"%lf",&curr_points[0]);fl_move++; }
   if (read_word(curr,'Y',word)) { sscanf(word,"%lf",&curr_points[1]);fl_move++; }
   if (read_word(curr,'Z',word)) { sscanf(word,"%lf",&curr_points[2]);fl_move++; }
   if (read_word(curr,'F',word)) {
      sscanf(word,"%lf",&curr_feed);
      if (fl_n33!=0) {
         curr_feed=decodeFeed(curr_feed) ;
      }
      if (curr_feed<0.01) { curr_feed= prev_feed ; }
      if (curr_feed<0.01) { curr_feed= rapid_speed ; }
   }

   if (fl_move!=0) {
   	if ((Gmotion==0)||(Gmotion==1)) { numbers_of_kadrs_line++ ; }
   	if ((Gmotion==2)||(Gmotion==3)) { numbers_of_kadrs_circle++ ; }
   }

   if (Gmode_coord==91) {
       for(i=0;i<3;i++) {
         SUMXYZ[i]+=curr_points[i];
         curr_points[i]+=prev_points[i];
       }
   }

   mv=0.0; for(i=0;i<3;i++) mv+=pow(curr_points[i]-prev_points[i],2);
   DIST+=sqrt(mv);
   if (curr_feed>=0.1) TIME+=sqrt(mv)/curr_feed;

   /*Анализ на экстремум 1*/
   for(i=0;i<3;i++) {
      if (curr_points[i]<=min_points[i]) { min_points[i]=curr_points[i]; }
      if (curr_points[i]>=max_points[i]) { max_points[i]=curr_points[i]; }
   }

   /*Анализ на экстремум 2*/
   curr_feed_min=-curr_feed;
   if (curr_feed_min>=min_feed) { min_feed=curr_feed_min ; }
   if (curr_feed<=min_feed) { min_feed=curr_feed ; }
   if (curr_feed>=max_feed) { max_feed=curr_feed ; }

   prev_feed=curr_feed;
   for(i=0;i<3;i++) prev_points[i]=curr_points[i];

   curr[0]='\0'; fgets(curr,mStr,rf);
   numbers_of_kadrs++;
 } // end while

 min_feed=fabs(min_feed);

 fclose(rf);

 /* параметры вывода */
 if (fl_n33!=0) {
    DIST/=100.0;
    for(i=0;i<3;i++) {
      min_points[i]/=100.0;
      max_points[i]/=100.0;
      SUMXYZ[i]/=100.0;
    }
    /* Время выполнения*/
    TIME=-1.0;
 }

   /*Заполняем структуру*/
   infofile->TIME=TIME;
   infofile->DIST=DIST;
   infofile->numbers_of_kadrs=numbers_of_kadrs;
   infofile->numbers_of_kadrs_line=numbers_of_kadrs_line;
   infofile->numbers_of_kadrs_circle=numbers_of_kadrs_circle;
   infofile->min_feed=min_feed;
   infofile->max_feed=max_feed;
   for(i=0;i<3;i++)
   {
      infofile->min_points[i]=min_points[i];
      infofile->max_points[i]=max_points[i];
      infofile->SUMXYZ[i]=SUMXYZ[i];
   }
   strcpy(infofile->info,info);
   infofile->fl_n33=fl_n33;
   infofile->err=err;

 return(0);
}

/***********************************************************************/

