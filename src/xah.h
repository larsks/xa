
#define   ANZLAB    5000       /* mal 14 -> Byte   */
#define   LABMEM    40000L
#define   MAXLAB    32
#define   MAXBLK    16
#define   MAXFILE   7
#define   MAXLINE   200
#define   MAXPP     40000L
#define   ANZDEF    2340      /* mal 14 -> Byte , ANZDEF*14<32768       */
#define   TMPMEM    200000L   /* Zwischenspeicher von Pass1 nach Pass 2 */

typedef struct {
     int blk;
     int val;
     int len;
     int fl;   /* 0 = undef   1 = def             */
     int nextindex;
     char *n;
} Labtab;

typedef struct {
     char *search;
     int  s_len;
     char *replace;
     int  p_anz;
     int  nextindex;
} List;


#define   MEMLEN    (4+TMPMEM+MAXPP+LABMEM+(long)(sizeof(Labtab)*ANZLAB)+(long)(sizeof(List)*ANZDEF))

#define   DIRCHAR    '/'
#define	  DIRCSTRING "/" 
/* for Atari:
#define	  DIRCHAR    '\\'
#define	  DIRCSTRING "\\"
*/

#define	BUFSIZE	4096		/* File-Puffegr”že (wg Festplatte)	*/
	
#define   E_OK      0         /* Fehlernummern                   */
#define   E_SYNTAX  -1        /* Syntax Fehler                   */
#define   E_LABDEF  -2        /* Label definiert                 */
#define   E_NODEF   -3        /* Label nicht definiert           */
#define   E_LABFULL -4        /* Labeltabelle voll               */
#define   E_LABEXP  -5        /* Label erwartet                  */
#define   E_NOMEM   -6        /* kein Speicher mehr              */
#define   E_ILLCODE -7        /* Illegaler Opcode                */
#define   E_ADRESS  -8        /* Illegale Adressierung           */
#define   E_RANGE   -9        /* Branch out of range             */
#define   E_OVERFLOW -10      /* šberlauf                        */
#define   E_DIV     -11       /* Division durch Null             */
#define   E_PSOEXP  -12       /* Pseudo-Opcode erwartet          */
#define   E_BLKOVR  -13       /* Block-Stack bergelaufen        */
#define   E_FNF     -14       /* File not found (pp)             */
#define   E_EOF     -15       /* End of File                     */
#define   E_BLOCK   -16       /* Block inkonsistent              */
#define   E_NOBLK   -17
#define   E_NOKEY   -18
#define   E_NOLINE  -19
#define   E_OKDEF   -20
#define   E_DSB     -21
#define   E_NEWLINE -22
#define   E_NEWFILE -23
#define   E_CMOS    -24
#define   E_ANZPAR  -25

#define   T_VALUE   -1
#define   T_LABEL   -2 
#define   T_OP      -3
#define   T_END     -4
#define   T_LINE    -5
#define   T_FILE    -6

#define   P_START   0         /* Priorit„ten fr Arithmetik      */
#define   P_LOR     1         /* Von zwei Operationen wird immer */
#define   P_LAND    2         /* die mit der h”heren Priorit„t   */
#define   P_OR      3         /* zuerst ausgefhrt               */
#define   P_XOR     4
#define   P_AND     5
#define   P_EQU     6
#define   P_CMP     7
#define   P_SHIFT   8
#define   P_ADD     9
#define   P_MULT    10
#define   P_INV     11


