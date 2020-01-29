 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xah.h"         /*   structs        */
/*#include <tos.h>*/
 
#define	strlen(a)		((int)(strlen(a)))

/*
#define	MAXLINE		200
#define	BUFSIZE		4096

#define	getenv(a)		get_env(a)

FILE *xfopen(const char *filename,const char *mode);
void ini_env(char **envp);
void put_env(char *envar,char *enval);
char *get_env(char*);
*/

FILE *xfopen(const char *fn,const char *mode)
{
	FILE *file;
	char c,*cp,n[MAXLINE],path[MAXLINE];
	char xname[MAXLINE], n2[MAXLINE];
	int i,l=strlen(fn);
	for(i=0;i<l+1;i++) {
	  xname[i]=((fn[i]=='\\')?DIRCHAR:fn[i]);
	}

	if(mode[0]=='r')
	{
		if((file=fopen(fn,mode))==NULL 
		   && (file=fopen(xname, mode))==NULL
	  	   && (cp=getenv("XAINPUT"))!=NULL)
		{
			strcpy(path,cp);
			cp=strtok(path,",");
			while(cp && !file)
			{
				if(cp[0])
				{
					strcpy(n,cp);
					c=n[strlen(n)-1];
					if(c!=DIRCHAR&&c!=':')
						strcat(n,DIRCSTRING);
					strcpy(n2,n);
					strcat(n2,xname);
					strcat(n,fn);
/*		printf("name=%s\nmode=%s\n",n,mode);*/
					file=fopen(n,mode);
					if(!file) file=fopen(n2,mode);
				}
				cp=strtok(NULL,",");
			}
		}
	} else
	{
		if((cp=getenv("XAOUTPUT"))!=NULL)
		{
			strcpy(n,cp);
			if(n[0])
			{
				c=n[strlen(n)-1];
				if(c!=DIRCHAR&&c!=':')
					strcat(n,DIRCSTRING);
			}
			cp=strrchr(fn,DIRCHAR);
			if(!cp)
			{
				cp=strrchr(fn,':');
				if(!cp)
					cp=(char*)fn;
				else
					cp++;
			} else
				cp++;
			strcat(n,cp);
			file=fopen(n,mode);
		} else
			file=fopen(fn,mode);
	}
	if(file)
		setvbuf(file,NULL,_IOFBF,BUFSIZE);

	return(file);		
}

char *m_base;
char *m_act;
char *m_end;

int m_init() 
{
     int er=E_NOMEM;

     m_base=m_end=m_act=0L;
/*
     printf("MEMLEN=%ld\n",MEMLEN);
     getchar();
*/    
/*
     if ((m_base=(char*)malloc(MEMLEN))!=NULL)
     {
          m_end =m_base+MEMLEN;
          m_act =(char*)(((long)m_base+3)&0xfffffffcl);
          er=E_OK;
     }
     else m_base=NULL;
*/
    er=E_OK;

     return(er);
}

void m_exit(void)
{
/*
	free(m_base);
*/
}

m_alloc(n,adr)
long n;
char **adr;
{
     int er=E_NOMEM;

     if((*adr=calloc(n,1))) {
	er=E_OK;
     }
/*
     if(m_act+n<m_end)
     {
          *adr=m_act;
          m_act=m_act+n;
          er=E_OK;
     }
*/
/*
     printf("m_alloc n=%ld adr=%lx\n",n,*adr);
     getchar();
*/     
     return(er);
}

