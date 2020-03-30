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
#include "algobase.h"

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

/* This function replace the numeric string after key letter
	if there isn't a key letter in a string funktion adds it
	at the end of a string
*/
void __fastcall replace_word(char *s,char w,char *sw)
{
  int n;
  char old_sw[20],save[100],*p;

  if(s==NULL) return;

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
		save[0]=' ';save[1]=w; save[2]='\n'; save[3]='\0'; strcat(strcat(s,save),sw); }
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

/*�������� �����*/
int delete_word(char *s,char w)
{
 int n;
 char old_sw[mStr],save[mStr],*p;

 if(s==NULL) return (0);

 n=0;
 if ((n=read_word(s,w,old_sw))!=0)
  {
	 /* replace word */
	 p=strchr(s,w);
	 *(p+0)='\0'; strcpy(save,p+n+1);
	 strcat(s,save);
  }

 return n;
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
/* Author: Gumen Olga, 12 dec 2003 */
/*
��������� ����� ����� ��
��������� :
��������� :
    namefile - ��� ����� ,
������������ �������� :
    num  -  ����� ����� ��
    -1 -  ���������� ������� ���� '������'
*/

long int program_list(char *namefile)
{
/*��������� ����������*/
 FILE *rf; /*��������� �� ����*/
 long int numbers_of_kadrs;/* ����� ������*/
 long int error;
 char info[mStr*3];
 char curr[mStr];

 /*��������� ����*/
 rf=fopen(namefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",namefile );
   /*Application->MessageBox(info,"�������� ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

 /*��������� �������*/
 error=0;
 curr[0]='\0'; fgets(curr,mStr,rf);
 numbers_of_kadrs=1;
 while(!feof(rf))
 {
   curr[0]='\0'; fgets(curr,mStr,rf);
   numbers_of_kadrs++;
 } // end while

 fclose(rf);

 return(numbers_of_kadrs);
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
/* Author: Gumen Olga, 8 dec 2003 */
/*
��������� ����� ���������� �� (���� H33)
��������� :
    ������ ���������� ���������� �5 ��� (��� ����� �������-����������)
    ���� � �� ����� ����� �������� �� ����������� - ������ ����������
��������� :
    namefile - ��� ����� ,
������������ �������� :
    t  -  ����� ���������� ��
    -1 -  ���������� ������� ���� '������'
*/
double program_time_H33(char *namefile)
{
 double  TIME;/* ����� ���������� ���������*/
/*��������� ����������*/
 FILE *rf; /*��������� �� ����*/
 char info[mStr*3];
 char curr[mStr];
 char word[mStr];
 double curr_points[3];
 double curr_feed;
 long int i;
 double mv;
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

 /*��������� �������*/
 TIME=0.0;
 curr_feed=20;

 curr[0]='\0'; fgets(curr,mStr,rf);
 while(!feof(rf))
 {

   for(i=0;i<3;i++) curr_points[i]=0.0;

   fl_move=0;/*���� ��������*/

   if (read_word(curr,'X',word)) { sscanf(word,"%lf",&curr_points[0]);fl_move++; }
   if (read_word(curr,'Y',word)) { sscanf(word,"%lf",&curr_points[1]);fl_move++; }
   if (read_word(curr,'Z',word)) { sscanf(word,"%lf",&curr_points[2]);fl_move++; }
   if (read_word(curr,'F',word)) {
      sscanf(word,"%lf",&curr_feed);
      curr_feed=decodeFeed(curr_feed);
   }

   mv=0.0; for(i=0;i<3;i++) { curr_points[i]/=100.0 ; mv+=pow(curr_points[i],2); }
   if (curr_feed>=0.1) TIME+=sqrt(mv)/curr_feed;

   curr[0]='\0'; fgets(curr,mStr,rf);
 } // end while

 fclose(rf);

 /* ��������� ������ */

 return(TIME);
}


/*################################################################################*/
/* Author: Gumen Olga, 12 dec 2003 */
/*
��������� ����� ���������� �� (���� MTC200)
��������� :
    ������ ���������� ���������� �2 ���
    ���� � �� ����� ����� �������� �� ����������� - ������ ����������
��������� :
    namefile - ��� ����� ,
    rapid_speed - �������� ���������� ������ (��/���).
������������ �������� :
    t  -  ����� ���������� ��
    -1 -  ���������� ������� ���� '������'
*/
double program_time(char *namefile,double rapid_speed)
{
 double  TIME;/* ����� ���������� ���������*/
/*��������� ����������*/
 FILE *rf; /*��������� �� ����*/
 char info[mStr*3];
 int err;
 char curr[mStr],currtmp[mStr];
 char word[mStr];
 double curr_points[3],prev_points[3];
 double curr_feed,prev_feed;
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

 /*��������� �������*/
 TIME=0.0;
 for(i=0;i<3;i++)
 {
   prev_points[i]=0;
   curr_points[i]=0;
 }
 curr_feed=-rapid_speed;
 prev_feed=-rapid_speed;
 Gmode_coord=90;
 Gmotion=0;
 fl_move=0; /*���� ��������*/

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
      if (curr_feed<0.1) { curr_feed= prev_feed ; }
      if (curr_feed<0.1) { curr_feed= rapid_speed ; }
   }

   if (Gmode_coord==91) {
       for(i=0;i<3;i++) {
         curr_points[i]+=prev_points[i];
       }
   }

   mv=0.0; for(i=0;i<3;i++) mv+=pow(curr_points[i]-prev_points[i],2);
   if (curr_feed>=0.1) TIME+=sqrt(mv)/curr_feed;

   prev_feed=curr_feed;
   for(i=0;i<3;i++) prev_points[i]=curr_points[i];

   curr[0]='\0'; fgets(curr,mStr,rf);
 } // end while

 fclose(rf);

 /* ��������� ������ */

 return(TIME);
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
    TIME=program_time_H33(namefile);

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


/*################################################################################*/
/* Author: Gumen Olga, 18 oct 2004 */
/*
���������� �� ����������� � ����
��������� :

��������� :
   namefile - ��� �����,
   struct INFO_FILE_STAT *infofile - ���������� ��

������������ �������� :
    0  -  ������ ���
    -1 -  ���������� ������� ���� namefile �� - '����������'
*/
int program_statistics_save(char *namefile, struct INFO_FILE_STAT infofile)
{
//��������� ����������
 FILE *rf; /*��������� �� ����*/
 int i;
 char info[mStr*3];

 /*��������� ����*/
 rf=fopen(namefile,"a");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '����������' \n",namefile );
   /*Application->MessageBox(info,"���������� ��������� - > ������ ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

  /*��������� ���������*/
  fprintf(rf,"==================================================\n");
  fprintf(rf,"%s",infofile.info);
  if (infofile.fl_n33==0)
     fprintf(rf,"��� ����� = (%ld) \n",infofile.fl_n33);
    else
     fprintf(rf,"��� ����� = (%ld) - �33 (Ataka) \n",infofile.fl_n33);
  fprintf(rf,"����� ���������� (���)= %.1lf \n",infofile.TIME);
  fprintf(rf,"���������� ���� (��)= %.2lf \n",infofile.DIST);
  fprintf(rf,"����� ������= %ld \n",infofile.numbers_of_kadrs);
  fprintf(rf,"����� ������ � �������� �������������= %ld \n",infofile.numbers_of_kadrs_line);
  fprintf(rf,"����� ������ � �������� �������������= %ld \n",infofile.numbers_of_kadrs_circle);
  fprintf(rf,"������: \n");
  fprintf(rf," min ������ (��/���) = %.2lf \n",infofile.min_feed);
  fprintf(rf," max ������ (��/���) = %.2lf \n",infofile.max_feed);
  fprintf(rf,"���������� �����������:\n");
  fprintf(rf," min: (��) \n\t");
  for(i=0;i<3;i++)
  {
      fprintf(rf," %10.3lf ",infofile.min_points[i]);
   }
  fprintf(rf,"\n max: (��)\n\t");
  for(i=0;i<3;i++)
  {
      fprintf(rf," %10.3lf ",infofile.max_points[i]);
   }
  fprintf(rf,"\n����������� �����:\n");
  fprintf(rf," SUM : (��)\n\t");
  for(i=0;i<3;i++)
  {
      fprintf(rf," %10.2lf ",infofile.SUMXYZ[i]);
   }
  fprintf(rf,"\n����� ������ = %ld \n",infofile.err);
  fprintf(rf,"==================================================\n");

  fclose(rf);

  return (0);
}


/*################################################################################*/
/* Author: Gumen Olga, 10 dec 2004 */
/*
������������ �� - ������� ��� �������� F
��������� :

��������� :
    sourcefile - ��� ����� (��������),
    destfile - ��� ����� (����������������)
    type = 0 - ��� ��������������,
           1 - �������� ���� F,
           2 - �������� ���� F:F>maxfeed ,
           3 - �������� ���� F:F<minfeed ,
           4 - �������� ���� F: minfeed<=F<=maxfeed  ,
           5 - �������������� ���� F �� �����.,
    minfeed - min ��������
    maxfeed - max ��������.
������������ �������� :
     n -  ��� ��������� (����� ������������� ��������)
    -1 -  ���������� ������� ���� sourcefile - '������'
    -2 -  ���������� ������� ���� destfile - '������'
    -3 -  ����������� ���������� �������
*/
long int program_correct_feed(char *sourcefile,char *destfile,int type, double minfeed, double maxfeed, double coef)
{
 /*��������� ����������*/
 FILE *rf,*ff; /*��������� �� ����*/
 char info[mStr*3];
 int err;
 char curr[mStr], word[mStr], currtmp[mStr];
 double feed;
 double tmp;
 long int numbers_of_kadrs ;

 /*��������� ���� �� ������*/
 rf=fopen(sourcefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",sourcefile );
   /*Application->MessageBox(info,"��������� ������ - > ������ ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

 /*��������� ���� �� ������*/
 ff=fopen(destfile,"w");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",destfile );
   /*Application->MessageBox(info,"��������� ������ - > ������ ...", MB_OK);*/
   fprintf(stderr, info);
   return(-2);
  }

 /*�������� ���������� ��������*/
   switch (type) {
    case 0 : /*- ��� ��������������*/
    break;
    case 1 : /*- �������� ���� F*/
    break;
    case 2 : /*- �������� ���� F:F>maxfeed*/
      if (maxfeed<0) return (-3);
    break;
    case 3 : /*- �������� ���� F:F<minfeed*/
      if (minfeed<0) return (-3);
    break;
    case 4 : /*- �������� ���� F: minfeed<=F<=maxfeed*/
      if (maxfeed<=0) return (-3);
      if (minfeed<0) return (-3);
      if (minfeed>maxfeed) return (-3);
    break;
    case 5 : /*- �������������� ���� F �� �����.*/
    break;
    default :
    break;
   }

 /*��������� �������*/
 feed=0;
 numbers_of_kadrs=0;

 currtmp[0]='\0';
 curr[0]='\0'; fgets(curr,mStr,rf);
 while(!feof(rf))
 {

   switch (type) {
    case 0 : /*- ��� ��������������*/
    break;
    case 1 : /*- �������� ���� F*/
       if (read_word(curr,'F',word)) { delete_word(curr,'F'); numbers_of_kadrs++ ; }
    break;
    case 2 : /*- �������� ���� F:F>maxfeed*/
       if (read_word(curr,'F',word)) {
          sscanf(word,"%lf",&feed);
          if (feed>maxfeed) { delete_word(curr,'F');numbers_of_kadrs++ ; }
       }
    break;
    case 3 : /*- �������� ���� F:F<minfeed*/
       if (read_word(curr,'F',word)) {
          sscanf(word,"%lf",&feed);
          if (feed<minfeed) { delete_word(curr,'F'); numbers_of_kadrs++ ; }
       }
    break;
    case 4 : /*- �������� ���� F: minfeed<=F<=maxfeed*/
       if (read_word(curr,'F',word)) {
          sscanf(word,"%lf",&feed);
          if ((feed>=minfeed)&&(feed<=maxfeed)) { delete_word(curr,'F'); numbers_of_kadrs++ ; }
       }

    break;
    case 5 : /*- �������������� ���� F �� �����.*/
       if (read_word(curr,'F',word)) {
          sscanf(word,"%lf",&feed);
          tmp=feed/100.0*coef; /* 1% */
          feed=feed+tmp;
          word[0]='\0'; sprintf(word,"%.1lf",feed);
          replace_word(curr,'F',word) ;
          numbers_of_kadrs++ ;
       }
    break;
    default :
    break;
   }

   fputs(curr,ff);
   curr[0]='\0'; fgets(curr,mStr,rf);
 } // end while

 fclose(rf);

 fputs(curr,ff);
 fclose(ff);

 return(numbers_of_kadrs);
}








/***********************************************************************/
int  EQ_is_zero (double s)
{
  double mom_system_tolerance=0.00000001 ;
  if (fabs(s)<= mom_system_tolerance) { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_equal (double s, double t)
{
	double mom_system_tolerance=0.00000001 ;
	if (fabs(s-t)<= mom_system_tolerance) { return(1); } else { return(0) ; }
}
/***********************************************************************/
int  EQ_is_gt (double s, double t)
{
         double mom_system_tolerance=0.00000001 ;
         if (s > (t + mom_system_tolerance)) { return(1); } else { return(0) ; }
}
/***********************************************************************/
//  VEC3_dot(u,v)                    (u dot v)          Vector dot product
double VEC3_dot (double u[ 3 ],double v[ 3 ])
{
 return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]) ;
}
/***********************************************************************/
//  VEC3_mag(u)                      ( || u || )        Vector magnitude
double VEC3_mag (double u[ 3 ])
{
  return (sqrt(VEC3_dot(u,u)));
}
/***********************************************************************/
//  VEC3_is_zero(u,tol)              (|| u || < tol)    Is vector zero?
int VEC3_is_zero (double u[ 3 ])
{
 double v[3]={0.0,0.0,0.0};
 int is_equal=1;
 for(int ii=0;ii < 3;ii++) { if (!EQ_is_equal(u[ii],v[ii])) { is_equal=0; break; } }
 return is_equal ;
}
/***********************************************************************/
/*=============================================================*/
int VEC_overfall_feed ( double v1[ 3 ], double v2[ 3 ],  double vMax[ 3 ] ) {
// �������� ���������� �������� �����
/*=============================================================*/
 int i , flf ; double df ;
 flf= 0 ;//# ������� ��������� �������� ������
 for(i=0;i<3;i++) {
  df=fabs( v2[i] - v1[i] ) ;
  //df=fabs( fabs(v2[i]) - fabs(v1[i]) ) ;
  if (EQ_is_gt(df,vMax[i])) { flf=1 ; break ; }
 }
 fprintf(stderr, "������� ��������� �������� ������ flf=%d\n",flf);
 return(flf);
}
/***********************************************************************/
/***********************************************************************/


/*################################################################################*/

/* Author: Gumen Olga, 8 jul 2004 */
/*
������������ �� (���� MTC200)
��������� :
  ************************  ��  ���������� �� ������ ****************************
��������� :
    sourcefile - ��� ����� (��������),
    destfile - ��� ����� (����������������)
    rapid_speed - �������� ���������� ������ (��/���),
    acc - ��������� (%).
������������ �������� :
    n  -  ����� ��������������� ������
    -1 -  ���������� ������� ���� sourcefile - '������'
    -2 -  ���������� ������� ���� destfile - '������'
*/
long int program_correct(char *sourcefile,char *destfile,double rapid_speed,double acc)
{

/* ; # �������� ���������� ��� ����������� ����������*/
 double accel;/*# ���������� ��������� ���������� - ��������� mm/(min*min)*/
 accel=1000.0/100.0*acc ; /*# ���������� ��������� mm/(sec*sec)*/
 double MAXFEED[3] ; /*#  ������������ ������� ����� �� ���� (��/���)*/
 MAXFEED[0]= 1700.0 ;/* # ��� X = 1700.0   100.0   300.0*/
 MAXFEED[1]= 1700.0 ; /*# ��� Y = 1700.0   100.0   300.0*/
 MAXFEED[2]= 200.0  ; /*# ��� Z = 200.0    100.0   300.0*/

 double timeEvent;/* #  ������������ ����� ��������� � ���������� ����� (���)*/
 /* # 2mc=0.002c=0.000034min - min ����� ��������� ����� ����;
  # 20mc=0.02c=0.00034min - ���������� ��������� ;
  # 0.003c=0.00005min; 0.005c=0.0000833min; 0.006c=0.0001min; 0.12c=0.002min*/
 timeEvent=0.0007 ;/* # ������-> 0.0004=������(0.00034min)  0.0007=������(2*0.00034min)*/

 FILE *rf,*ff; /*��������� �� ����*/
 long int error;
 char info[mStr*3];

 double mom_motion_distance ;
 double mom_event_time ;
 double feed ;
 long int level ;
 long int numbers_of_kadrs;

//��������� ����������
 char curr[mStr],currtmp[mStr],next[mStr];
 char word[mStr]; char *p ;
 double next_points[6],curr_points[6],prev_points[6];
 double vec1[3], vec2[3], vec1f[3], vec2f[3] ;
 double angle ;  double len1, len2 , scal;
 double x_feed, feed_old, prev_feed, next_feed ;
 short int small_move ;
 double Fcur, Ft, St, vp ;
 double feed_optimal ; short int flag_optimal ;  double Rmax ;  double R[3] ;
 long int i,j,k ;
 double Gmode_coord, Gmotion , Gmotion_next, Gmode_coord_next ;
 double tmp,gcode;
 char *jpd,*jpdg;
 short int fl_move, fl_move_Z, fl_move_next;
 //fpos_t filepos;
 double tmp1, tmp2, tmp3;
 double feed_Z;

 /*��������� ���� �� ������*/
 rf=fopen(sourcefile,"r");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",sourcefile );
   /*Application->MessageBox(info,"��������� ��������� - > ������ ...", MB_OK);*/
   fprintf(stderr, info);
   return(-1);
  }

 /*��������� ���� �� ������*/
 ff=fopen(destfile,"w");
 if (rf==NULL) {
   info[0]='\0';
   sprintf(info,"���������� ������� ����:\n %s \n �� '������' \n",destfile );
   /*Application->MessageBox(info,"��������� ��������� - > ������ ...", MB_OK);*/
   fprintf(stderr, info);
   return(-2);
  }

 /*��������� �������*/

   /* mathl *mlib=new mathl();*/
 numbers_of_kadrs=0 ;
 mom_motion_distance=0.0;
 mom_event_time=0.0;
 error=0;
 for(i=0;i<6;i++)
 {
   prev_points[i]=0;
   curr_points[i]=0;
   next_points[i]=0;
 }
 feed=-rapid_speed;
 prev_feed=-rapid_speed;
 next_feed= 0.0 ;
 Gmode_coord=90;  Gmode_coord_next=90;
 Gmotion=0;    Gmotion_next=0;
 fl_move=0; /*���� ��������*/
 fl_move_next= 0;
 fl_move_Z=0; /*���� �������� �� ���������� Z*/
 small_move=0;
 angle= 0.0 ;
 for(i=0;i<3;i++) { vec1[i]=0.0;   vec2[i]=0.0;   vec1f[i]=0.0;   vec2f[i]=0.0; }

 curr[0]='\0'; fgets(curr,mStr,rf);
 next[0]='\0'; fgets(next,mStr,rf);
 /* save the file pointer position */
 //fgetpos(rf, &filepos);//fsetpos(rf, &filepos);

 while(!feof(rf))
 {

   /*����� � G - ����� �� ������� Symmetry ( ���� trans)*/
   if (read_word(curr,'G',word))  {
     currtmp[0]='\0'; strcpy(currtmp,curr);
     jpd=strchr(currtmp,'G');
     if(jpd!=NULL) {
       while(strcmp(jpd,""))
       {
         if (read_word(jpd,'G',word))
         { sscanf(word,"%lf",&gcode);
           /*printf(" Gf==%f Gs=%s |" ,gcode,word);*/
	   if (gcode==0) {
	       feed= rapid_speed ;
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
   fl_move_Z=0;

   if (read_word(curr,'X',word)) { sscanf(word,"%lf",&curr_points[0]);fl_move++; }
   if (read_word(curr,'Y',word)) { sscanf(word,"%lf",&curr_points[1]);fl_move++; }
   if (read_word(curr,'Z',word)) {
      sscanf(word,"%lf",&curr_points[2]);
      fl_move++;
      fl_move_Z++;
   }
   if (read_word(curr,'I',word)) { sscanf(word,"%lf",&curr_points[3]);fl_move++; }
   if (read_word(curr,'J',word)) { sscanf(word,"%lf",&curr_points[4]);fl_move++; }
   if (read_word(curr,'K',word)) { sscanf(word,"%lf",&curr_points[5]);fl_move++; }
   if (read_word(curr,'F',word)) {
      sscanf(word,"%lf",&feed);
      if (feed<0.1) { feed= prev_feed ; }
      if (feed<0.1) { feed= rapid_speed ; }
   }
   if (fl_move_Z!=0) {
   	feed_Z=feed;
   }

   if (fl_move!=0) {
   	if (Gmotion==0) fl_move=0 ;
        if (Gmotion==1) { ; }
   	if ((Gmotion==2)||(Gmotion==3)) { ; }
   }

   if (Gmode_coord==91) {
       for(i=0;i<3;i++) {
         curr_points[i]+=prev_points[i];
       }
   }

/***********************************************************************/
   next_feed = feed  ;
   Gmode_coord_next = Gmode_coord ;

   /*����� � G - ����� �� ������� Symmetry ( ���� trans)*/
   if (read_word(next,'G',word))  {
     currtmp[0]='\0'; strcpy(currtmp,next);
     jpd=strchr(currtmp,'G');
     if(jpd!=NULL) {
       while(strcmp(jpd,""))
       {
         if (read_word(jpd,'G',word))
         { sscanf(word,"%lf",&gcode);
           /*printf(" Gf==%f Gs=%s |" ,gcode,word);*/
	   if (gcode==0) {
	       next_feed= rapid_speed ;
	       Gmotion_next=gcode;
	   }
	   if (gcode==1) {
	       Gmotion_next=gcode;
	   }
	   if ((gcode==2)||(gcode==3)) {
	       Gmotion_next=gcode;
	   }
	   if ((gcode==90)||(gcode==91)) {
	       Gmode_coord_next=gcode;
	   }
         }
         jpdg=strchr(jpd,'G');
         if(jpdg==NULL) strcpy(jpd,"");else strcpy(jpd,jpdg+1);
       }
     }

   }

   /*!!!!*/
   for(i=0;i<3;i++) next_points[i]=0.0;
   /*!!!!*/
   fl_move_next=0;

   if (read_word(next,'X',word)) { sscanf(word,"%lf",&next_points[0]);fl_move_next++; }
   if (read_word(next,'Y',word)) { sscanf(word,"%lf",&next_points[1]);fl_move_next++; }
   if (read_word(next,'Z',word)) { sscanf(word,"%lf",&next_points[2]);fl_move_next++; }
   if (read_word(next,'I',word)) { sscanf(word,"%lf",&next_points[3]);fl_move_next++; }
   if (read_word(next,'J',word)) { sscanf(word,"%lf",&next_points[4]);fl_move_next++; }
   if (read_word(next,'K',word)) { sscanf(word,"%lf",&next_points[5]);fl_move_next++; }
   if (read_word(next,'F',word)) {
      sscanf(word,"%lf",&next_feed);
      if (next_feed<0.1) { next_feed= feed ; }
      if (next_feed<0.1) { next_feed= rapid_speed ; }
   }

   if (Gmode_coord_next==91) {
       for(i=0;i<3;i++) {
         next_points[i]+=curr_points[i];
       }
   }

   /*!!!!*/
   if (fl_move_next==0) {
      for(i=0;i<6;i++) {
         next_points[i]=curr_points[i];
         next_feed= feed  ;
       }
   }
   /*!!!!*/

    /***********************************************************************/
      /*!!!!*/
      /*!!������ ��������!!*/
      len1=0;  len2=0;
      for(i=0;i<3;i++) {
        vec1[i]=curr_points[i]-prev_points[i];
        len1+=pow(vec1[i],2);
        vec2[i]=next_points[i]-curr_points[i];
        len2+=pow(vec2[i],2);
      }
      len1=sqrt(len1);
      len2=sqrt(len2);
      /*!!������������!!*/
      for(i=0;i<3;i++) {
        if (!EQ_is_zero(len1)) vec1[i]/=len1 ;
        if (!EQ_is_zero(len2)) vec2[i]/=len2 ;
      }
      /*!!������������!!*/
      for(i=0;i<3;i++) {
        vec1f[i]=vec1[i]*feed ;
        vec2f[i]=vec2[i]*next_feed ;
      }
      /*!!!!*/
    /***********************************************************************/
    /*������ ����*/
    angle= 0.0 ;
    if (!VEC3_is_zero( vec1 ) && !VEC3_is_zero( vec2 )) { 
        len1=VEC3_mag(vec1) ;
        len2=VEC3_mag(vec2) ;
        scal=VEC3_dot(vec1, vec2) ;
        tmp3=scal/(len1*len2) ;
        if (tmp3>1.0) tmp3=1.0 ;
        if (tmp3<-1.0) tmp3=-1.0 ;
        fprintf(stderr, "cos(angle) =%.4f\n",tmp3);
        angle=(90.0 / asin(1.0))*acos(tmp3) ;
        if (EQ_is_zero(angle)) { angle=0.0 ; }
     }
    /*!!!!*/
    /***********************************************************************/

   mom_event_time=0.0;
   mom_motion_distance=0.0; for(i=0;i<3;i++) mom_motion_distance+=pow(curr_points[i]-prev_points[i],2);
   mom_motion_distance=sqrt(mom_motion_distance);
   if (feed>=0.1) mom_event_time=mom_motion_distance/feed;

   /*���� � ����� - ���� �������� X? Y? Z? I? J? K?*/
   x_feed=feed;
   small_move=0;

   if (fl_move_Z!=0) {
   	if (feed>5000) feed=5000;
   }

   if (fl_move==1) {

     /*  ;#2.0  ��������� ���� �� ������� ��������� �� ���. ����� �����  � ���������� */
     if (mom_event_time<timeEvent) {
       mom_event_time=timeEvent ;
       tmp=mom_motion_distance/mom_event_time ;
       if (feed>tmp) {
         if (tmp<=10.0) {
            small_move=1;
            /*
            �������� � ���� ����� ���������� �� ������� ��������� ������
      	    */
         }
         feed=tmp ;
         mom_event_time=mom_motion_distance/feed ;
         level++ ;
       }
     }

     /*      #1.0 */
     if (feed==0.0) feed=rapid_speed ;
     Fcur=feed/60.0 ; /*������� � ��/��� */
     Ft=0.0 ; /* # 0.0 - ���������� �� 0 ;  [ expr 100.0/60.0 ]   - ���������� �� 100 ;*/
     St=(Fcur*Fcur - Ft*Ft)/(2.0*accel) ;
     if (St>=mom_motion_distance) {
           vp=(Ft*Ft + 2.0*accel*mom_motion_distance) ;
           vp=fabs(vp) ;
           feed=sqrt(vp)*60.0*0.98 ;
           mom_event_time=mom_motion_distance/feed ;
           level++ ;
      } /* ; #[EQ_is_ge $St $mom_motion_distance] */

    /***********************************************************************/
/* ; #3.1 - ��������� ���� �� ������� ���������� �� �������� ���������� ����� ��� �� ����
   ; #3.2 - ��������� ���� �� ������� ��������� � �����
   ; # ������������ ����� ��������, (�����  Fanuc)
   ; # ����������� ���������� �� �������������� ������������ � ����������� � ����� �������� Vt, �����,
   ; # ��� ��� �������� ����������� ��������� ��� ������� ���������� ����������� �� ��������� � ������ �����.
   ; # feed_optimal (Vt) - ����������� ��������� ��������. */

    flag_optimal=0 ;
    if (VEC_overfall_feed(vec1f, vec2f, MAXFEED)) {

        for(i=0;i<3;i++) {
          R[i]= 0.0 ;
          if (vec1f[i]>MAXFEED[i]) {
          	R[i]=vec1f[i]/MAXFEED[i] ;
          	flag_optimal=1 ;
          }
        } /* ; # end for */
        /*;# ����� ���������*/
        Rmax=R[0] ;
        for(j=1;j<3;j++) {
        	if (Rmax<=R[j]) Rmax=R[j] ;
        }

        if (!EQ_is_zero(Rmax)) { feed_optimal=feed/Rmax ; }
        if (flag_optimal==1) {
          feed=feed_optimal ;
          mom_event_time=mom_motion_distance/feed ;
          level++;
        }

      } /* ; # end VEC_overfall_feed */

      /*������ ������ , ���� ��� ����������*/
      /*if (x_feed!=feed) {
         word[0]='\0'; sprintf(word,"%.1lf",feed);
         replace_word(curr,'F',word) ;
         numbers_of_kadrs++ ;
       }*/

   }

   /*4.0 ��� ����������� ���� �������� ������ �� ����������� - ���� F>=maxF{���}
    � �������� �� �������� ����������*/

   if (feed>1700.0) {
   
     if (angle<=5.0) {
        ;
     } else if (angle<=15.0) {
        feed*=0.98  ;
     } else if (angle<=30.0) {
        feed*=0.95  ;
     } else if (angle<=45.0) {
        feed*=0.90  ;
     } else if (angle<=60.0) {
        feed*=0.85   ;
     } else if (angle<=75.0) {
        feed*=0.80  ;
     } else if (angle<=80.0) {
        feed*=0.80  ;
     } else if (angle<=110.0) {
        feed*=0.75  ;
     } else if (angle<=135.0) {
        feed*=0.73  ;
     } else if (angle<=165.0) {
        feed*=0.73  ;
     } else if (angle<=175.0) {
        feed*=0.71  ;
     }

   } /*; (feed>1500.0)*/

   /*������ ������ , ���� ��� ����������*/
   if (x_feed!=feed) {
       word[0]='\0'; sprintf(word,"%.1lf",feed);
       replace_word(curr,'F',word) ;
       numbers_of_kadrs++ ;
   }

   prev_feed=feed;
   for(i=0;i<6;i++) prev_points[i]=curr_points[i];

   /*�������������� ��� ����������� ����� ���������*/
   if (small_move!=0) {
       currtmp[0]='\0';
       p=strchr(curr,'<') ; /* string have <CR> or <LF> as text */
       if (p==NULL) p=strchr(curr,'\r'); /* string have \r\n */
       if (p==NULL) p=strchr(curr,'\n'); /* string have \n   */
       if (p!=NULL) { strcpy(currtmp,p); *p=' '; *(p+1)='\0'; strcat(strcat(curr," ; ERROR time - small move"),currtmp); }
  	   else { currtmp[0]=' ';currtmp[1]='\n'; currtmp[2]='\0'; strcat(strcat(curr,currtmp)," ; ERROR time - small move"); }
      	small_move=0;
    }

   /*
   info[0]='\0';
   sprintf(info," %s \n\n f=%lf ",curr,feed );
   Application->MessageBox(info,"��������� ��������� - > ������ ...", MB_OK);
   */
    /* ShowMessage( AnsiString(curr) ); */

   /*��������� ������ � ����*/
   fputs(curr,ff);

   curr[0]='\0'; strcpy( curr, next );
   next[0]='\0'; fgets(next,mStr,rf);
 } // end while

 fputs(curr,ff);
 fclose(ff);
 fclose(rf);

/* mlib->mathl::~mathl(); */ // ����������

 return(numbers_of_kadrs);
}




