#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "TimeCount.h"
#include "razbivka.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

void __fastcall TMainDialog::PtpToUp(char *PTP_file,char *UP_file)
{
        acenter=5;         /* ������������������� ��������� */

        sprintf(PLANE,"XY"); /* ��������� �������� ������������ */
        flag=0;
        k=1; /* ������� ������ */
        feed=0; /* ������ �������� ����� curr */
        mode=0; /* ���� */
        SUMX=0;SUMY=0;SUMZ=0;

        int ii;
        char ch,str[10],word[50];

        input=fopen(PTP_file,"r");
        if (input==NULL) {
                Application->MessageBox("�� ���� ������� ptp-���� -> new_gpm::PtpToUp",
                                         "������ � ���������", MB_OK);
                /*Application->Terminate();*/
                return ;
        }

        out=fopen(UP_file,"w");
        if (out==NULL) {
                Application->MessageBox("�� ���� ������� up-���� -> new_gpm::PtpToUp",
                                         "������ � ���������", MB_OK);
                /*Application->Terminate();*/
                return ;
        }

       if (mode_machine==0) { none_methods(); goto end_message; }

        /* scaning input file */
	curr=fgets(s_curr,199,input); next=fgets(s_next,199,input);
	i=-1;
	while (curr!=NULL)
	{
	  i=i+1;
	  analiz(curr,next);

        /* 18-�� � 19-�� ������� � ������� � ������ ����
	�������� ����� ��������� ������� ������� analiz */
	  if(i==18)
	  {
                if(read_word(P[0].kadr,'N',word))
                {
/*                        sprintf(str,"%3.3d",k);
                        *(P[0].kadr+1)=*str;
                        *(P[0].kadr+2)=*(str+1);
                        *(P[0].kadr+3)=*(str+2);
                        k=k+1; if(k==1000) k=1;
*/
                    if (mode_seq!=0) {
                         sprintf(str,"%05ld",k);
    		         replace_word(P[0].kadr,'N',str) ;
    		         k=k+1; if(k==30000) k=1; //if(k==100000) k=1;
    	             } else {
        	         sprintf(str,"%03d",k);
                         replace_word(P[0].kadr,'N',str) ;
   		         k=k+1; if(k==1000) k=1;
    	             }

                }
                fputs(P[0].kadr,out);

		for(i=0;i<=17;i++)
		{
		        strcpy(P[i].kadr,P[i+1].kadr);
		        P[i].feed=P[i+1].feed;
		        P[i].span=P[i+1].span;
		}
		i=17;
	  } /* end of if(i==18) */
	  if(i==19)
	  {
                if(read_word(P[0].kadr,'N',word))
                {
/*                        sprintf(str,"%3.3d",k);
                        *(P[0].kadr+1)=*str;
                        *(P[0].kadr+2)=*(str+1);
                        *(P[0].kadr+3)=*(str+2);
                        k=k+1;if(k==1000) k=1;
*/
                    if (mode_seq!=0) {
                         sprintf(str,"%05ld",k);
    		         replace_word(P[0].kadr,'N',str) ;
    		         k=k+1; if(k==30000) k=1;  // if(k==100000) k=1;
    	             } else {
        	         sprintf(str,"%03d",k);
                         replace_word(P[0].kadr,'N',str) ;
    		         k=k+1; if(k==1000) k=1;
    	             }

                }
                fputs(P[0].kadr,out);

                if(read_word(P[1].kadr,'N',word))
                {
/*                        sprintf(str,"%3.3d",k);
                        *(P[1].kadr+1)=*str;
                        *(P[1].kadr+2)=*(str+1);
                        *(P[1].kadr+3)=*(str+2);
                        k=k+1; if(k==1000) k=1;
*/
                    if (mode_seq!=0) {
                         sprintf(str,"%05ld",k);
    		         replace_word(P[1].kadr,'N',str) ;
    		         k=k+1; if(k==30000) k=1; // if(k==100000) k=1;
    	             } else {
                         sprintf(str,"%03d",k);
                         replace_word(P[1].kadr,'N',str) ;
    		         k=k+1; if(k==1000) k=1;
    	             }
                }
                fputs(P[1].kadr,out);

		for(i=0;i<=17;i++)
		{
		        strcpy(P[i].kadr,P[i+2].kadr);
		        P[i].feed=P[i+2].feed;
		        P[i].span=P[i+2].span;
		}
		i=17;
          } /* end of if(i==19) */

	  if ((next!=NULL) && (strcmp(s_next,"END")!=0)) strcpy(curr,next);
	  else curr=NULL;
	  next=fgets(s_next,199,input);
	} /* end of while */

        /* ����� ��������� ������ �� ������� � � �������� ���� */
        for(ii=0;ii<=i;ii++)
        {
                if(read_word(P[ii].kadr,'N',word))
                {
/*                        sprintf(str,"%3.3d",k);
                        *(P[ii].kadr+1)=*str;
                        *(P[ii].kadr+2)=*(str+1);
                        *(P[ii].kadr+3)=*(str+2);
                        k=k+1; if(k==1000) k=1;
*/
                    if (mode_seq!=0) {
                         sprintf(str,"%05ld",k);
    		         replace_word(P[ii].kadr,'N',str) ;
    		         k=k+1; if(k==30000) k=1; // if(k==100000) k=1;
    	             } else {
        	         sprintf(str,"%03d",k);
                         replace_word(P[ii].kadr,'N',str) ;
    		         k=k+1; if(k==1000) k=1;
    	             }
                }
                fputs(P[ii].kadr,out);
        }


end_message:
        fclose(input); fclose(out);

        /* �������� ��������� */
        char mes[500];
        char mes1[400],mes2[100];
        mes1[0]='\0';
        sprintf(mes1,"������ ���� %s \n����������� �����:\n\t\
SUMX =%10.2f mm\n\tSUMY =%10.2f mm\n\tSUMZ =%10.2f mm\n",UP_file,SUMX,SUMY,SUMZ);
        //Application->MessageBox(mes, "new_gpm", MB_OK);
        //FormResults->EditSumma->Caption=mes1;

        // ����� ����������
        TTimeCounter *tc=new TTimeCounter(UP_file,a);
        float  TIME;
        TIME=tc->count();
        tc->TTimeCounter::~TTimeCounter(); // ����������

        // ������ ������� - ����� � �����
        float  tmpTime , tm2;
        long int tm1 ;
        tmpTime =TIME/60.0 ;
        tm1=(int) tmpTime/60.0;
        tm2=(tmpTime/60.0 - tm1) * 60.0 ;
        if (tm1==0) {
           sprintf(mes2,"\n����� ���������� ->\n %10.0f ������ = %.2f �����",TIME,TIME/60);
         } else {
            sprintf(mes2,"\n����� ���������� ->\n %10.0f ������ = %.2f ����� = [ %d � %.1f ��� ]",TIME,tmpTime, tm1 ,tm2);
        }
        //FormResults->EditTime->Caption=mes2;

        /*�������� ���������*/
        strcpy(mes,mes1);
        strcat(mes,mes2);
      //1//ShowMessage(AnsiString(mes));
      //2//
      MessageDlg(mes, mtConfirmation, TMsgDlgButtons() << mbOK , 0);
      //3//FormResults->Visible=True;
      //4//FormResults->ShowModal();

        if (mode_output!=0) {
             mes[0]='\0';
             strcpy(mes,"notepad ");
             strcat(mes, UP_file);
             try
             {
                WinExec(mes,1);
             }
             catch (Exception &exception)
             {
                 Application->ShowException(&exception);
              }
        }

        // ��������
        TRazbivka *razbiv = new TRazbivka(UP_file,SPEED_LIMIT,a,FEED_MAX);
}

/* This function read the numeric string after key letter
	in success it return 1, otherwice 0
*/
int __fastcall TMainDialog::read_word(char *s,char w,char *sw)
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

void __fastcall TMainDialog::replace_word(char *s,char w,char *sw)
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
void __fastcall TMainDialog::feed_convert(int f,int mode,char *new_feed)
{
int k;
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


/* ������ ����� � ���� �������� ������ curr � next.
	��������� curr � ���������������� ����� � �������
	� "�������" P */

void __fastcall TMainDialog::analiz(char *curr,char *next)
{
  char mes[250];
  float	x,xn,y,yn,z,zn,I,In,J,Jn,K,Kn,CosA,tmp1,tmp2,FA;
  int	f1,fn,move,new_move,feed_set,new_feed_set;
  char  new_feed[10],str[20];
  char	word_x[50],word_y[50],word_z[50],word_f[50];
  int error;
  int IJKcurr=0;  /*������� �������� ������������*/
  int IJKnext=0;
  FA=0;

if(strstr(curr,"G02")!=NULL) G=2;
else if(strstr(curr,"G03")!=NULL) G=3;

if(strstr(curr,"G17")!=NULL)strcpy(PLANE,"XY");
else if(strstr(curr,"G18")!=NULL)strcpy(PLANE,"XZ");
		  else if(strstr(curr,"G19")!=NULL)strcpy(PLANE,"YZ");

move=new_move=feed_set=new_feed_set=0;
	  if (read_word(curr,'X',word_x)) { sscanf(word_x,"%f",&x);x=x/100;
						 SUMX=SUMX+x;
						 move=1; }
	  else x=0.0;
	  if (read_word(curr,'Y',word_y)) { sscanf(word_y,"%f",&y);y=y/100;
						 SUMY=SUMY+y;
						 move=1; }
	  else y=0.0;
	  if (read_word(curr,'Z',word_z)) { sscanf(word_z,"%f",&z);z=z/100;
						 SUMZ=SUMZ+z;
						 move=1; }
	  else z=0.0;
			 if(move==1){
if (read_word(curr,'I',word_z))  {sscanf(word_z,"%f",&I);I=I/100;IJKcurr=1;}
	  else I=0.0;
if (read_word(curr,'J',word_z))  {sscanf(word_z,"%f",&J);J=J/100;IJKcurr=1;}
	  else J=0.0;
if (read_word(curr,'K',word_z))  {sscanf(word_z,"%f",&K);K=K/100;IJKcurr=1;}
	  else K=0.0;
							}
	  if (read_word(curr,'F',word_f))
				 { sscanf(word_f,"%d",&feed);
					if((i==0) && (feed>FEED_MAX))
					  {
					  feed=FEED_MAX;
					  sprintf(new_feed,"%d",feed);
					  replace_word(curr,'F',new_feed);
					  }
			 feed_set=1;
				 }
/* ���� � ������� ����� ��������� �����������, �� ����������
   �������� �������� */
if( move==1 && (sqrt(x*x+y*y+z*z) <= 0.21) && (feed>SPEED_LIMIT*2) )
  {
  old_f=feed;
  feed=SPEED_LIMIT*2;
  sprintf(new_feed,"%d",feed);
  replace_word(curr,'F',new_feed);
  flag=1; feed_set=1;
  }

/* ���� ��������� ��������� ���������� ������
	� ������� ����� curr */
if(move==1 && mode>0 && feed_set==0)
  if(flag==0) feed_set=1;
  else {/*****************************/
       sprintf(new_feed,"%d",old_f);
       replace_word(curr,'F',new_feed);
       feed_set=1;
       feed=old_f;
       flag=0;
       }/*****************************/

/* ��������� curr � ������� � */
strcpy(P[i].kadr,curr);
if (feed_set==1) P[i].feed=feed;
else if (i==0) P[i].feed=0;
	  else if(move==1) P[i].feed=feed;
if (move==1) P[i].span=sqrt(x*x+y*y+z*z);
else P[i].span=0;
error=otkat(i); /* ��� �������������, ��������� �����
			� ���������� ������ � ����������� ��
			������ �������� ����� curr */
if(error) exit(0);

	  mode=0;
label:
		if (read_word(next,'X',word_x)) { new_move=1; sscanf(word_x,"%f",&xn); xn=xn/100;}
		else xn=0.0;
		if (read_word(next,'Y',word_y)) { new_move=1; sscanf(word_y,"%f",&yn); yn=yn/100;}
		else yn=0.0;
		if (read_word(next,'Z',word_z)) { new_move=1; sscanf(word_z,"%f",&zn); zn=zn/100;}
		else zn=0.0;
					 if(new_move==1){
if (read_word(next,'I',word_z))  {sscanf(word_z,"%f",&In);In=In/100;IJKnext=1;}
	  else In=0.0;
if (read_word(next,'J',word_z))  {sscanf(word_z,"%f",&Jn);Jn=Jn/100;IJKnext=1;}
	  else Jn=0.0;
if (read_word(next,'K',word_z))  {sscanf(word_z,"%f",&Kn);Kn=Kn/100;IJKnext=1;}
	  else Kn=0.0;
										 }
		if (read_word(next,'F',word_f))
						 {
 						 new_feed_set=1;
			sscanf(word_f,"%d",&fn);
						 if(fn>FEED_MAX)
							{
							fn=FEED_MAX;
							sprintf(new_feed,"%d",fn);
							replace_word(next,'F',new_feed);
							}
						 }
		else {new_feed_set=0; fn=feed;}
/* ���� � ����� curr ���� �������� �������� �� ��������� ����������� �
   ����� ������������ �� �������� �������� ��� ����� next */
if(flag==1 && new_feed_set==0 && new_move==1)
  {
  sprintf(new_feed,"%d",old_f);
  replace_word(next,'F',new_feed);
  new_feed_set=1;
  fn=old_f;
  flag=0;
  }
else if(flag==1 && new_move==1) flag=0;

/* ���� � ����� next ������ ��� �����������, �� ���������� �� */
if( move==1 && new_move==0 && new_feed_set==1 )
  { if(read_word(next,'M',word_x) ||
	read_word(next,'L',word_x) ||
	 read_word(next,'G',word_x))
		 {feed_del(next); new_feed_set=0;}

	 else{
	 next=fgets(s_next,199,input);
	 if(next==NULL) strcpy(s_next,"END");
	 new_feed_set=0;
	 goto label;
	}
  }

/* ���� �������������������� ��������� */

if(IJKnext){FA=sqrt(acenter*(In*In+Jn*Jn+Kn*Kn))*60; /* FA - ����������� ���������� ��������� �������� �� ���� */
            mes[0]='\0';
            sprintf(mes,"%.3f",FA) ;
            FA=atof(mes);
				 if(FA<fn) {/******/
                                      //old_f=feed;
                                      if (mode_circle_slowdn!=0) flag=1;
                                       //else { old_f=feed; }
                                            /******/
                                            fn=FA;
				            sprintf(new_feed,"%d",fn);
            				    replace_word(next,'F',new_feed);
					    new_feed_set=1;

					   }
				}
/*************************************************/
/*���������� ����� ����������� ������ � ������ � ��������
  ������������� � � ������ � �������� �������������,
  ���� ����� > circle_dist ��*/
if(IJKcurr && (feed>fn) && (sqrt(x*x+y*y+z*z)<circle_dist)){
                       // old_f=feed;
                         //if (mode_circle_slowdn==0) old_f=feed;
                          /**/
			 feed=fn; P[i].feed=feed;
			 feed_convert(feed,0,new_feed);
			 replace_word(P[i].kadr,'F',new_feed);
			 error=otkat(i);
			 if(error) exit(0);
			 }
/*************************************************/
		if (move==1)
		{

		if (new_move)
		{
	if (fn > SPEED_LIMIT )
		 {
					/* ��� �������� ������������ ������ �������� ��������� �� �����������
						� ���� ���������� */
						if(IJKcurr){
							if(strcmp(PLANE,"XY")==0)     /*tangent(I+x,J+y,&x,&y);*/ tangentn(I,J,&x,&y,2);
							else if(strcmp(PLANE,"XZ")==0)	/*tangent(I+x,K+z,&x,&z);*/ tangentn(I,K,&x,&z,2);
								  else if(strcmp(PLANE,"YZ")==0)      /*tangent(J+y,K+z,&y,&z);*/ tangentn(J,K,&y,&z,2);
							/* {x,y,z} - ����������� � ���� � �� �������� ����� */
									  }

						if(IJKnext){
										if(strstr(next,"G02")!=NULL) G=2;
										else if(strstr(next,"G03")!=NULL) G=3;

										if(strcmp(PLANE,"XY")==0)	/*tangent(In,Jn,&xn,&yn); */ tangentn(In,Jn,&xn,&yn,1);
										else if(strcmp(PLANE,"XZ")==0) /*tangent(In,Kn,&xn,&zn); */ tangentn(In,Kn,&xn,&zn,1);
											  else if(strcmp(PLANE,"YZ")==0)   /*tangent(Jn,Kn,&yn,&zn); */ tangentn(Jn,Kn,&yn,&zn,1);
										 /* {x,y,z} - ����������� � ���� � �� ��������� ����� */
										}

		 /* ������� ���� ����� ���������  ��������� {x,y,z} � {xn,yn,zn} */
                 /*23 ������ 2004*/
                 tmp1=sqrt(x*x+y*y+z*z);
                 tmp2=sqrt(xn*xn+yn*yn+zn*zn);
                 if (tmp1==0.0 || tmp2==0.0) CosA=0.0 ; else CosA=(x*xn+y*yn+z*zn)/(tmp1*tmp2);
                 mes[0]='\0';
                 sprintf(mes,"%.3f",CosA) ;
                 CosA=atof(mes);
		 if( CosA!=1.0 )
		  {
		  /* ����������� ���������� �������� ��� ������ ���� */
		  if(SPEED_LIMIT==0)
                     if(CosA==-1.0)FA=0; else FA=2/(1-CosA);
		   else FA=SPEED_LIMIT*2/(1-CosA);
		  /* ��� ������������� �������� ������������� ���� �
		�������, ���������� ���� mode=1 */
		  if ( (FA < fn) && ( FA < feed ) ) mode=1;
		  } else { FA=feed ; }
		 }
		}
		else  /* new_move==0  */
		 /* ��� ������������� �������� ������������� ���� �
			 ������� �������, ���������� ���� mode=2 */
		 if(new_feed_set==0)
		 mode=2;

	  } /* end of if(move==1) */

/* ���� ��������� ��� ����� next ��� ��������� ���������
	� ������� analiz ��������� ������ ����������� �����������,
	�� ���������� ���� mode=3 */
	  if(move==0 && new_move==1 && new_feed_set==0)  mode=3;

/* ���� � ������� ����� ����� ������� ������ */
	  if (feed_set)
	  {
 	  if (feed) { feed_convert(feed,0,new_feed);
			replace_word(P[i].kadr,'F',new_feed);
				}
		 else  replace_word(P[i].kadr,'F',"0000");
	  }


/* �������� � ������� � ������������� ���� � ������� FA */
	  if(mode==1){ f1=(int)FA;
		  feed_convert(f1,0,new_feed);
		  strcpy(str,"N000F");
		  strcat(str,strcat(new_feed,"\n"));
/*************************************************/
/*���������� ����� ����������� ������ � ������ � ��������
  ������������� � � ������ � �������� �������������,
  ���� ����� > circle_dist ��*/
if (read_word(curr,'X',word_x)) { sscanf(word_x,"%f",&x);x=x/100; }
else x=0.0;
if (read_word(curr,'Y',word_y)) { sscanf(word_y,"%f",&y);y=y/100; }
else y=0.0;
if (read_word(curr,'Z',word_z)) { sscanf(word_z,"%f",&z);z=z/100; }
else z=0.0;
			 if(IJKcurr && (sqrt(x*x+y*y+z*z)<circle_dist)) {
				 P[i].feed=f1;
				 feed_convert(f1,0,new_feed);
				 replace_word(P[i].kadr,'F',new_feed);
				 }
			 else    {
/*************************************************/
		  i=i+1;
		  strcpy(P[i].kadr,str);
		  P[i].feed=f1;
		  P[i].span=0;
		  /* ��� ������������� ���������������
			  ������ ���������� ������ */
				 }
		  error=otkat(i);
		  if(error) exit(0);
		}

/* �������� � ������� ������������� ���� � ������� ������� */
	  if(mode==2) {i=i+1;
		  strcpy(P[i].kadr,"N000F0000\n");
		  P[i].feed=0;
		  P[i].span=0;
		  /* ��� ������������� ��������������� ������
			  ���������� ������ */
		  error=otkat(i);
		  if(error) exit(0);
		  mode=0;}

}

/* ��� ������� ������������ ������ �������� �����������
	����������� �����. ���� ��� ����������� ������������
	��� ���������� �� �������� �������� �����, �� ������
	����������� ����� ���������� �� �������, � ����������-
	���� ����� �� ���� ���� ����� � ����������� �������� */

int __fastcall TMainDialog::otkat(int j)
{
float f,x,y,z;
int f1,jj;
char new_feed[10];
char word[50],word_x[50],word_y[50],word_z[50];

if(j==0) return 0;
jj=j;

/*5 ���� 2004*/
if (j>0) {
   if ((P[j].feed<=P[j-1].feed)&&(P[j].feed==0.0)&&(P[j-1].feed==0.0)) {
     x=0.0;y=0.0;z=0.0;
     if (read_word(P[j-1].kadr,'X',word_x)) { sscanf(word_x,"%f",&x);x=x/100; }
     if (read_word(P[j-1].kadr,'Y',word_y)) { sscanf(word_y,"%f",&y);y=y/100; }
     if (read_word(P[j-1].kadr,'Z',word_z)) { sscanf(word_z,"%f",&z);z=z/100; }
     if((read_word(P[j-1].kadr,'I',word)||read_word(P[j-1].kadr,'J',word)||
	read_word(P[j-1].kadr,'K',word)) && (sqrt(x*x+y*y+z*z)<circle_dist))
	  if(P[j].feed==0){ print_error(-21,P[j-1].kadr); return 1; }
   }
 }

 while((j>0) && (P[j].feed<P[j-1].feed))
	  {
	  /**********************************/
	   if (read_word(P[j-1].kadr,'X',word_x)) { sscanf(word_x,"%f",&x);x=x/100; }
	   else x=0.0;
	   if (read_word(P[j-1].kadr,'Y',word_y)) { sscanf(word_y,"%f",&y);y=y/100; }
	   else y=0.0;
	   if (read_word(P[j-1].kadr,'Z',word_z)) { sscanf(word_z,"%f",&z);z=z/100; }
	   else z=0.0;
	  if((read_word(P[j-1].kadr,'I',word)||read_word(P[j-1].kadr,'J',word)||
	read_word(P[j-1].kadr,'K',word)) && (sqrt(x*x+y*y+z*z)<circle_dist))
	  {
	  if(P[j].feed==0){ print_error(-21,P[j-1].kadr); return 1; }
	  P[j-1].feed=P[j].feed;
	  feed_convert(P[j].feed,0,new_feed);
	  replace_word(P[j-1].kadr,'F',new_feed);
	  j--; continue;
	  }
	  /**********************************/
          /*5 ���� 2004*/
          f=(float)P[j].feed*(float)P[j].feed/60/60;
          f=f + 2.0*a*P[j-1].span ;
          f=sqrt(f);
	  f1=f*60; /* ������� ������ � ��/��� */
          /*5 ���� 2004*/
	  if(f1==0) f1=30;
          /*7 ���� 2004*/
	  if ( (f1 < P[j-1].feed) && \
                  !read_word(P[j-1].kadr,'M',word)  && !read_word(P[j-1].kadr,'S',word) )
	  {
            P[j-1].feed = f1;
	    feed_convert(f1,0,new_feed);
	    replace_word(P[j-1].kadr,'F',new_feed);
	    j--;
           } else break;
	  }
  if(j==0) { print_error(-22,P[jj-1].kadr); return 1; }
else return 0;
}

/* ��� ������� ������� �� ����� ������ */
void __fastcall TMainDialog::feed_del(char *kadr)
{
char ch;
char str[20];
int i=0,j=0;

ch=*kadr;
do
{
ch=*(kadr+j);
if(ch=='F') {j=j+5;continue;}
str[i++]=ch;
j++;
} while(ch!='\0');

strcpy(kadr,str);
}

/* ���������� ����������� {x,y} � ����, �������� ������������ x,y,i,j,
	� ��������� ����� ���� */
void __fastcall TMainDialog::tangent(float i,float j,float *x,float *y)
{
if(i==0 || j==0)
{/*****/
if((*x>0) && (*y<0) && (G==2))
  {*x=0; *y=-1;}
else if((*x<0) && (*y>0) && (G==3))
       {*x=-1; *y=0;}
     else if((*x>0) && (*y>0) && (G==2))
	    {*x=1; *y=0;}
	  else if((*x<0) && (*y<0) && (G==3))
		 {*x=0; *y=-1;}
	       else if((*x<0) && (*y>0) && (G==2))
		      {*x=0; *y=1;}
		    else if((*x>0) && (*y<0) && (G==3))
			   {*x=1; *y=0;}
			 else if((*x<0) && (*y<0) && (G==2))
				{*x=-1; *y=0;}
			      else if((*x>0) && (*y>0) && (G==3))
				     {*x=0; *y=1;}
return;
}/*****/

/* ���� � ������ ��������, �������� �� �.�. */
if((*x>0) && (*y<0) && (G==2))
  {*x=fabs(j); *y=-fabs(i);}
/* ���� � ������ ��������, �������� ������ �.�. */
else if((*x<0) && (*y>0) && (G==3))
			{*x=-fabs(j); *y=fabs(i);}
/* ���� �� ������ ��������, �������� �� �.�. */
	  else if((*x>0) && (*y>0) && (G==2))
						{*x=fabs(j); *y=fabs(i);}
/* ���� �� ������ ��������, �������� ������ �.�. */
			 else if((*x<0) && (*y<0) && (G==3))
						{*x=-fabs(j); *y=-fabs(i);}
/* ���� � ������� ��������, �������� �� �.c. */
					else if((*x<0) && (*y>0) && (G==2))
							 {*x=-fabs(j); *y=fabs(i);}
/* ���� � ������� ��������, �������� ������ �.�. */
						  else if((*x>0) && (*y<0) && (G==3))
									{*x=fabs(j); *y=-fabs(i);}
/* ���� � ��������� ��������, �������� �� �.c. */
								 else if((*x<0) && (*y<0) && (G==2))
										  {*x=-fabs(j); *y=-fabs(i);}
/* ���� � ��������� ��������, �������� ������ �.�. */
									else if((*x>0) && (*y>0) && (G==3))
												 {*x=fabs(j); *y=fabs(i);}
}

void __fastcall TMainDialog::print_error(int err,char *mes)
{
  char imes[300];

  switch (err) {
  	case 0 :
  	        break ;
  	case -11 :
  	        break ;
  	case -21 :
  	  /*printf("\n� ����� c �������� ������������� %s\
          ������������ ���������� �� 0. �������������� �������� ���������.\n\n",P[j-1].kadr);*/
          sprintf(imes,"� ����� c �������� ������������� %s \
              ������������ ���������� �� 0. �������������� �������� ���������.\n ��� �������:\n\t 1.����� ���������� ��������� ���������\n\t 2.������ ������� �������������� �������� �� ����������",mes);
          Application->MessageBox(imes, "new_gpm", MB_OK);
  	        break ;
  	case -22 :
	 /*printf("\nToo few feed in kadr:\n%s%s\n",P[jj-1].kadr,P[jj].kadr);
	 printf("  <<  Press any key  >>\n");
printf("\n������� ��������� ������ � �����:\n%s%s\n",P[jj-1].kadr,P[jj].kadr);
printf("\n������ �� �������� ������������� � ���������� 18-� ������.\n\
�������������� �������� ���������.\n\n<< << ������� ����� ������� >> >>\n"); */
	 sprintf(imes,"������� ��������� ������ � �����:\n%s \n \
	  ������ �� �������� ������������� � ���������� 18-� ������.\n \
	  �������������� �������� ���������.\n ��� �������: \n\t 1.����� ���������� ��������� ���������\n\t 2.������ ������� �������������� �������� �� ����������",mes);
	 Application->MessageBox(imes, "new_gpm", MB_OK);
  	        break ;
  	case -31 :
  	        break ;
  	case -41 :
  	        break ;
  	default :
  	        break ;
  }

 return ;
}


float __fastcall TMainDialog::tangentn(float i,float j,float *xk,float *yk,int tp)
{
  /*5 ���� 2004*/
  float x,y;
  x=*xk; y=*yk;
  i=fabs(i);
  j=fabs(j);
  if (tp==2) {
	  if ((x<=0) && (y>=0)) { i=-fabs(i); j=-fabs(j); }
	  if ((x>=0) && (y>=0)) { i=-fabs(i); j=fabs(j); }
	  if ((x>=0) && (y<=0)) { i=fabs(i); j=fabs(j); }
	  if ((x<=0) && (y<=0)) { i=fabs(i); j=-fabs(j); }
	  if (G==3) { i=(-1)*i;  j=(-1)*j; }
	  i=i+x;  j=j+y;
  }

  x=fabs(-j)*EQ_sign(x);
  y=fabs(i)*EQ_sign(y);

  *xk=x; *yk=y;
  return(0);
}

float __fastcall TMainDialog::EQ_sign (float s)
{
 /*5 ���� 2004*/
 if (s<0.0) { return(-1) ; }
 if (fabs(s)<=0.00000001) { return(0) ; }
 if (s>0.0) { return(1) ; }
 return(1) ;
}

int __fastcall TMainDialog::none_methods (void)
{
  /*6 ���� 2004*/
  char word[20],str[20];
  float x,y,z;

  SUMX=0;SUMY=0;SUMZ=0;
  curr=fgets(s_curr,199,input);
  k=1;
  while(curr!=NULL)
  {
    /*....����� .....*/
    x=0.0; y=0.0; z=0.0;
    if (read_word(curr,'X',word)) { sscanf(word,"%f",&x);x=x/100; SUMX=SUMX+x; }
    if (read_word(curr,'Y',word)) { sscanf(word,"%f",&y);y=y/100; SUMY=SUMY+y; }
    if (read_word(curr,'Z',word)) { sscanf(word,"%f",&z);z=z/100; SUMZ=SUMZ+z; }
    /*����������� ������*/
    if (read_word(curr,'F',word))
    {
      sscanf(word,"%d",&feed);
      if(feed>FEED_MAX) { feed=FEED_MAX; }
      feed_convert(feed,0,str);
      replace_word(curr,'F',str);
    }
    /*��������� ������*/
    if(read_word(curr,'N',word))
    {
        if (mode_seq!=0) {
             sprintf(str,"%05ld",k);
             replace_word(curr,'N',str) ;
             k=k+1; if(k==100000) k=1;
         } else {
             sprintf(str,"%03d",k);
             replace_word(curr,'N',str) ;
             k=k+1; if(k==1000) k=1;
         }
    }
    /*���������� � ���� ....*/
    fputs(curr,out);
    curr= fgets(s_curr,199,input);
  }//end of while

 return 0 ;
}

