
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "xah.h"
#include "xah2.h"

const char *copyright={
"Cross-Assembler 65xx V2.07d 20mar96 (c) 1989-96 by A.Fachat\n"};

time_t tim1,tim2;
char out[MAXLINE];
extern char s[MAXLINE];
extern Datei *filep;
extern char *lz;
FILE *fpout,*fp,*fperr,*fplab;
int getline();
extern int pc;
int ner;
int ncmos,cmosfl;

void printstat(void);
void logout(char*);
void usage(void);
int setfext(char*,char*);
int  m_init(void);
void m_exit(void);
int  b_init(void);
int  l_init(void);
int  pp_init(void);
int  x_init(void);
int  pp_open(char*);
void pp_close(void);
void pp_end(void);
int  pass1(void);
int  pass2(void);
void printllist(FILE*);
int  puttmp(int);
int  puttmps(char*,int);
int  t_p2(char*,int*,int);
int  t_p1(char*,char*,int*);
void chrput(int);
void errout(int);
int  getline(char*);
void lineout(void);
int  m_alloc(long,char**);
int  pgetline(char*);
FILE *xfopen(const char *filename,const char *mode);

int main(argc,argv)
int argc;
char *argv[];
{
     int er=1,i;
     char ofile[MAXLINE];
     char efile[MAXLINE];
     char lfile[MAXLINE];
     char ifile[MAXLINE];

	tim1=time(NULL);
     
     ncmos=0;
     cmosfl=1;

	printf("%s",copyright);
	
     if(argc<=1)
     {
          usage();
          return(1);
     }
     
     strcpy(ofile,argv[1]);
     strcpy(efile,argv[1]);
     strcpy(lfile,argv[1]);

     setfext(ofile,".obj");
     setfext(lfile,".lab");
     setfext(efile,".err");

     for(i=1; i<argc; i++)
          if(argv[i][0]=='-' && tolower(argv[i][1])=='o')
               strcpy(ofile,argv[i]+2);

     for(i=1; i<argc; i++)
          if(argv[i][0]=='-' && tolower(argv[i][1])=='e')
               strcpy(efile,argv[i]+2);

     for(i=1; i<argc; i++)
          if(argv[i][0]=='-' && tolower(argv[i][1])=='l')
               strcpy(lfile,argv[i]+2);

     for(i=1; i<argc; i++)
          if(argv[i][0]=='-' && tolower(argv[i][1])=='c')
               cmosfl =0;

     if(!m_init())
     {
/*printf("memory ok\n"); getchar();*/
      if(!b_init())
      {
/*printf("block ok\n"); getchar();*/
       if(!l_init())
       {
/*printf("label ok\n"); getchar();*/
        if(!pp_init())
        {
/*printf("pp ok\n"); getchar();*/
         if(!x_init())
         {
/*printf("x ok\n"); getchar();*/
/*printf("ofile=%s\n",ofile);*/
          fpout=xfopen(ofile,"wb");

          if (fpout)
          { 
/*printf("fpout!=0\n"); getchar();*/
           fplab=xfopen(lfile,"w");
            
           if(fplab)
           {
/*printf("fplab!=0\n"); getchar();*/
            fperr=xfopen(efile,"w");

            if(fperr)
            {
/*printf("fperr!=0\n"); getchar();*/

			fprintf(fperr,"%s",copyright);
			logout(ctime(&tim1));

/* Hier beginnt Pass 1                                                */

               pc   =0;

               for (i=1; i<argc; i++)
               {
                    if (isalpha(argv[i][0]))
                    {
                         strcpy(ifile,argv[i]);
                         setfext(ifile,".a65");

                         sprintf(out,"xAss65: Pass 1: %s\n",ifile);
                         logout(out);

                         er=pp_open(ifile);
/*printf("pp_open=%d\n",er); getchar();*/
                         if(!er)
                         {
                              er=pass1();
                              pp_close();
                         }
                         else
                            /*  logout("Kann Quell-Datei nicht ”ffnen !\n");*/
                              logout("Couldn't open source file!\n");
                    }
               }           

               if(!er)
               {
                    logout("xAss65: Pass 2:\n");

                    er=pass2();
               } 

               printllist(fplab);
               tim2=time(NULL);
			printstat();
			                              
               fclose(fperr);
            }
            else
               /* logout("Kann Log-Datei nicht ”ffnen!\n");*/
               logout("Couldn't open log file!\n");
                  
            fclose(fplab);
           }
           else
              /* logout("Kann Label-Datei nicht ”ffnen!\n");*/
              logout("Couldn't open label file!\n");
               
           fclose(fpout);
          }
          else
               /* logout("Kann Ausgabe-Datei nicht ”ffnen!\n");*/
               logout("Couldn't open output file!\n");

          pp_end();
         } else
         {
          /* logout("X: Kein Speicher\n");*/
          logout("fatal: x: no memory!\n");
         }
        } else
        {
          /* logout("PP: Kein Speicher\n");*/
          logout("fatal: pp: no memory!");
          getchar();
        }
       } else
       {
          /* logout("L: Kein Speicher\n");*/
          logout("fatal: l: no memory!\n");
          getchar();
       }
      } else
      {
          /* logout("B: Kein Speicher\n");*/
          logout("fatal: b: no memory!\n");
          getchar();
      }
      m_exit();
     } else
     { 
          /* logout("Nicht genug Speicher vorhanden\n");*/
          logout("Not enough memory available!\n");
          getchar();
     }

     if(er)
     {
          /* printf("Abbruch nach Fehler !\nWeiter mit <Return>\n"); */
          printf("Break after error\ngo on with <return>\n");
          getchar();
     }

     return( er ? 1 : 0 );
}

void printstat()
{
	int ga_lab(void),gm_lab(void),ga_blk(void),ga_pp(void),gm_pp(void);
	long ga_ppm(void),gm_ppm(void),ga_labm(void),gm_labm(void);
	long ga_p1(void),gm_p1(void);
	
	logout("Statistics:\n");
/*
	sprintf(out," %8d von %8d Labels benutzt\n",ga_lab(),gm_lab()); logout(out);
	sprintf(out," %8ld von %8ld Byte Label-Speicher benutzt\n",ga_labm(),gm_labm()); logout(out);
	sprintf(out," %8d von %8d PP-Defs benutzt\n",ga_pp(),gm_pp()); logout(out);
	sprintf(out," %8ld von %8ld Byte PP-Speicher benutzt\n",ga_ppm(),gm_ppm()); logout(out);
	sprintf(out," %8ld von %8ld Byte Zwischenspeicher benutzt\n",ga_p1(),gm_p1()); logout(out);
	sprintf(out," %8d Blocks benutzt\n",ga_blk()); logout(out);
	sprintf(out," %8ld Sekunden ben”tigt\n",(long)difftime(tim2,tim1)); logout(out);
*/
	sprintf(out," %8d of %8d label used\n",ga_lab(),gm_lab()); logout(out);
	sprintf(out," %8ld of %8ld byte label-memory used\n",ga_labm(),gm_labm()); logout(out);
	sprintf(out," %8d of %8d PP-defs used\n",ga_pp(),gm_pp()); logout(out);
	sprintf(out," %8ld of %8ld byte PP-memory used\n",ga_ppm(),gm_ppm()); logout(out);
	sprintf(out," %8ld of %8ld byte buffer memory used\n",ga_p1(),gm_p1()); logout(out);
	sprintf(out," %8d blocks used\n",ga_blk()); logout(out);
	sprintf(out," %8ld seconds used\n",(long)difftime(tim2,tim1)); logout(out);
}

int setfext(s,ext)
char *s,*ext;
{
     int j,i=(int)strlen(s);

     if(i>MAXLINE-5)
          return(-1);
          
     for(j=i-1;j>=0;j--)
     {
          if(s[j]==DIRCHAR)
          {
               strcpy(s+i,ext);
               break;
          }
          if(s[j]=='.')
          {
               strcpy(s+j,ext);
               break;
          }
     }
     if(!j)
          strcpy(s+i,ext);

     return(0);
}

char *tmp;
unsigned long tmpz;
unsigned long tmpe;

long ga_p1()
{
	return(tmpz);
}
long gm_p1()
{
	return(TMPMEM);
}

#define abs(a) ((a)>=0 ? a : -a)

int pass2()
{
     int c,er,l,ll,i;
     Datei datei;

     filep=&datei;
     ner=0;
     tmpe=0L;

     while(ner<20 && tmpe<tmpz)
     {
          l=tmp[tmpe++];
          ll=l;

          if(!l)
          {
               if(tmp[tmpe]==T_LINE)
               {
                    datei.fline=(tmp[tmpe+1]&255)+(tmp[tmpe+2]<<8);
                    tmpe+=3;
               } else
               if(tmp[tmpe]==T_FILE)
               {
                    datei.fline=(tmp[tmpe+1]&255)+(tmp[tmpe+2]<<8);
                    strcpy(datei.fname,tmp+tmpe+3);
                    tmpe+=3+strlen(datei.fname);
               }
          } else
          {
               er=t_p2(tmp+tmpe,&ll,0);
          
               if(er==E_NOLINE)
               {
               } else
               if(er==E_OK)
               {
                    for(i=0;i<ll;i++)
                         chrput(tmp[tmpe+i]);
               } else
               if(er==E_DSB)
               {
                    /*printf("E_DSB, ll=%d, l=%d, c=%c\n",ll,l,tmp[tmpe]);*/
                    c=tmp[tmpe];
                    for(i=0;i<ll;i++)
                         chrput(c);
               } else
               {
                    errout(er);
               }
          }
          tmpe+=abs(l);
     }
     return(ner);
}     
     

int pass1()
{
     static char o[MAXLINE];
     static int l,er;
/*     int i;*/

     ner=0;

     while(!(er=getline(s)))
     {         

          er=t_p1(s,o,&l);

/*          printf(": er= %d, l=%d, tmpz=%d\n",er,l,tmpz);
*/
          if(l)
          {
            if(er)
            {
               if(er==E_OKDEF)
               {
                    if(!(er=puttmp(l)))
                         er=puttmps(o,l);
               } else
               if(er==E_NOLINE)
                    er=E_OK;
            } else
            {
               if(!(er=puttmp(-l)))
                    er=puttmps(o,l);
            }
          }
          if(er)
          {
               lineout();
               errout(er);
          }

/*          printf("tmpz =%d\n",tmpz);
*/
     } 

     if(er!=E_EOF)
          errout(er);

          

     /*printf("Pass 1 \n");
     for(i=0;i<tmpz;i++)
          printf(" %02x",tmp[i]);
     getchar();*/

     return(ner);
}


void usage()
{
     printf("xAss65 usage : Sourcefile1 [ ... Sourcefilen]\n"
            "         [-oObjectfile] [-eErrorfile] [-lLabelfile]\n"
            "further options:\n"
            " -c      =  no CMOS-opcodes\n"
            "Environment:\n"
            " XAINPUT = include files path\n"
            " XAOUTPUT= output files path\n"
            "press <Return>\n");
     getchar();
}

#define   ANZERR    25

/*
static char *ertxt[] = { "Syntax","Label definiert",
          "Label nicht definiert","Labeltabelle voll",
          "Label erwartet","Speicher voll","Illegaler Opcode",
          "Falsche Adressierungsart","Branch aužerhalb des Bereichs",
          "šberlauf","Division durch Null","Pseudo-Opcode erwartet",
          "Block-Stack-šberlauf","Datei nicht gefunden",
          "End of File","Block-Struktur nicht abgeschlossen",
          "NoBlk","NoKey","NoLine","OKDef","DSB","NewLine",
          "NewFile","CMOS-Befehl","pp:Falsche Anzahl Parameter" };
*/
static char *ertxt[] = { "Syntax","Label defined",
          "Label not defined","Labeltab full",
          "Label expected","no more memory","Illegal opcode",
          "Wrong addressing mode","Branch out of ranges",
          "Overflow","Division by zero","Pseudo-opcode expected",
          "Block stack overflow","file not found",
          "End of file","Block structure not closed",
          "NoBlk","NoKey","NoLine","OKDef","DSB","NewLine",
          "NewFile","CMOS-Befehl","pp:Wrong parameter count" };

int gl;
int gf;  

int x_init()
{
     int er;
     gl=0;
     gf=0;    
     er=m_alloc(TMPMEM,&tmp);
     tmpz=0L;
     return(er);
}

puttmp(c)
int c;
{
     int er=E_NOMEM;
     if(tmpz<TMPMEM)
     {
          tmp[tmpz++]=c;
          er=E_OK;
     }
     return(er);
}

puttmps(s,l)
char *s;
int l;
{
     int i=0,er=E_NOMEM;
     
     if(tmpz+l<TMPMEM)
     {
          while(i<l)
               tmp[tmpz++]=s[i++];

          er=E_OK;
     }
     return(er);
}

char l[MAXLINE];

int getline(s)
char *s;
{
     static int ec;
     static int i,c;
     int hkfl,j;

     j=hkfl=0;
     ec=E_OK;

     if(!gl)
     {
          do
          {
               ec=pgetline(l);
               i=0;
               while(l[i]==' ')
                    i++;
               while(l[i]!='\0' && isdigit(l[i]))
                    i++;
               gf=1;

               if(ec==E_NEWLINE)
               {
                    puttmp(0);
                    puttmp(T_LINE);
                    puttmp((filep->fline)&255);
                    puttmp(((filep->fline)>>8)&255);
                    ec=E_OK;
               } else
               if(ec==E_NEWFILE)
               {
                    puttmp(0);
                    puttmp(T_FILE);
                    puttmp((filep->fline)&255);
                    puttmp(((filep->fline)>>8)&255);
                    puttmps(filep->fname,1+(int)strlen(filep->fname));
                    ec=E_OK;
               }
          } while(!ec && l[i]=='\0');
     }

     gl=0;
     if(!ec)
     {
          do {
               c=s[j]=l[i++];

               if (c=='\"')
                    hkfl^=1;
               if (c=='\0')
                    break;
               if (c==':' && !hkfl)
               {
                    gl=1;
                    break;
               }
               j++;
          } while (c!='\0' && j<MAXLINE-1 && i<MAXLINE-1);
     
          s[j]='\0';
     } else
          s[0]='\0';

     return(ec);
}

void lineout()
{
     if(gf)
     {
          logout(filep->flinep);
          logout("\n");
          gf=0;
     }
}

void errout(er)
int er;
{
     if (er<-ANZERR || er>-1)
          /* sprintf(out,"%s:Zeile %d: %04x:Unbekannter Fehler Nr.: %d\n",*/
          sprintf(out,"%s:line %d: %04x: Unknown error # %d\n",
               filep->fname,filep->fline,pc,er);
     else 
     if (er==E_NODEF)
          sprintf(out,"%s:line %d: %04x:Label '%s' not defined\n",
               filep->fname,filep->fline,pc,lz);
     else  
          sprintf(out,"%s:line %d: %04x:%s error\n",
               filep->fname,filep->fline,pc,ertxt[-er-1]);

     logout(out);
 
     ner++;
}

void chrput(c)
int c;
{
     /*     printf(" %02x",c&255);*/

     putc( c&0x00ff,fpout);
}

void logout(s)
char *s;
{
     printf("%s",s);
     if(fperr)
          fprintf(fperr,"%s",s);
}

