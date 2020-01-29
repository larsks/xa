 
#include  <stdio.h>
#include  <ctype.h>
#include  <string.h>
#include  "xah.h"
#include  "xah2.h"

/* now in xah.h 
typedef struct {
     char *search;
     int  s_len;
     char *replace;
     int  p_anz;
     int  nextindex;
} List;
*/


int ll_such(char*,int*);
int ll_pdef(char*);
int tcompare(char*,char**,int);
int suchdef(char*);
int pp_replace(char*,char*,int,int);
void errout(int);
void logout(char*);
int b_term(char*,int*,int*,int);
int m_alloc(long,char**);
int fgetline(char*,FILE*);
int pp_comand(char*);
int icl_close(int*);
int rgetc(FILE*);
FILE *xfopen(const char *filename,const char *mode);

extern int pc;
char s[MAXLINE];
char *mem;
unsigned long memfre;
int nlf;
int nff;
static int hashindex[256];

#define   hashcode(n,l)  (n[0]&0x0f)|(((l-1)?(n[1]&0x0f):0)<<4)

int  icl_open(),pp_define(),pp_ifdef(),pp_ifndef(),pp_else(),pp_endif();
int  pp_echo(),pp_if(),pp_print(),pp_prdef(),pp_ifldef(),pp_iflused();
int  pp_undef();

#define   ANZBEF    13
#define   VALBEF    6

char *cmd[]={ "echo","include","define","undef","printdef","print",
			"ifdef","ifndef","else","endif",
               "ifldef","iflused","if" };
               
int (*func[])(char*) = { pp_echo,icl_open,pp_define,pp_undef,
			 pp_prdef,pp_print, pp_ifdef,pp_ifndef,
                         pp_else,pp_endif,
                         pp_ifldef,pp_iflused,pp_if };

List      *liste;
unsigned int       rlist;
unsigned long      rmem;
int       fsp;
int       loopfl;
Datei     flist[MAXFILE+1];
Datei     *filep;
char      in_line[MAXLINE];

pp_comand(t)
char *t;
{
     int i,l,er=1;

     i=tcompare(t,cmd,ANZBEF);

     if(i>=0)
     {
          if(loopfl && (i<VALBEF))
               er=0;
          else
          {
               l=(int)strlen(cmd[i]);
               while(isspace(t[l])) l++;

               er=(*func[i])(t+l);
          }
     }
     return(er);
}

int pp_ifdef(t)
char *t;
{
/*     int x;
     printf("t=%s\n",t);
     x=suchdef(t);
     printf("suchdef(t)=%d\n",x);
*/   
     loopfl=(loopfl<<1)+( suchdef(t) ? 0 : 1 );
     return(0);
}

int pp_ifndef(t)
char *t;
{
     loopfl=(loopfl<<1)+( suchdef(t) ? 1 : 0 );
     return(0);
}

int pp_ifldef(char *t)
{
	loopfl=(loopfl<<1)+( ll_pdef(t) ? 1 : 0 );
	return(0);
}

int pp_iflused(char *t)
{
	int n;
	loopfl=(loopfl<<1)+( ll_such(t,&n) ? 1 : 0 );
	return(0);
}

int pp_echo(t)
char *t;
{
     int er;
     
     if(er=pp_replace(s,t,-1,rlist))
          errout(er);
     else
     {
          logout(s);
          logout("\n");
     }
     return(0);
}

int pp_print(t)
char *t;
{
     int f,a,er;
     
     logout(t);
     if(er=pp_replace(s,t,-1,rlist))
     {
          logout("\n");
          errout(er);
     }
     else
     {
          logout("=");
          logout(s);
          logout("=");
          er=b_term(s,&a,&f,pc);
          if(er)
          {
               logout("\n");
               errout(er);
          }
          else
               { sprintf(s,"%d\n",a); logout(s); }
     }
     
     return(0);
}

int pp_if(t)
char *t;
{
     int a,f,l,er;

     if(er=pp_replace(s,t,-1,rlist))
          errout(er);
     else
     {

          f=b_term(s,&a,&l,pc);

          if(f)     
               errout(f);
          else
               loopfl=(loopfl<<1)+( a ? 0 : 1 );
     }
     return(0);
}

int pp_else(t)
char *t;
{
     loopfl ^=1;
     return(0);
}

int pp_endif(t)
char *t;
{
     loopfl=loopfl>>1;
     return(0);
}

/* pp_undef is a great hack to get it working fast... */
int pp_undef(char *t) {
     int i;
     if(i=suchdef(t)) {
	i+=rlist;
	liste[i].s_len=0;
     }
     return 0;
}

int pp_prdef(t)
char *t;
{
     char *x;
     int i,j;

     if(i=suchdef(t))
     {
          i+=rlist;
          x=liste[i].search;
          sprintf(s,"\n%s",x);
          if(liste[i].p_anz)
          {
               sprintf(s+strlen(s),"(");
               for(j=0;j<liste[i].p_anz;j++)
               {
                    x+=strlen(x)+1;
                    sprintf(s+strlen(s),"%s%c",x,(liste[i].p_anz-j-1) ? ',' : ')');
               }
          }
          sprintf(s+strlen(s),"=%s\n",liste[i].replace);
          logout(s);
     }
     return(E_OK);
}

int suchdef(t)
char *t;
{
     int i=0,j,k,l=0;

     while(t[l]!=' ' && t[l]!='\0') l++;

     if(rlist)
     {
       i=hashindex[hashcode(t,l)];
     
       do   /*for(i=0;i<rlist;i++)*/
       {
          k=liste[i].s_len;
          j=0;

          if(k && (k==l))
          {
               while((t[j]==liste[i].search[j])&&j<k) j++;
               if(j==k)
                    break;
          }
          
          if(!i)
          {
               i=rlist;
               break;
          }
          
          i=liste[i].nextindex;
               
       } while(1);
     } 
    
     return(i-rlist);
}

int ga_pp()
{
	return(rlist);
}

int gm_pp()
{
	return(ANZDEF);
}

long gm_ppm()
{
	return(MAXPP);
}

long ga_ppm()
{
	return(MAXPP-memfre);
}
    
int pp_define(k)
char *k;
{
     int i,er=E_OK,j,hash,rl;
     char h[MAXLINE*2],*t;
     
     t=k;
          
     if(rlist>=ANZDEF || memfre<MAXLINE*2)
          return(E_NOMEM);
/*
     printf("define:mem=%04lx\n",mem);
     getchar();
*/   
     rl=rlist++;
     
     liste[rl].search=mem;
     for(i=0; (t[i]!=' ') && (t[i]!='\0') && (t[i]!='(') ;i++)
          *mem++=t[i];
     *mem++='\0';
     memfre-=i+1;
     liste[rl].s_len=i;
     liste[rl].p_anz=0;
/*
     printf("define:%s\nlen1=%d\n",liste[rl].search,liste[rl].s_len);
     getchar();
*/ 
     if(t[i]=='(')
     {
          while(t[i]!=')' && t[i]!='\0')
          {
               i++;
               liste[rl].p_anz++;
               for(j=0; t[i+j]!=')' && t[i+j]!=',' && t[i+j]!='\0';j++);
               if(j<memfre)
               {
                    strncpy(mem,t+i,j);
                    mem+=j+1;
                    memfre-=j+1;
               }
               i+=j;
          }
          if(t[i]==')')
               i++;
     }
     while(t[i]==' ')
          i++;
     t+=i;
     
     pp_replace(h,t,-1,0);

     t=h;     

     liste[rl].replace=mem;
     strcpy(mem,t);
     mem+=strlen(t)+1;
/*
     if(liste[rl].p_anz)
     {
          s=liste[rl].search;
          printf("define:\n%s(",liste[rl].search);
          for(j=0;j<liste[rl].p_anz;j++)
          {
               s+=strlen(s)+1;
               printf("%s%c",s,(liste[rl].p_anz-j-1) ? ',' : ')');
          }
          printf("=%s\n",liste[rl].replace);
          getchar();
     }
     else
     {
          printf("define:%s=%s\nlen1=%d\n",liste[rl].search,
               liste[rl].replace,liste[rl].s_len);
     }
*/
     if(!er)
     {
          hash=hashcode(liste[rl].search,liste[rl].s_len);
          liste[rl].nextindex=hashindex[hash];
          hashindex[hash]=rl;
     } else
          rlist=rl;
     
     return(er);
}

int tcompare(s,v,n)
char s[],*v[];
int n;
{
     int i,j,l;
     static char t[MAXLINE];

     for(i=0; s[i]!='\0'; i++) 
          t[i]=tolower(s[i]);
     t[i]='\0';

     for(i=0;i<n;i++)
     {
          l=(int)strlen(v[i]);
          
          for(j=0;j<l;j++)
          {
               if(t[j]!=v[i][j])
                    break;
          }
          if(j==l)
               break;
     }
     return((i==n)? -1 : i);
}

int pp_replace(to,ti,a,b)
char *ti,*to;
int a,b;
{
     char *t=to,c,*x,*y,*mx,*rs;
     int i,l,n,sl,d,ld,er=E_OK,hkfl,klfl;
     char fti[MAXLINE],fto[MAXLINE];
/*
     int flag=!strncmp(ti,"TOUT",4);
     if(flag) printf("flag=%d\n",flag);
*/   
     strcpy(t,ti);

     if(rlist)
     {
       while(t[0]!='\0')
       {
          while(!isalpha(t[0]) && t[0]!='_')
               if(t[0]=='\0')
                    break;    /*return(E_OK);*/
               else
               {
                    t++;
                    ti++;
               }
         
          for(l=0;isalnum(t[l])||t[l]=='_';l++);
          ld=l;
/*          
          if(flag) printf("l=%d,a=%d,t=%s\n",l,a,t);
*/        
          if(a<0)
          {
            n=hashindex[hashcode(t,l)];
            
            do      
            {
               sl=liste[n].s_len;
          
               if(sl && (sl==l))
               {
                    i=0;
                    x=liste[n].search;
                    while(t[i]==*x++ && t[i])
                         i++;

                    if(i==sl)
                    {     
                         rs=liste[n].replace;
                         
                         if(liste[n].p_anz)        
                         {
                              strcpy(fti,liste[n].replace);

                              if(rlist+liste[n].p_anz>=ANZDEF || memfre<MAXLINE*2)
                                   er=E_NOMEM;
                              else
                              {
                                   y=t+sl;
                                   x=liste[n].search+sl+1;
                                   if(*y!='(')
                                        er=E_SYNTAX;
                                   else
                                   {
                                        mx=mem-1;
                                        for(i=0;i<liste[n].p_anz;i++)
                                        {
                                             liste[rlist+i].search=x;
                                             liste[rlist+i].s_len=(int)strlen(x);
                                             x+=strlen(x)+1;
                                             liste[rlist+i].p_anz=0;
                                             liste[rlist+i].replace=mx+1;
                                             c=*(++mx)=*(++y);
                                             hkfl=klfl=0;
                                             while(c!='\0' 
                                                  && ((hkfl!=0 
                                                       || klfl!=0) 
                                                       || (c!=',' 
                                                       && c!=')') 
                                                       )
                                                  )
                                             {
                                                  if(c=='\"')
                                                       hkfl=hkfl^1;
                                                  if(!hkfl)
                                                  {
                                                       if(c=='(')
                                                            klfl++;
                                                       if(c==')')
                                                            klfl--;
                                                  }     
                                                  c=*(++mx)=*(++y);
                                             }
                                             *mx='\0';
                                             if(c!=((i==liste[n].p_anz-1) ? ')' : ','))
                                             {
                                                  er=E_ANZPAR;
                                                  break;
                                             }
                                        }   
/*
     printf("replace:\n");
     printf("%s=%s\n",liste[n].search,liste[n].replace);
     for(i=0;i<liste[n].p_anz;i++)
          printf("-%s=%s\n",liste[rlist+i].search,liste[rlist+i].replace);
*/
                                        if(!er)
                                             er=pp_replace(fto,fti,rlist,rlist+i);
/*               if(flag) printf("sl=%d,",sl);*/
                                        sl=(int)((long)y+1L-(long)t);
/*               if(flag) printf("sl=%d\n",sl);*/
                                        rs=fto;
/*     printf("->%s\n",fto);*/
                                   }    
                              }
                              if(er)
                                   return(er);     
                         }

                         d=(int)strlen(rs)-sl;

                         if(strlen(to)+d>=MAXLINE)
                              return(E_NOMEM);

/*
                         if(d<0)
                         {
                              y=t+sl+d;
                              x=t+sl;
                              while(*y++=*x++);
                         }
                         if(d>0)
                         {
                              for(ll=strlen(t);ll>=sl;ll--)
                                   t[ll+d]=t[ll];
                         }
*/
                         if(d)
                              strcpy(t+sl+d,ti+sl);

                         i=0;
                         while(c=rs[i])
                              t[i++]=c;
                         l=sl+d;/*=0;*/
                         break;
                    }
               }
               if(!n)
                    break;
                    
               n=liste[n].nextindex;
               
            } while(1);
          } else
          {
            for(n=b-1;n>=a;n--)
            {
               sl=liste[n].s_len;
          
               if(sl && (sl==l))
               {
                    i=0;
                    x=liste[n].search;
                    while(t[i]==*x++ && t[i])
                         i++;

                    if(i==sl)
                    {     
                         rs=liste[n].replace;
/*                         
                         if(liste[n].p_anz)        
                         {
                              strcpy(fti,liste[n].replace);
                              if(rlist+liste[n].p_anz>=ANZDEF || memfre<MAXLINE*2)
                                   er=E_NOMEM;
                              else
                              {
                                   y=t+sl;
                                   x=liste[n].search+sl+1;
                                   if(*y!='(')
                                        er=E_SYNTAX;
                                   else
                                   {
                                        mx=mem-1;
                                        for(i=0;i<liste[n].p_anz;i++)
                                        {
                                             liste[rlist+i].search=x;
                                             liste[rlist+i].s_len=strlen(x);
                                             x+=strlen(x)+1;
                                             liste[rlist+i].p_anz=0;
                                             liste[rlist+i].replace=mx+1;
                                             c=*(++mx)=*(++y);
                                             hkfl=klfl=0;
                                             while(c!='\0' 
                                                  && ((hkfl!=0 
                                                       || klfl!=0) 
                                                       || (c!=',' 
                                                       && c!=')') 
                                                       )
                                                  )
                                             {
                                                  if(c=='\"')
                                                       hkfl=hkfl^1;
                                                  if(!hkfl)
                                                  {
                                                       if(c=='(')
                                                            klfl++;
                                                       if(c==')')
                                                            klfl--;
                                                  }     
                                                  c=*(++mx)=*(++y);
                                             }
                                             *mx='\0';
                                             if(c!=((i==liste[n].p_anz-1) ? ')' : ','))
                                             {
                                                  er=E_ANZPAR;
                                                  break;
                                             }  
                                        }   
                                        if(!er)
                                             er=pp_replace(fto,fti,rlist,rlist+i);
                                        sl=(int)((long)y+1L-(long)t);
                                        rs=fto;
                                   }    
                              }
                              if(er)
                                   return(er);     
                         }
*/
                         d=(int)strlen(rs)-sl;

                         if(strlen(to)+d>=MAXLINE)
                              return(E_NOMEM);
/*
                         if(d<0)
                         {
                              y=t+sl+d;
                              x=t+sl;
                              while(*y++=*x++);
                         }
                         if(d>0)
                         {
                              for(ll=strlen(t);ll>=sl;ll--)
                                   t[ll+d]=t[ll];
                         }
*/
                         if(d)
                              strcpy(t+sl+d,ti+sl);
                              
                         i=0;
                         while(c=rs[i])
                              t[i++]=c;
                         l+=d;/*0;*/
                         break;
                    }
               }
            }
          }
          ti+=ld;
          t+=l;
       }
     }
     return(E_OK);
}

int pp_init()
{
     int er;

     for(er=0;er<256;er++)
          hashindex[er]=0;
          
     fsp=0;
     er=m_alloc(MAXPP,&mem);
     memfre=MAXPP;
     rlist=0;
     nlf=1;
     nff=1;
     if(!er)
          er=m_alloc((long)ANZDEF*sizeof(List),(char**)&liste);
     return(er);
}

int pp_open(name)
char *name;
{
     FILE *fp;

     fp=xfopen(name,"r");
		
     strcpy(flist[0].fname,name);
     flist[0].fline=0;
     flist[0].filep=fp;
     flist[0].flinep=NULL;    

     return(((long)fp)==0l);
}

void pp_close()
{
     fclose(flist[fsp].filep);
}

void pp_end() { }

int icl_close(c)
int *c;
{
     if(!fsp)
          return(E_EOF);
     
     fclose(flist[fsp--].filep);
     nff=1;
     *c='\n';

     return(E_OK);
}

int icl_open(t)
char *t;
{
     FILE *fp2;
     int j,i=0;

     if(fsp>=MAXFILE)
          return(-1);

     if(t[i]=='<' || t[i]=='"')
          i++;

     for(j=i;t[j];j++)
          if(t[j]=='>' || t[j]=='"')
               t[j]='\0';

     fp2=xfopen(t+i,"r");

     if(!fp2)
          return(E_FNF);

	setvbuf(fp2,NULL,_IOFBF,BUFSIZE);
	
     fsp++;

     strcpy(flist[fsp].fname,t+i);
     flist[fsp].fline=0;
     flist[fsp].flinep=NULL;
     flist[fsp].filep=fp2;
     nff=1;

     return(0);
}

int pgetline(t)
char *t;
{
     int c,er=E_OK;
     loopfl =0;

     filep =flist+fsp;

     do {
          c=fgetline(in_line,flist[fsp].filep);

          if(in_line[0]=='#')
          {
               if(er=pp_comand(in_line+1))
               {
                    if(er!=1)
                    {
                         logout(in_line);
                         logout("\n");
                    }
               }
          } else
               er=1;

          if(c==EOF)
               er=icl_close(&c);

     } while(!er || (loopfl && er!=E_EOF));

     if(!er || loopfl)
          in_line[0]='\0';
     er= (er==1) ? E_OK : er ;

     if(!er)
          er=pp_replace(t,in_line,-1,rlist);

     if(!er && nff)
          er=E_NEWFILE;
     if(!er && nlf)
          er=E_NEWLINE;
     nlf=nff=0;

     filep=flist+fsp;
     filep->flinep=in_line;
     
     return(er);
}


/*************************************************************************/

int rgetc(fp)
FILE *fp;
{
     static int c,d,fl;

     fl=0;

     do
     {
          while((c=getc(fp))==13);  /* remove ^M for unices */

          if(fl && (c=='*'))
          {
               if((d=getc(fp))!='/')
                    ungetc(d,fp);
               else
               {
                    fl--;
                    while((c=getc(fp))==13);
               }
          }

          if(c=='\n')
          {
               flist[fsp].fline++;
               nlf=1;
          } else
          if(c=='/')
          {
               if((d=getc(fp))!='*')
                    ungetc(d,fp);
               else
                    fl++;
          }

     } while(fl && (c!=EOF));

     return(c-'\t'?c:' ');
}

fgetline(t,fp)
char *t;
FILE *fp;
{
     static int c,i;

     i=0;

     do {
          c=rgetc(fp);
          
          if(c==EOF || c=='\n')
          {
               t[i]='\0';
               break;
          }
          t[i]=c;
          i= (i<MAXLINE-1) ? i+1 : MAXLINE-1;
     } while(1);

     return(c);
}



