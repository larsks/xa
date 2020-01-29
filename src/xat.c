
#include <ctype.h>
#include <stdio.h>
#include "xah.h"

int t_conv(char*,char*,int*,int,int*,int*,int*,int);
int a_term(char*,int*,int*,int);
int b_open(void);
int b_close(void);
int t_p2(char*,int*,int);
int l_get(int,int*);
void l_set(int,int);
int l_def(char*,int*,int*,int*);
int t_keyword(char*,int*,int*);
int l_such(char*,int*,int*,int*);
int tg_asc(char*,char*,int*,int*,int*,int*);
void tg_dez(char*,int*,int*);
void tg_hex(char*,int*,int*);
void tg_oct(char*,int*,int*);
void tg_bin(char*,int*,int*);

char *kt[] ={ 
     "adc","and","asl","bbr","bbs","bcc","bcs","beq",
     "bit","bmi",
     "bne","bpl","bra","brk","bvc","bvs","clc","cld",
     "cli",
     "clv","cmp","cpx","cpy","dec","dex","dey","eor",
     "inc","inx","iny","jmp","jsr","lda","ldx","ldy",
     "lsr","nop","ora","pha","php","phx","phy","pla",
     "plp","plx","ply","rmb","rol",
     "ror","rti","rts","sbc","sec","sed","sei","smb",
     "sta",
     "stx","sty","stz","tax","tay","trb","tsb","tsx",
     "txa","txs","tya",
     ".byt",".word",".asc",".dsb",
     ".(",".)","*="
};

int ktp[]={ 0,3,16,23,26,27,27,27,27,30,32,32,36,36,37,38,46,
               46,51,60,68,68,68,68,68,68,68,75 };

int lp[]= { 0,1,1,1,1,2,2,1,1,1,2,2,2,1,1,1,2,2 };

#define   Lastbef   67 
#define   Anzkey    75

#define   Kbyt      68
#define   Kword     69
#define   Kasc      70
#define   Kdsb      71
#define   Kopen     72   /* .(     */
#define   Kclose    73   /* .)     */
#define   Kpcdef    74   /* *=     */

int ct[68][16] ={
/*     0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15  */
     -1,  0x65,0x75,-1,  0x172,0x61,0x71,0x69,0x6d,0x7d,0x79,-1,  -1,  -1,  -1,  -1,  /*adc*/
     -1,  0x25,0x35,-1,  0x132,0x21,0x31,0x29,0x2d,0x3d,0x39,-1,  -1,  -1,  -1,  -1,  /*and*/
     0x0a,0x06,0x16,-1,  -1,  -1,  -1,  -1,  0x0e,0x1e,-1,  -1,  -1,  -1,  -1,  -1,  /*asl*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x10f,-1,  /*bbr*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x18f,-1,  /*bbs*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x90,-1,  -1,  -1,  -1,  /*bcc*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0xb0,-1,  -1,  -1,  -1,  /*bcs*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0xf0,-1,  -1,  -1,  -1,  /*beq*/
     -1,  0x24,0x134,-1,  -1,  -1,  -1,  0x189,0x2c,0x13c,-1,  -1,  -1,  -1,  -1,  -1,  /*bit*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x30,-1,  -1,  -1,  -1,  /*bmi*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0xd0,-1,  -1,  -1,  -1,  /*bne*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x10,-1,  -1,  -1,  -1,  /*bpl*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x180,-1,  -1,  -1,  -1,  /*bra*/
     0x00,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*brk*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x50,-1,  -1,  -1,  -1,  /*bvc*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x70,-1,  -1,  -1,  -1,  /*bvs*/
     0x18,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*clc*/
     0xd8,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*cld*/
     0x58,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*cli*/
     0xb8,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*clv*/
     -1,  0xc5,0xd5,-1,  0x1d2,0xc1,0xd1,0xc9,0xcd,0xdd,0xd9,-1,  -1,  -1,  -1,  -1,  /*cmp*/
     -1,  0xe4,-1,  -1,  -1,  -1,  -1,  0xe0,0xec,-1,  -1,  -1,  -1,  -1,  -1,  -1,  /*cpx*/
     -1,  0xc4,-1,  -1,  -1,  -1,  -1,  0xc0,0xcc,-1,  -1,  -1,  -1,  -1,  -1,  -1,  /*cpy*/
     0x13a,0xc6,0xd6,-1,  -1,  -1,  -1,  -1,  0xce,0xde,-1,  -1,  -1,  -1,  -1,  -1,  /*dec*/
     0xca,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*dex*/
     0x88,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*dey*/
     -1,  0x45,0x55,-1,  0x152,0x41,0x51,0x49,0x4d,0x5d,0x59,-1,  -1,  -1,  -1,  -1,  /*eor*/
     0x11a,0xe6,0xf6,-1,  -1,  -1,  -1,  -1,  0xee,0xfe,-1,  -1,  -1,  -1,  -1,  -1,  /*inc*/
     0xe8,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*inx*/
     0xc8,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*iny*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x4c,-1,  -1,  -1,  0x6c,0x17c,-1,  -1,  /*jmp*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  0x20,-1,  -1,  -1,  -1,  -1,  -1,  -1,  /*jsr*/
     -1,  0xa5,0xb5,-1,  0x1b2,0xa1,0xb1,0xa9,0xad,0xbd,0xb9,-1,  -1,  -1,  -1,  -1,  /*lda*/
     -1,  0xa6,-1,  0xb6,-1,  -1,  -1,  0xa2,0xae,-1,  0xbe,-1,  -1,  -1,  -1,  -1,  /*ldx*/
     -1,  0xa4,0xb4,-1,  -1,  -1,  -1,  0xa0,0xac,0xbc,-1,  -1,  -1,  -1,  -1,  -1,  /*ldy*/
     0x4a,0x46,0x56,-1,  -1,  -1,  -1,  -1,  0x4e,0x5e,-1,  -1,  -1,  -1,  -1,  -1,  /*lsr*/
     0xea,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*nop*/
     -1,  0x05,0x15,-1,  0x112,0x01,0x11,0x09,0x0d,0x1d,0x19,-1,  -1,  -1,  -1,  -1,  /*ora*/
     0x48,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*pha*/
     0x08,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*php*/
     0x1da,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*phx*/
     0x15a,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*phy*/
     0x68,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*pla*/
     0x28,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*plp*/
     0x1fa,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*plx*/
     0x17a,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*ply*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,0x107,  /*rmb*/
     0x2a,0x26,0x36,-1,  -1,  -1,  -1,  -1,  0x2e,0x3e,-1,  -1,  -1,  -1,  -1,  -1,  /*rol*/
     0x6a,0x66,0x76,-1,  -1,  -1,  -1,  -1,  0x6e,0x7e,-1,  -1,  -1,  -1,  -1,  -1,  /*ror*/
     0x40,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*rti*/
     0x60,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*rts*/
     -1,  0xe5,0xf5,-1,  0x1f2,0xe1,0xf1,0xe9,0xed,0xfd,0xf9,-1,  -1,  -1,  -1,  -1,  /*sbc*/
     0x38,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*sec*/
     0xf8,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*sed*/
     0x78,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*sei*/
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,0x187,  /*smb*/
     -1,  0x85,0x95,-1,  0x192,0x81,0x91,-1,  0x8d,0x9d,0x99,-1,  -1,  -1,  -1,  -1,  /*sta*/
     -1,  0x86,-1,  0x96,-1,  -1,  -1,  -1,  0x8e,-1,  -1,  -1,  -1,  -1,  -1,  -1,  /*stx*/
     -1,  0x84,0x94,-1,  -1,  -1,  -1,  -1,  0x8c,-1,  -1,  -1,  -1,  -1,  -1,  -1,  /*sty*/
     -1,  0x164,0x174,-1,  -1,  -1,  -1,  -1,  0x19c,0x19e,-1,  -1,  -1,  -1,  -1,  -1,  /*stz*/
     0xaa,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*tax*/
     0xa8,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*tay*/
     -1,  0x114,-1,  -1,  -1,  -1,  -1,  -1,  0x11c,-1,  -1,  -1,  -1,  -1,  -1,  -1,  /*trb*/
     -1,  0x104,-1,  -1,  -1,  -1,  -1,  -1,  0x10c,-1,  -1,  -1,  -1,  -1,  -1,  -1,  /*tsb*/
     0xba,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*tsx*/
     0x8a,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*txa*/
     0x9a,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  /*txs*/
     0x98,-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1   /*tya*/
} ;

int cf[] ={ 0,0,0,1,1,0,0,0,0,0,0,0,1,  /* 1= CMOS     */
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,1,1,0,0,1,1,1,
            0,0,0,0,0,0,0,0,1,0,0,0,
            1,0,0,1,1,0,0,0,0 };

#define   Syntax    10

int at[Syntax][3] ={    
     0,   -1,  -1,
     -1,  7,   -1,
     -1,  15,  -1,
     -1,  -1,  14,
     -1,  1,   8,
     -1,  2,   9,
     -1,  3,   10,
     -1,  4,   12,
     -1,  5,   13,
     -1,  6,   -1,
};

#define   AnzAlt    4

int xt[AnzAlt][2] ={ /* Alternativ Adr-Modes  */
     8,   11,       /* abs -> rel  */
     2,   3,        /* z,x -> z,y  */
     5,   6,        /* ,x) -> ),y  */
     9,   10        /* a,x -> a,y  */
};

int le[] ={ 1,2,2,2,2,2,2,2,3,3,3,2,3,3,3,2 };

int opt[] ={ -1,-1,-1,-1,-1,-1,-1,-1,1,2,3,-1,4,5,-1,-1 }; /* abs -> zp */

#define   cval(s)   256*((s)[1]&255)+((s)[0]&255)
#define   wval(i,v) t[i++]=T_VALUE;t[i++]=(v)&255;t[i++]=((v)>>8)&255

int pc;
extern int ncmos,cmosfl;

int t_p1(s,t,ll)
char *s,*t;
int *ll;
{
     static int er,l,n,v,nk,na1,na2,bl,am,sy,i; /*,j,v2 ;*/

     bl=0;

     er=t_conv(s,t,&l,pc,&nk,&na1,&na2,0);

     *ll=l;
/*
     printf("t_conv:");
     for(i=0;i<l;i++)
          printf("%02x,",t[i]);
     printf("\n");
*/
     if(!er)
     {
          n=t[0];
          if(n==Kpcdef)
          {
               if(!(er=a_term(t+1,&pc,&l,pc)))
               {
                    i=1;
                    wval(i,pc);
                    t[i++]=T_END;
                    *ll=5;
                    er=E_OKDEF;
               }
          } else
          if(n==Kopen)
          {
               b_open();
               er=E_NOLINE;
          } else
          if(n==Kclose)
          {
               b_close();
               er=E_NOLINE;
          } else
          if(n==Kdsb)
          {
               if(!(er=a_term(t+1,&bl,&l,pc)))
                    er=E_OKDEF;
          } else         
               er=t_p2(t,ll,1);
          
     } else
     if(er==E_NODEF)
     {
          er=E_OK;
          n=t[0];

          if(n>=0 && n<=Lastbef)
          {
               if(t[1]==T_END)
               {
                    sy=0;
               } else
               if(t[1]=='#')
               {
                    sy=1+nk;
               } else
               if(t[1]=='(')
               {
                    sy=7+nk;
               } else
                    sy=4+nk;

               bl=4;
               
               while(--bl)
               {
                    if((am=at[sy][bl-1])>=0)
                    {
                         if(am>15)
                         {
                              er=E_SYNTAX;
                              break;
                         }
                         if(ct[n][am]>=0)
                              break;

                         for(v=0;v<AnzAlt;v++)
                              if(xt[v][0]==am && ct[n][xt[v][1]]>=0)
                                   break;
                         if(v<AnzAlt) 
                         {
                              am=xt[v][1];
                              break;
                         }
                    }
               }
          
               if(!bl)
                    er=E_SYNTAX;
               else
                    bl=le[am];

          } else
          if(n==Kbyt || n==Kasc)
          {
               bl=nk+1-na1+na2;
          } else
          if(n==Kword)
          {
               bl=2*nk+2;
          } else
          if(n==Kdsb)
          {
               er=a_term(t+1,&bl,&l,pc);
          } else
          if(n==T_OP)
          {
               er=E_OKDEF;
          } else
               er=E_NODEF;
          
          if(!er)
               er=E_OKDEF;
     }
     if(er==E_NOLINE)
     {
          er=E_OK;
          *ll=0;
     }

     pc+=bl;

     return(er);
}

int t_p2(t,ll,fl)
char *t;
int *ll,fl;
{
     static int i,j,k,er,v,n,l,bl,sy,am,c,vv[3],v2;

     er=E_OK;
     bl=0;
     if(*ll<0) /* <0 bei E_OK, >0 bei E_OKDEF     */
     {
          *ll=-*ll;
          bl=*ll;
          er=E_OK;
     } else
     {
          n=t[0];
          if(n==T_OP)
          {
               n=cval(t+1);

               er=a_term(t+4,&v,&l,pc);

               if(!er)
               {
                    if(t[3]=='=')
                    {
                         v2=v;
                    } else
                    if(!(er=l_get(n,&v2)))
                    {
                         if(t[3]=='+')
                         {
                              v2+=v;
                         } else
                         if(t[3]=='-')
                         {
                              v2-=v;
                         } else
                         if(t[3]=='*')
                         {
                              v2*=v;
                         } else
                         if(t[3]=='/')
                         {
                              if(v)
                                   v2/=v;
                              else
                                   er=E_DIV;
                         } else
                         if(t[3]=='|')
                         {
                              v2=v|v2;
                         } else
                         if(t[3]=='&')
                         {
                              v2=v2&v;
                         }
                    }
                    l_set(n,v2);

                    *ll=0;
                    if(!er)
                         er=E_NOLINE;
               }
          } else
          if(n==Kword)
          {
               i=1;
               j=0;
               while(!er && t[i]!=T_END)
               {
                    if(!(er=a_term(t+i,&v,&l,pc)))
                    {    
                         t[j++]=v&255;
                         t[j++]=(v>>8)&255;

                         i+=l;     
                         if(t[i]!=T_END && t[i]!=',')
                              er=E_SYNTAX;
                         else
                         if(t[i]==',')
                              i++;

                    }                    
               }
               *ll=j;
               bl=j;
          } else
          if(n==Kasc || n==Kbyt)
          {
               i=1;
               j=0;
               while(!er && t[i]!=T_END)
               {
                    if(t[i]=='\"')
                    {
                         i++;
                         k=t[i]+i+1;
                         i++;
                         while(i<k)
                              t[j++]=t[i++];
                    } else
                    {
                         if(!(er=a_term(t+i,&v,&l,pc)))
                         {
                              if(v&0xff00)
                                   er=E_OVERFLOW;
                              else
                              {
                                   t[j++]=v;                        
                                   i+=l;     
                              }
                         }
                    }
                    if(t[i]!=T_END && t[i]!=',')
                         er=E_SYNTAX;
                    else
                         if(t[i]==',')
                              i++;
               }
               *ll=j;
               bl=j;
          } else
          if(n==Kpcdef)
          {
               er=a_term(t+1,&pc,&l,pc);
               bl=0;     
               *ll=0;
          } else
          if(n==Kdsb)
          {
               if(!(er=a_term(t+1,&j,&i,pc)))
               {
                    if(t[i+1]!=',')
                         er=E_SYNTAX;
                    else
                    {
                         er=a_term(t+2+i,&v,&l,pc);
                         if(!er && v>>8)
                              er=E_OVERFLOW;
                         t[0]=v&255;
                         if(!er)
                         {
                              *ll=j;
                              bl=j;
                              er=E_DSB;
                         }
                    }
                    if(!er)
                         bl=j;
               }
          } else
          if(n<=Lastbef)
          {
               if((c=t[1])=='#')
               {
                    i=2;
                    sy=1;
                    if(!(er=a_term(t+i,vv,&l,pc)))
                    {
                         i+=l;
                         if(t[i]!=T_END)
                         {
                              if(t[i]!=',')
                                   er=E_SYNTAX;
                              else
                              {
                                   i++;
                                   sy++;
                                   if(!(er=a_term(t+i,vv+1,&l,pc)))
                                   {
                                        i+=l;
                                        if(t[i]!=T_END)
                                        {
                                             if(t[i]!=',')
                                                  er=E_SYNTAX;
                                             else
                                             {
                                                  i++;
                                                  sy++;
                                                  if(!(er=a_term(t+i,vv+2,&l,pc)))
                                                  {
                                                       i+=l;
                                                       if(t[i]!=T_END)
                                                            er=E_SYNTAX;
                                                  }
                                             }
                                        }
                                   }
                              }
                         }
                    }
               } else
               if(c==T_END)
               {
                    sy=0;
               } else
               if(c=='(')
               {
                    sy=7;
                    if(!(er=a_term(t+2,vv,&l,pc)))
                    {

                         if(t[2+l]!=T_END)
                         {
                              if(t[2+l]==',' && tolower(t[3+l])=='x')
                                   sy=8;
                              else
                              if(t[2+l]==')')
                              {
                                   if(t[3+l]==',')
                                   {
                                        if(tolower(t[4+l])=='y')
                                             sy=9;
                                        else
                                             er=E_SYNTAX;
                                   } else
                                   if(t[3+l]!=T_END)
                                        er=E_SYNTAX;
                              } 
                         } else
                              er=E_SYNTAX;
                    }
               } else
               {
                    sy=4;
                    if(!(er=a_term(t+1,vv,&l,pc)))
                    {
                         if(t[1+l]!=T_END)
                         {
                              if(t[1+l]==',')
                              {
                                   if(tolower(t[2+l])=='y')
                                        sy=6;
                                   else
                                        sy=5;
                              } else
                                   er=E_SYNTAX;
                         }
                    }
               }
                
               bl=4;
               
               while(--bl)
               {
                    if((am=at[sy][bl-1])>=0)
                    {
                         if(am>15)
                         {
                              er=E_SYNTAX;
                              break;
                         }
                         if(ct[n][am]>=0)
                              break;

                         for(v=0;v<AnzAlt;v++)
                              if(xt[v][0]==am && ct[n][xt[v][1]]>=0)
                                   break;
                         if(v<AnzAlt) 
                         {
                              am=xt[v][1];
                              break;
                         }
                    }
               }

               if(fl && bl && !er && !(vv[0]&0xff00) && opt[am]>=0)
                    if(ct[n][opt[am]]>=0)
                         am=opt[am];

               if(!bl)
                    er=E_SYNTAX;
               else
               {
                    bl=le[am];
                    *ll=bl;
               }


               if(!er)
               {
                    t[0]=ct[n][am]&0x00ff;
                    if(ct[n][am]&0x0100)
                    {
                         ncmos++;
                         if(!cmosfl)
                              er=E_CMOS;
                    }
                    if(am!=0)
                    {
                         if(am<8)
                         {
                              if(vv[0]&0xff00)
                                   er=E_OVERFLOW;
                              else
                                   t[1]=vv[0];
                         } else
                         if(am<14 && am!=11)
                         {
                              t[1]=vv[0]&255;
                              t[2]=(vv[0]>>8)&255;
                         } else
                         if(am==11)
                         {
                              v=vv[0]-pc-2;
                              if(((v&0xff80)!=0xff80) && (v&0xff80))
                                   er=E_RANGE;
                              else
                                   t[1]=v;
                         } else
                         if(am==14)
                         {
                              if(vv[0]&0xfff8 || vv[1]&0xff00)
                                   er=E_RANGE;
                              else
                              {
                                   t[0]=t[0]|(vv[0]<<4);
                                   t[1]=vv[1];
                                   v=vv[2]-pc-3;
                                   if((v&0xff80) && ((v&0xff80)!=0xff80))
                                        er=E_OVERFLOW;
                                   else
                                        t[2]=v;
                              }
                         } else
                         if(am==15)
                         {
                              if(vv[0]&0xfff8 || vv[1]&0xff00)
                                   er=E_OVERFLOW;
                              else
                              {
                                   t[0]=t[0]|(vv[0]<<4);
                                   t[1]=vv[1];
                              }
                         } else
                              er=E_SYNTAX;
                    }
               }          
                    
          } else
               er=E_SYNTAX;
     }
     pc+=bl;
     return(er);
}

int b_term(s,v,l,pc)
char *s;
int *v,*l,pc;
{
     static char t[MAXLINE];
     int er,i;

     if(!(er=t_conv(s,t,l,pc,&i,&i,&i,1)))
     {
          er=a_term(t,v,&i,pc);
     
     }
     return(er);
}
     
int t_conv(s,t,l,pc,nk,na1,na2,af)  /* Pass1 von s nach t    */
char *s,*t;
int *l,pc,*nk,*na1,*na2,af;
{
     static int p,q,ud,n,v,ll,mk,er,f;
     static int operand,o,fl;

     *nk=0;         /* Anzahl Komma               */
     *na1=0;        /* Anzahl "asc-texte"         */
     *na2=0;        /* und Anzahl Byte in diesen  */
     ll=0;
     er=E_OK;
     p=0;
     q=0;
     ud=0;
     mk=0;          /* 0 = mehrere Kommas erlaubt */
     fl=0;          /* 1 = text einfach weitergeben */

     while(s[p]==' ') p++;

     n=T_END;

     if(!af)
     {
          while(s[p]!='\0' && s[p]!=';')
          {

               if(!(er=t_keyword(s+p,&ll,&n)))
                    break;

               if(er && er!=E_NOKEY)
                    break;

               if(er=l_def(s+p,&ll,&n,&f))
                    break;

               p+=ll;

               while(s[p]==' ') p++;

               if(s[p]=='=')
               {
                    t[q++]=T_OP;
                    t[q++]=n&255;
                    t[q++]=(n>>8)&255;  
                    t[q++]='=';
                    p++;
                    ll=n=0;
                    break;
               } else
               if(f && s[p]!='\0' && s[p+1]=='=')
               {
                    t[q++]=T_OP;
                    t[q++]=n&255;
                    t[q++]=(n>>8)&255;
                    t[q++]=s[p];
                    p+=2;
                    ll=n=0;
                    break;
               } else
               {
                    l_set(n,pc);
                    n=0;
               }

          }
   

          if(n<=Lastbef)
               mk=1;     /* 1= nur 1 Komma erlaubt     */
     }
     if(s[p]=='\0' || s[p]==';')
     {
          er=E_NOLINE;
          ll=0;
     } else
     if(!er)
     {
          p+=ll;
          if(ll)
               t[q++]=n;

          operand=1;
          
          while(s[p]==' ') p++;

          if(s[p]=='#')
          {
               mk=0;
               t[q++]=s[p++];
               while(s[p]==' ')
                    p++;
          }
          while(s[p]!='\0' && s[p]!=';' && !er)
          {
               if(fl)
               {
                    t[q++]=s[p++];
               } else
               {
                 if(operand) 
                 {
                    if(s[p]=='(' || s[p]=='-' || s[p]=='>' || s[p]=='<')
                    {
                         t[q++]=s[p++];
                         operand= -operand+1;
                    } else
                    if(s[p]=='*')
                    {
                         t[q++]=s[p++];
                    } else
                    if(isalpha(s[p]) || s[p]=='_')
                    {
                         er=l_such(s+p,&ll,&n,&v);

                         if(!er)
                         {
                              wval(q,v);
                         }
                         else
                         if(er==E_NODEF)
                         {
                              t[q++]=T_LABEL;
                              t[q++]=n & 255;
                              t[q++]=(n>>8) & 255;
                              ud++;
                              er=E_OK;
                         }
                         p+=ll;
                    }
                    else
                    if(s[p]<='9' && s[p]>='0')
                    {
                         tg_dez(s+p,&ll,&v);
                         p+=ll;
                         wval(q,v);
                    }
                    else
                    switch(s[p]) {
                    case '$':
                         tg_hex(s+p+1,&ll,&v);
                         p+=1+ll;
                         wval(q,v);
                         break;
                    case '%':
                         tg_bin(s+p+1,&ll,&v);
                         p+=1+ll;
                         wval(q,v);
                         break;
                    case '&':
                         tg_oct(s+p+1,&ll,&v);
                         p+=1+ll;
                         wval(q,v);
                         break;
                    case '\"':
                         er=tg_asc(s+p,t+q,&q,&p,na1,na2);
                         break;
                    case ',':
                         if(mk)
                              while(s[p]!='\0' && s[p]!=';')
                              {
                                   while(s[p]==' ') p++;
                                   *nk+=(s[p]==',');
                                   t[q++]=s[p++];
                              }
                         else
                         {
                              *nk+=1;
                              t[q++]=s[p++];
                         }
                         break;
                    default :
                         er=E_SYNTAX;
                         break;
                    }
                    operand= -operand+1;

                 } else    /* operator    */
                 { 
                    o=0;
                    if(s[p]==')')
                    {
                         t[q++]=s[p++];
                         operand =-operand+1;
                    } else
                    if(s[p]==',')
                    {
                         t[q++]=s[p++];
                         if(mk)
                              fl++;
                         *nk+=1;
                    } else
                    switch(s[p]) {
                    case '+':
                         o=1;
                         break;
                    case '-':
                         o=2;
                         break;
                    case '*':
                         o=3;
                         break;
                    case '/':
                         o=4;
                         break;
                    case '<':
                         switch (s[p+1]) {
                         case '<':
                              o=6;
                              break;
                         case '>':
                              o=12;
                              break;
                         case '=':
                              o=10;
                              break;
                         default :
                              o=7;
                              break;
                         }
                         break;
                    case '>':
                         switch (s[p+1]) {
                         case '>':
                              o=5;
                              break;
                         case '<':
                              o=12;
                              break;
                         case '=':
                              o=11;
                              break;
                         default:
                              o=8;
                              break;
                         }
                         break;
                    case '=':
                         switch (s[p+1]) {
                         case '<':
                              o=10;
                              break;
                         case '>':
                              o=11;
                              break;
                         default:
                              o=9;
                              break;
                         }
                         break;
                    case '&':
                         if (s[p+1]=='&')
                              o=16;
                         else
                              o=13;
                         break;
                    case '|':
                         if (s[p+1]=='|')
                              o=17;
                         else   
                              o=15;
                         break;
                    case '^':
                         o=14;
                         break;
                    default:
                         er=E_SYNTAX;
                         break;
                    }
                    if(o)
                    {
                         t[q++]=o;
                         p+=lp[o];
                    }
                    operand= -operand+1;
                 }

                 while(s[p]==' ') p++;
               }
          }
     }
     if(!er)
     {
          t[q++]=T_END;
          if(ud)
               er=E_NODEF;
     }
     *l=q;

     return(er);
}

t_keyword(s,l,n)
char *s;
int *l,*n;
{
     int i=0,j,hash;

     if(!isalpha(s[0]) && s[0]!='.' && s[0]!='*' )
          return(E_NOKEY);

     if(isalpha(s[0]))
          hash=tolower(s[0])-'a';
     else
          hash=26;
     
     i=ktp[hash];
     hash=ktp[hash+1];
     
     while(i<hash)
     {
          j=0;
          while(kt[i][j]!='\0' && kt[i][j]==tolower(s[j]))
               j++;

          if((kt[i][j]=='\0') && ((i==Kpcdef) || ((s[j]!='_') && !isalnum(s[j]))))
               break;
          i++;
     }    
     *l=j;
     *n=i;

     return( i==hash ? E_NOKEY : E_OK );
}

void tg_dez(s,l,v)
char *s;
int *l,*v;
{
     int i=0,val=0;

     while(isdigit(s[i]))
          val=val*10+(s[i++]-'0');

     *l=i;
     *v=val;
}

void tg_bin(s,l,v)
char *s;
int *l,*v;
{
     int i=0,val=0;

     while(s[i]=='0' || s[i]=='1')
          val=val*2+(s[i++]-'0');

     *l=i;
     *v=val;
}

void tg_oct(s,l,v)
char *s;
int *l,*v;
{
     int i=0,val=0;

     while(s[i]<'8' && s[i]>='0')
          val=val*8+(s[i++]-'0');

     *l=i;
     *v=val;
}

void tg_hex(s,l,v)
char *s;
int *l,*v;
{
     int i=0,val=0;

     while((s[i]>='0' && s[i]<='9') || (tolower(s[i])<='f' && tolower(s[i])>='a'))
     {
          val=val*16+(s[i]<='9' ? s[i]-'0' : tolower(s[i])-'a'+10);
          i++;
     }
     *l=i;
     *v=val;
}

int tg_asc(s,t,q,p,na1,na2)
char *s,*t;
int *p,*q,*na1,*na2;
{
     int er=E_OK,i=0,j=0;
     
     t[j++]=s[i++];      /* " */
     j++;
     while(s[i]!='\0' && s[i]!='\"')
     {
          if(s[i]!='^')
               t[j++]=s[i];
          else 
          switch(s[i+1]) {
          case '\0':
          case '\"':
               er=E_SYNTAX;
               break;
          case '^':
               t[j++]='^';
               i++;
               break;
          default:
               t[j++]=s[i+1]&0x1f;
               i++;
               break;
          }
          i++;
     }
     if(j==3)
     {
          t[0]=T_VALUE;
          t[1]=t[2];
          t[2]=0;
     } else
     {
          t[1]=j-2;
          *na1 +=1;
          *na2 +=j-2;
     }
     if(s[i]=='\"')
          i++;
     *q +=j;
     *p +=i;
     return(er);
}

