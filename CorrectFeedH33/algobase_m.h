//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#define mStr 128


/* declaring additional variables */
struct INFO_FILE_STAT {
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
} ;



int __fastcall read_word(char *,char ,char *) ;
void __fastcall replace_word(char *,char ,char *) ;
int delete_word(char *,char ) ;

double decodeFeed (double ) ;
long int program_format_H33(char *, int ) ;

long int program_list(char *);
int program_statistics_save(char *namefile, struct INFO_FILE_STAT );

long int program_statistics(char *,double , struct INFO_FILE_STAT *) ;

