#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

#include <vcl.h>
#pragma hdrstop

#include "UpToUp.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
// Конструктор
__fastcall TUpToUp::TUpToUp(char *up_file1,char *up_file2,int sl,int A,int fm)
{
        sprintf(UP_file1,"%s",up_file1);
        sprintf(UP_file2,"%s",up_file2);
        SPEED_LIMIT=sl;
        FEED_MAX=fm;
        a=A;
        acenter=5;
        sprintf(PLANE,"XY");
        flag=0;  f2=0;
        k=1;
        f=0;
        mode=0;
        SUMX=0;SUMY=0;SUMZ=0;
}

int __fastcall TUpToUp::korrect()
{
  int ii;
  char ch,str[10],word[50];

  input=fopen(UP_file1,"r");

  if (input==NULL)
  {
     char *s;
     sprintf(s,"Не могу открыть файл %s",UP_file1);
     Application->MessageBox(s,"Ошибка в программе UpToUp", MB_OK);
     return(-1);
  }

  out=fopen(UP_file2,"w");
  if (out==NULL)
  {
     char *s;
     sprintf(s,"Не могу открыть файл %s",UP_file2);
     Application->MessageBox(s,"Ошибка в программе UpToUp", MB_OK);
     return(-2);
  }

  /* scaning input file */

	curr=fgets(s_curr,199,input); next=fgets(s_next,199,input);
	i=-1;
	while (curr!=NULL)
	{
	i=i+1;
	  analiz1(curr,next);
/* 18-ая и 19-ая позиции в очереди Р должны быть
	свободны перед следующим вызовом функции analiz */
	  if(i==18)
		 {
if(read_word(P[0].kadr,'N',word))
  {
  sprintf(str,"%3.3d",k);
  *(P[0].kadr+1)=*str;*(P[0].kadr+2)=*(str+1);*(P[0].kadr+3)=*(str+2);
  k=k+1; if(k==1000) k=1;
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
  sprintf(str,"%3.3d",k);
  *(P[0].kadr+1)=*str;*(P[0].kadr+2)=*(str+1);*(P[0].kadr+3)=*(str+2);
  k=k+1; if(k==1000) k=1;
  }
fputs(P[0].kadr,out);

if(read_word(P[1].kadr,'N',word))
  {
  sprintf(str,"%3.3d",k);
  *(P[1].kadr+1)=*str;*(P[1].kadr+2)=*(str+1);*(P[1].kadr+3)=*(str+2);
  k=k+1; if(k==1000) k=1;
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

/* Вывод последних кадров из очереди Р в выходной файл */
 for(ii=0;ii<=i;ii++)
{
if(read_word(P[ii].kadr,'N',word))
  {
  sprintf(str,"%3.3d",k);
  *(P[ii].kadr+1)=*str;*(P[ii].kadr+2)=*(str+1);*(P[ii].kadr+3)=*(str+2);
  k=k+1; if(k==1000) k=1;
  }
fputs(P[ii].kadr,out);
}
 fclose(input); fclose(out);

//printf("\n\nСоздан файл %s \n",argv[2]);

/* Вывод контрольных сумм по координатам X, Y, Z */
char s[200];
sprintf(s,"Контрольные суммы:\nSUMX =%10.2f mm\nSUMY =%10.2f mm\nSUMZ =%10.2f mm",
SUMX,SUMY,SUMZ);
ShowMessage(s);
	 // sprintf( str,"%d", a );
	 // execl("/server/programs/postprocess_n33/time_cnt","/server/programs/postprocess_n33/time_cnt",argv[2],str,NULL);
 return(0);
}

/* This function read the numeric string after key letter
	in success it return 1, otherwice 0
*/
int __fastcall TUpToUp::read_word(char *s,char w,char *sw)
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
void __fastcall TUpToUp::replace_word(char *s,char w,char *sw)
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
void __fastcall TUpToUp::feed_convert(int f,int mode,char *new_feed)
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

/* Эта функция преобразует подачу формата up-файла
	в целое число f */
int __fastcall TUpToUp::feed_convert1(int ff)
{
int x,y,f;

if(ff>4000) ff=ff-4000;
x=ff/100; /* x - это число цифр в подаче плюс 3 */
if(x==4){
	f=(ff-400)/10;
	if(f==0)f=1;
	if(f>9)f=f/10;/*?????????*/
	}
else
	{
y=ff-x*100; /* y - это две последние цифры кода подачи */
x=x-5;
f=y*pow(10,x);
	}

return f;
}


/* Анализ подач в двух соседних кадрах curr и next.
	Занесение curr и сгенерированного кадра с подачей
	в "очередь" P */

void __fastcall TUpToUp::analiz1(char *curr,char *next)
{
 float	x,xn,y,yn,z,zn,I,In,J,Jn,K,Kn,CosA,FA;
  int	f1,fn,ff,ffn,move,new_move,feed_set,new_feed_set,old_f;
  char  new_feed[10],str[20];
  char	word_x[50],word_y[50],word_z[50],word_f[50];
  int error;
  int IJKcurr=0;  /*Признак круговой интерполяции*/
  int IJKnext=0;


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
				 { sscanf(word_f,"%d",&ff);
					f=feed_convert1(ff);
					if((i==0) && (f>FEED_MAX))
					  {
					  f=FEED_MAX;
					  feed_convert(f,0,new_feed);
					  replace_word(curr,'F',new_feed);
					  }
			 feed_set=1;
				 }
/* Если в текущем кадре маленькое перемещение, то необходимо
   занизить скорость */
if( move==1 && (sqrt(x*x+y*y+z*z) <= 0.21) && (f>SPEED_LIMIT*2) )
  {
  old_f=f;
  f=SPEED_LIMIT*2;
  feed_convert(f,0,new_feed);
  replace_word(curr,'F',new_feed);
  flag=1;feed_set=1;
  }

/* Если требуется повторить предыдущую подачу
	в текущем кадре curr */
if(move==1 && mode>0 && feed_set==0)
  if(flag==0) feed_set=1;
  else {/*****************************/
       feed_convert(old_f,0,new_feed);
       replace_word(curr,'F',new_feed);
       feed_set=1;
       f=old_f;
       flag=0;
       }/*****************************/

/* Занесение curr в очередь Р */
strcpy(P[i].kadr,curr);
if (feed_set==1) P[i].feed=f;
else if (i==0) P[i].feed=0;
	  else if(move==1) P[i].feed=f;
if (move==1) P[i].span=sqrt(x*x+y*y+z*z);
else P[i].span=0;
error=otkat(i); /* При необходимости, коррекция подач
			в предыдущих кадрах в зависимости от
			подачи текущего кадра curr */
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
						 {f2=0;
						 new_feed_set=1;
			sscanf(word_f,"%d",&ffn);
			fn=feed_convert1(ffn);
						 if(fn>FEED_MAX)
							{
							fn=FEED_MAX;
							feed_convert(fn,0,new_feed);
							replace_word(next,'F',new_feed);
							}
                       }
	        else if(f2>0){new_feed_set=1;
	                      fn=f2;f2=0;
	                      feed_convert(fn,0,new_feed);
	                      replace_word(next,'F',new_feed);
	                      }
		      else {new_feed_set=0; fn=f;}

/* Если в кадре curr была занижена скорость на маленьком перемещении и
   нужно восстановить ее исходное значение для кадра next */
if(flag==1 && new_feed_set==0 && new_move==1)
  {
  feed_convert(old_f,0,new_feed);
  replace_word(next,'F',new_feed);
  new_feed_set=1;
  fn=old_f;
  flag=0;
  }
else if(flag==1 && new_move==1) flag=0;

/* Если в кадре next подача без перемещения, то пропустить ее */
if(  new_move==0 && new_feed_set==1 )
  { if(read_word(next,'M',word_x) ||
		 read_word(next,'L',word_x) ||
		 read_word(next,'G',word_x))
		 {feed_del(next); new_feed_set=0;}

	 else{
	 next=fgets(s_next,199,input);
	 if(next==NULL)
		 strcpy(s_next,"END");
	 new_feed_set=0;
	 goto label;
	}
  }

/* Учет центростремительного ускорения */
if(IJKnext){FA=sqrt(acenter*(In*In+Jn*Jn+Kn*Kn))*60; /* FA - максимально допустимая
																		  контурная скорость на дуге */
				 if(FA<fn) {f2=fn; fn=FA;
								feed_convert(fn,0,new_feed);
								replace_word(next,'F',new_feed);
								new_feed_set=1;
							  }
				}
/*************************************************/
/*Торможение можно производить только в кадрах с линейной
  интерполяцией и в кадрах с круговой интерполяцией,
  если хорда > 100 мм*/
if(IJKcurr && (f>fn) && (sqrt(x*x+y*y+z*z)<100)){
			 f=fn; P[i].feed=f;
			 feed_convert(f,0,new_feed);
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
					/* При круговой интерполяции вектор скорости направлен по касательной
						к дуге окружности */
						if(IJKcurr){
							if(strcmp(PLANE,"XY")==0)	tangent(I+x,J+y,&x,&y);
							else if(strcmp(PLANE,"XZ")==0)	tangent(I+x,K+z,&x,&z);
								  else if(strcmp(PLANE,"YZ")==0)	tangent(J+y,K+z,&y,&z);
							/* {x,y,z} - касательная к дуге в ее конечной точке */
									  }

						if(IJKnext){
										if(strstr(next,"G02")!=NULL) G=2;
										else if(strstr(next,"G03")!=NULL) G=3;

										if(strcmp(PLANE,"XY")==0)	tangent(In,Jn,&xn,&yn);
										else if(strcmp(PLANE,"XZ")==0)	tangent(In,Kn,&xn,&zn);
											  else if(strcmp(PLANE,"YZ")==0)	tangent(Jn,Kn,&yn,&zn);
										 /* {x,y,z} - касательная к дуге в ее начальной точке */
										}

		 /* Косинус угла между векторами  скоростей {x,y,z} и {xn,yn,zn} */
		 CosA=(x*xn+y*yn+z*zn)/sqrt(x*x+y*y+z*z)/sqrt(xn*xn+yn*yn+zn*zn);
		 if( CosA!=1 )
		  {
		  /* Максимально допустимая скорость для такого угла */
		  if(SPEED_LIMIT==0)
		if(CosA==-1)FA=0;
		else        FA=2/(1-CosA);
		  else FA=SPEED_LIMIT*2/(1-CosA);
		  /* При необходимости вставить промежуточный кадр с
		подачей, установить флаг mode=1 */
		  if ( (FA < fn) && ( FA < f ) ) mode=1;
		  }
		 }
		}
		else  /* new_move==0  */
		 /* При необходимости вставить промежуточный кадр с
			 нулевой подачей, установить флаг mode=2 */
		 if(new_feed_set==0)
		 mode=2;

	  } /* end of if(move==1) */

/* Если требуется для кадра next при следующем обращении
	к функции analiz повторить подачу предыдущего перемещения,
	то установить флаг mode=3 */
	  if(move==0 && new_move==1 && new_feed_set==0)  mode=3;

/* Если в текущем кадре нужно вывести подачу */
	  if (feed_set)
	  {
	  if(f ) { feed_convert(f,0,new_feed);
			replace_word(P[i].kadr,'F',new_feed);
				}
		 else  replace_word(P[i].kadr,'F',"0000");
	  }


/* Вставить в очередь Р промежуточный кадр с подачей FA */
	  if(mode==1){ f1=(int)FA;
		  feed_convert(f1,0,new_feed);
		  strcpy(str,"N000F");
		  strcat(str,strcat(new_feed,"\n"));
/*************************************************/
/*Торможение можно производить только в кадрах с линейной
  интерполяцией и в кадрах с круговой интерполяцией,
  если хорда > 100 мм*/
if (read_word(curr,'X',word_x)) { sscanf(word_x,"%f",&x);x=x/100; }
else x=0.0;
if (read_word(curr,'Y',word_y)) { sscanf(word_y,"%f",&y);y=y/100; }
else y=0.0;
if (read_word(curr,'Z',word_z)) { sscanf(word_z,"%f",&z);z=z/100; }
else z=0.0;
if(IJKcurr && (sqrt(x*x+y*y+z*z)<100)) {
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
		  /* При необходимости скорректировать
			  подачи предыдущих кадров */
				 }
		  error=otkat(i);
		  if(error) exit(0);
		}

/* Вставить в очередь промежуточный кадр с нулевой подачей */
	  if(mode==2) {i=i+1;
		  strcpy(P[i].kadr,"N000F0000\n");
		  P[i].feed=0;
		  P[i].span=0;
		  /* При необходимости скорректировать подачи
			  предыдущих кадров */
		  error=otkat(i);
		  if(error) exit(0);
		  mode=0;}

}

/* Эта функция осуществляет анализ величины перемещения
	предыдущего кадра. Если это перемещение недостаточно
	для торможения до скорости текущего кадра, то подача
	предыдущего кадра заменяется на меньшую, и осуществля-
	ется откат на один кадр назад и аналогичная проверка */

int __fastcall TUpToUp::otkat(int j)
{
float f,x,y,z;
int f1,jj;
char new_feed[10];
char word[50],word_x[50],word_y[50],word_z[50];
char info[360];

if(j==0) return 0;
jj=j;
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
	read_word(P[j-1].kadr,'K',word)) && (sqrt(x*x+y*y+z*z)<100))
	  {
	  if(P[j].feed==0){
	  	printf("\nВ кадре c круговой интерполяцией %s\
			недопустимое торможение до 0. Скорректируйте исходную программу.\n\
			\n",P[j-1].kadr);
                info[0]='\0';
                sprintf(info,"В кадре c круговой интерполяцией %s \
			недопустимое торможение до 0. Скорректируйте исходную программу.",P[j-1].kadr );
                MessageDlg(info, mtWarning, TMsgDlgButtons() << mbYes ,0);
		return 1;
	  }
	  P[j-1].feed=P[j].feed;
	  feed_convert(P[j].feed,0,new_feed);
	  replace_word(P[j-1].kadr,'F',new_feed);
	  j--; continue;
	  }
	  /**********************************/
	  f=sqrt(2*a*P[j-1].span+(float)P[j].feed*(float)P[j].feed/60/60);
	  f1=f*60; /* Перевод подачи в мм/мин */
	  if(f1==0) f1=5;
	  if( f1 < P[j-1].feed )
	 { P[j-1].feed = f1;
		feed_convert(f1,0,new_feed);
			replace_word(P[j-1].kadr,'F',new_feed);
		j--;
			}
	  else break;
	  }
if(j==0) {
	 /*
	 printf("\nToo few feed in kadr:\n%s%s\n",P[jj-1].kadr,
						  P[jj].kadr);
	 printf("  <<  Press any key  >>\n");*/
	 printf("\nСлишком маленькая подача в кадре:\n%s%s\n",
		P[jj-1].kadr,P[jj].kadr);
printf("\nСтанок не успевает затормозиться в предыдущих 18-и кадрах.\n\
Скорректируйте исходную программу.\n\n");
         info[0]='\0';
         sprintf(info,"Слишком маленькая подача в кадре:\n %s %s \n \
                       \nСтанок не успевает затормозиться в предыдущих 18-и кадрах.\n\
                      Скорректируйте исходную программу.",P[jj-1].kadr,P[jj].kadr );
         MessageDlg(info, mtWarning, TMsgDlgButtons() << mbYes ,0);
	 return 1;
	 }
else return 0;
}

/* Эта функция удаляет из кадра подачу */
void __fastcall TUpToUp::feed_del(char *kadr)
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

/* Нахождение касательной {x,y} к дуге, заданной координатами x,y,i,j,
	в начальной точке дуги */
void __fastcall TUpToUp::tangent(float i,float j,float *x,float *y)
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
/* Дуга в первой четверти, движение по ч.с. */
if((*x>0) && (*y<0) && (G==2))
  {*x=fabs(j); *y=-fabs(i);}
/* Дуга в первой четверти, движение против ч.с. */
else if((*x<0) && (*y>0) && (G==3))
			{*x=-fabs(j); *y=fabs(i);}
/* Дуга во второй четверти, движение по ч.с. */
	  else if((*x>0) && (*y>0) && (G==2))
						{*x=fabs(j); *y=fabs(i);}
/* Дуга во второй четверти, движение против ч.с. */
			 else if((*x<0) && (*y<0) && (G==3))
						{*x=-fabs(j); *y=-fabs(i);}
/* Дуга в третьей четверти, движение по ч.c. */
					else if((*x<0) && (*y>0) && (G==2))
							 {*x=-fabs(j); *y=fabs(i);}
/* Дуга в третьей четверти, движение против ч.с. */
						  else if((*x>0) && (*y<0) && (G==3))
									{*x=fabs(j); *y=-fabs(i);}
/* Дуга в четвертой четверти, движение по ч.c. */
								 else if((*x<0) && (*y<0) && (G==2))
										  {*x=-fabs(j); *y=-fabs(i);}
/* Дуга в четвертой четверти, движение против ч.с. */
										else if((*x>0) && (*y>0) && (G==3))
												 {*x=fabs(j); *y=fabs(i);}
}

TUpToUp::~TUpToUp()
{
        //TODO: Add your source code here

}


