 
#include <stdio.h>
#include <ctype.h>
#include "xah.h"

extern int m_alloc(long,char**);
int b_fget(int*,int);
int ll_such(char*,int*);
int b_ltest(int,int);
int ll_def(char*,int*,int);
int l_get(int,int*);
int b_get(int*);
int b_test(int);
void ll_exblk(int,int);

/* now in xah.h
typedef struct {
     int blk;
     int val;
     int len;
     int fl;  
     int nextindex;
     char *n;
} Labtab;
*/

#define   hashcode(n,l)  (n[0]&0x0f)|(((l-1)?(n[1]&0x0f):0)<<4)

static int hashindex[256];
char   *lt;
char   *ln;
char   *lz;
int    lti;
unsigned long lni;
long   sl;
Labtab *ltp;

int l_init()
{
     int er;

     for(er=0;er<256;er++)
          hashindex[er]=0;

     sl=(long)sizeof(Labtab);
     
     if(!(er=m_alloc((long)(sizeof(Labtab)*ANZLAB),&lt)))
          er=m_alloc((long)LABMEM,&ln);
          
     lti=0;
     lni=0L;

     return(er);
}

int ga_lab()
{
	return(lti);
}

int gm_lab()
{
	return(ANZLAB);
}

long gm_labm()
{
	return((long)LABMEM);
}

long ga_labm()
{
	return(lni);
}

void printllist(fp)
FILE *fp;
{
     int i;
     
     for(i=0;i<lti;i++)
     {
          ltp=(Labtab*)(lt+i*sl);
          fprintf(fp,"%s, %d, %d\n",ltp->n,ltp->blk,ltp->val);
     }
}
          
l_def(s,l,x,f)
char *s;
int *l,*f,*x;
{     
     int n,er,b,i=0;
 
     *f=0;
     b=0;
     n=0;

     if(s[0]=='-')
     {
          *f+=1;
          i++;
     } else
     if(s[0]=='+')
     {
          i++;
          n++;
          b=0;
     } 
     while(s[i]=='&')
     {
          n=0;     
          i++;
          b++;
     }
     if(!n)
          b_fget(&b,b);


     if(!isalpha(s[i]) && s[i]!='_')
          er=E_SYNTAX;
     else
     {
          er=ll_such(s+i,&n);
               
          if(er==E_OK)
          {
               ltp=(Labtab*)(lt+n*sl);
               
               if(*f)
               {
                    *l=ltp->len+i;
               } else
               if(ltp->fl==0)
               {
                    *l=ltp->len+i;
                    if(b_ltest(ltp->blk,b))
                         er=E_LABDEF;
                    else
                         ltp->blk=b;

               } else
                    er=E_LABDEF;
          } else
          if(er==E_NODEF)
          {
               if(!(er=ll_def(s+i,&n,b))) /* ll_def(...,*f) */
               {
                    ltp=(Labtab*)(lt+n*sl);
                    *l=ltp->len+i;
                    ltp->fl=0;
               }
          } 

          *x=n;
     }
     return(er);
}

l_such(s,l,x,v)
char *s;
int *l,*v,*x;
{
     int n,er,b;

     er=ll_such(s,&n);
     if(er==E_OK)
     {
          ltp=(Labtab*)(lt+n*sl);
          *l=ltp->len;
          if(ltp->fl)
          {
               l_get(n,v);/*               *v=lt[n].val;*/
               *x=n;
          } else
          {
               er=E_NODEF;
               lz=ltp->n;
               *x=n;
          }
     }
     else
     {
          b_get(&b);
          er=ll_def(s,x,b); /* ll_def(...,*v); */

          ltp=(Labtab*)(lt+(*x)*sl);
          
          *l=ltp->len;

          if(!er) 
          {
               er=E_NODEF;
               lz=ltp->n;   
          }
     }
     return(er);
}

int l_get(n,v)
int n,*v;
{
     ltp=(Labtab*)(lt+n*sl);
     (*v)=ltp->val;
     lz=ltp->n;
     return( ltp->fl ? E_OK : E_NODEF);
}

void l_set(n,v)
int n,v;
{
     ltp=(Labtab*)(lt+n*sl);
     ltp->val = v;
     ltp->fl = 1;
}

void ll_exblk(a,b)
int a,b;
{
     int i;
     for (i=0;i<lti;i++)
     {
          ltp=(Labtab*)(lt+i*sl);
          if((!ltp->fl) && (ltp->blk==a))
               ltp->blk=b;
     }
}

ll_def(s,n,b)          /* definiert naechstes Label  nr->n     */     
char *s;
int *n,b;	/*,v;*/
{
     int j=0,er=E_NOMEM,hash;
     char *s2;
     
     if((lti<ANZLAB)&&(lni<(long)(LABMEM-MAXLAB)))
     {
          ltp=(Labtab*)(lt+lti*sl);
          
          s2=ltp->n=ln+lni;

          while((j<MAXLAB-1) && (s[j]!='\0') && (isalnum(s[j]) || s[j]=='_'))
          {
               s2[j]=s[j];
               j++;
          }

          if(j<MAXLAB)
          {
               er=E_OK;
               s2[j]='\0';
               ltp->len=j;
               ltp->blk=b;
               ltp->fl=0;
               hash=hashcode(s,j); 
               ltp->nextindex=hashindex[hash];
               hashindex[hash]=lti;
               *n=lti;
               lti++;
               lni+=j+1;
          }
     }
     return(er);
}


int ll_such(s,n)          /* such Label in Tabelle ,nr->n    */
char *s;
int *n;
{
     int i,j=0,k,er=E_NODEF,hash;

     while(isalnum(s[j])||(s[j]=='_'))  j++;

     hash=hashcode(s,j);
     i=hashindex[hash];

     do
     {
          ltp=(Labtab*)(lt+i*sl);
          
          if(j==ltp->len)
          {
               for (k=0;(k<j)&&(ltp->n[k]==s[k]);k++);

               if((j==k)&&(!b_test(ltp->blk)))
               {
                    er=E_OK;
                    break;
               }
          }

          if(!i)
               break;          

          i=ltp->nextindex;
          
     }while(1);
     
     *n=i;
/*
     if(er!=E_OK && er!=E_NODEF)
     {
          printf("Fehler in ll_such:er=%d\n",er);
          getchar();
     }
*/
     return(er);
}

int ll_pdef(char *t)
{
	int n;
	
	if(ll_such(t,&n)==E_OK)
	{
		ltp=(Labtab*)(lt+n*sl);
		if(ltp->fl)
			return(E_OK);
	}
	return(E_NODEF);
}

int bt[MAXBLK];
int blk;
int bi;

int b_init()
{
     blk =0;
     bi =0;
     bt[bi]=blk;

     return(E_OK);
}     

int ga_blk()
{
	return(blk);
}

int b_open()
{
     int er=E_BLKOVR;

     if(bi<MAXBLK-1)
     {
          bt[++bi]=++blk;
          
          er=E_OK;  
     }
     return(er);
}

int b_close()
{

     if(bi)
     {
          ll_exblk(bt[bi],bt[bi-1]);
          bi--;
     }

     return(E_OK);
}

int b_get(n)
int *n;
{
     *n=bt[bi];

     return(E_OK);
}

int b_fget(n,i)
int *n,i;
{
     if((bi-i)>=0)
          *n=bt[bi-i];
     else
          *n=0;
     return(E_OK);
}

int b_test(n)
int n;
{
     int i=bi;

     while( i>=0 && n!=bt[i] )
          i--;

     return( i+1 ? E_OK : E_NOBLK );
}

int b_ltest(a,b)    /* testet ob bt^-1(b) in intervall [0,bt^-1(a)]   */
int a,b;
{
     int i=0,er=E_OK;

     if(a!=b)
     {
          er=E_OK;

          while(i<=bi && b!=bt[i])
          {
               if(bt[i]==a)
               {
                    er=E_NOBLK;
                    break;
               }
               i++;
          }
     }
     return(er);
}

