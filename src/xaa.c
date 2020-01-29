 
#include "xah.h"

int pr[]= { P_START,P_ADD,P_ADD,P_MULT,P_MULT,P_SHIFT,P_SHIFT,P_CMP,
            P_CMP,P_EQU,P_CMP,P_CMP,P_EQU,P_AND,P_XOR,P_OR,
            P_LAND,P_LOR };

int pp,pcc;

int ag_term(char*,int,int*);
extern int l_get(int,int*);
int get_op(char*,int*);
int do_op(int*,int,int);

#define   cval(s)   256*((s)[1]&255)+((s)[0]&255)

int a_term(s,v,l,xpc)
char *s;
int *v,*l,xpc;
{
     int er=E_OK;

     pp=0;
     pcc=xpc;

     if(s[0]=='<')
     {
          pp++;
          er=ag_term(s,P_START,v);
          *v = *v & 255;
     } else
     if(s[0]=='>')
     {    
          pp++;
          er=ag_term(s,P_START,v);
          *v=(*v>>8)&255;
     }
     else
          er=ag_term(s,P_START,v);

     *l=pp;
     return(er);
}

ag_term(s,p,v)
char *s;
int p,*v;
{
     int er=E_OK,o,w,mf=1;

     while(s[pp]=='-')
     {
          pp++;
          mf=-mf;
     }

     if(s[pp]=='(')
     {
          pp++;
          if(!(er=ag_term(s,P_START,v)))
          {
               if(s[pp]!=')')
                    er=E_SYNTAX;
               else 
                    pp++;
          }
     } else
     if(s[pp]==T_LABEL)
     {
          er=l_get(cval(s+pp+1),v);
          pp+=3;
     }
     else
     if(s[pp]==T_VALUE)
     {
          *v=cval(s+pp+1);
          pp+=3;
     }
     else
     if(s[pp]=='*')
     {
          *v=pcc;
          pp++;
     }
     else
          er=E_SYNTAX;

     *v *= mf;

     while(!er && s[pp]!=')' && s[pp]!=',' && s[pp]!=T_END)
     {
          er=get_op(s,&o);

          if(!er && pr[o]>p)
          {
               pp+=1;
               if(!(er=ag_term(s,pr[o],&w)))
               {
                    er=do_op(v,w,o);
               }
          } else
               break;
     }
     return(er);
}

int get_op(s,o)
char *s;
int *o;
{
     int er;

     *o=s[pp];

     if(*o<1 || *o>17)
          er=E_SYNTAX;
     else
          er=E_OK;

     return(er);
}
         
int do_op(w,w2,o)
int *w,w2,o;
{
     int er=E_OK;
     switch (o) {
     case 1:
          *w +=w2;
          break;
     case 2:
          *w -=w2;
          break;
     case 3:
          *w *=w2;
          break;
     case 4:
          if (w!=0)
               *w /=w2;
          else
               er =E_DIV;
          break;
     case 5:
          *w >>=w2;
          break;
     case 6:
          *w <<=w2;
          break;
     case 7:
          *w = *w<w2;
          break;
     case 8:
          *w = *w>w2;
          break;
     case 9:
          *w = *w==w2;
          break;
     case 10:
          *w = *w<=w2;
          break;
     case 11:
          *w = *w>=w2;
          break;
     case 12:
          *w = *w!=w2;
          break;
     case 13:
          *w &=w2;
          break;
     case 14:
          *w ^=w2;
          break;
     case 15:
          *w |=w2;
          break;
     case 16:
          *w =*w&&w2;
          break;
     case 17:
          *w =*w||w2;
          break;
     }
     return(er);
}

