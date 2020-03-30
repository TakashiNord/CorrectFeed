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

/*# ���������� ������ �33 � ���������� �����*/
double decodeFeed (double ffeed )  {
/*# ffeed - ����� ������ , ���� - �����������*/
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
��������� - �������� �� �� - ���������� ��� �33 (�����)
��������� :
    namefile - ��� ����� ,
    codefeed - 0 ������ F - ���������,
               1 ������ F - �� ���������
������������ �������� :
    0  -  �� �������� ���������� ��� �33 (�����)
    -1 -  ���������� ������� ���� '������'
    n  -  ����� ������ -> �� �� �������� ���������� ��� �33 (�����)
*/
long int program_format_H33(char *namefile, int codefeed = 0)
{
/*��������� ����������*/
 FILE *rf; /*��������� �� ����*/
 long int numbers_of_kadrs;/* ����� ������*/
 long int error;
 char info[mStr*3];
 char curr[mStr], word[mStr];

 /*��������� ����*/
 rf=fopen(namefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",namefile );
   /*Application->MessageBox(info,"�������� ��������� �� ������ H33 - > ������ ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

 /*��������� �������*/
 error=0;
 /*if (codefeed!=1) codefeed=0;*/
 numbers_of_kadrs=0;

 curr[0]='\0'; fgets(curr,mStr,rf);
 while(!feof(rf))
 {

   /*��������� ��������*/
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

   /*��������������� ��������*/
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

   /*��������� ��������*/
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
���������� �� (���� MTC200 & Alpha & ���� H33)
��������� :

��������� :
   namefile - ��� ����� (��������),
   rapid_speed - �������� ���������� ������ (��/���),
   struct INFO_FILE_STAT *infofile - ���������� ��

������������ �������� :

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

    0  -  ������ ���
    -1 -  ���������� ������� ���� namefile - '������'
*/

long int program_statistics(char *namefile,double rapid_speed, struct INFO_FILE_STAT *infofile)
{
 FILE *rf; /*��������� �� ����*/
 double  TIME;/* ����� ���������� ���������*/
 double  DIST;/* */
 long int numbers_of_kadrs;/* ����� ������*/
 long int numbers_of_kadrs_line;/* ����� ������ � ��������� �������������*/
 long int numbers_of_kadrs_circle;/* ����� ������ � ��������� �������������*/
 double min_feed,max_feed;
 double min_points[3],max_points[3];
 char info[mStr*3];
 short int fl_n33;
 int err;
 double  SUMXYZ[3];

//��������� ����������
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

 /*��������� ����*/
 rf=fopen(namefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",namefile );
   /*Application->MessageBox(info,"���������� ��������� - > ������ ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

 /*���������� � �����*/
 /*1 - Builder C++ v5.0 get information about the file */
 info[0]='\0';
 getftime(fileno(rf), &ft);
 fstat(fileno(rf), &statbuf);
 tmp=statbuf.st_size/1024.0;
 sprintf(info,"����: %s \n ���� ��������� %u/%u/%u , %u:%u:%u\n ������: %.2lf Kb ( %ld bytes )\n ����� ���������� ��������:\n\t\t %s ", \
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
       sprintf(info,"ERROR! \n  ����: %s \n\t Bad file handle\n ",namefile );
       return(-1);
     }
     else
     {
       tmp=buf.st_size/1024.0;
       sprintf(info,"����: %s \n ������: %.2lf Kb ( %ld bytes )\n ����� ���������� ��������:\n\t\t %s ", \
                 namefile, \
                 tmp, buf.st_size, \
                 _ctime64( &buf.st_ctime ));
     }
   _close( fh );
*/

/* Application->MessageBox(info,"���������� � �����", MB_OK); */

 /*��������� �������*/
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
 fl_move=0; /*���� ��������*/
 fl_n33=0;  /*������� ������� �33*/

 err=program_format_H33(namefile,0);
 if (err==0) {
      Gmode_coord=91;
      fl_n33=1;
 }


   /*�������� ���������*/
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

   /*����� � G - ����� �� ������� Symmetry ( ���� trans)*/
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

   /*������ �� ��������� 1*/
   for(i=0;i<3;i++) {
      if (curr_points[i]<=min_points[i]) { min_points[i]=curr_points[i]; }
      if (curr_points[i]>=max_points[i]) { max_points[i]=curr_points[i]; }
   }

   /*������ �� ��������� 2*/
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

 /* ��������� ������ */
 if (fl_n33!=0) {
    DIST/=100.0;
    for(i=0;i<3;i++) {
      min_points[i]/=100.0;
      max_points[i]/=100.0;
      SUMXYZ[i]/=100.0;
    }
    /* ����� ����������*/
    TIME=-1.0;
 }

   /*��������� ���������*/
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

