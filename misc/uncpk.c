
#include <stdio.h>
#include <string.h>

#define max(a,b)	(((a)>(b))?(a):(b))
#define min(a,b)	(((a)<(b))?(a):(b))

FILE *fp;
char name[100];
char *s;

void usage(int er) {
	printf("uncpk: handle c64 archives\n"
	       "  uncpk [x|l][v] archive-name\n"
	       "  uncpk [a|c][v] archive-name file1 [file2 [file3 ...]]\n");
	exit(er);
}

int list=0,verbose=0,add=0,create=0;

int main(int argc, char *argv[]){
	int i,c,c2,fileok;
	size_t n,n2;
	FILE *fp,*fpo=NULL;

	if(argc<=1) {
	  usage(1);
	}	

	if(strchr(argv[1],(int)'l')) {
	  list=1;
	}
	if(strchr(argv[1],(int)'v')) {
	  verbose=1;
	}
	if(strchr(argv[1],(int)'a')) {
	  add=1;
	}
	if(strchr(argv[1],(int)'c')) {
	  create=1;
	}

	if(add||create) {
	 if (argc<=3) {
	   usage(1);
	 }
	 if(add) {
	   fpo=fopen(argv[2],"ab");
	 } else 
	 if(create) {
	   fpo=fopen(argv[2],"wb");
	 }
	 if(fpo) {
	   if(!add) fputc(1,fpo);		/* Version Byte */
	   for(i=3;i<argc;i++) {
	     if(verbose) printf("%s\n",argv[i]);
	     fp=fopen(argv[i],"rb");
	     if(fp) {
	       while(s=strchr(argv[i],':')) *s='/';
	       fprintf(fpo,"%s",argv[i]);
	       fputc(0,fpo);
	       c=fgetc(fp);
	       while(c!=EOF) {
		 n=1;
	         while((c2=fgetc(fp))==c) {
		   n++;
		 }
		 while(n) {
		   if(n>=4 || c==0xf7) {
		     n2=min(255,n);
		     fprintf(fpo,"\xf7%c%c",(char)n2,(char)c);
		     n-=n2;
		   } else {
		     fputc(c,fpo);
		     n--;
		   }
		 }
		 c=c2; 
	       }
	       fclose(fp); 
	       fputc(0xf7,fpo); fputc(0,fpo);
	     } else {
	       fprintf(stderr,"Couldn't open file '%s' for reading!",argv[i]);
	     }
	   }
	   fclose(fpo);
	 } else {
	   fprintf(stderr,"Couldn't open file '%s' for writing!",argv[1]);
	 } 
	} else {
	 if (argc!=3) {
 	   usage(1);
	 }
	 fp=fopen(argv[2],"rb");
	 if(fp){
	  if(fgetc(fp)==1){
	    do{
	      /* read name */
	      i=0;
	      while(c=fgetc(fp)){
	        if(c==EOF) break;
	        name[i++]=c;
	      }
	      name[i++]='\0';
	      if(!c){	/* end of archive ? */
	        while(s=strchr(name,'/')) *s=':';

	        if(verbose+list) printf("%s\n",name);

	        if(!list) { 
		  fpo=fopen(name,"wb");
	          if(!fpo) {
	 	    fprintf(stderr,"Couldn't open output file %s !\n",name);
	          }
	        }
		fileok=0;
	        while((c=fgetc(fp))!=EOF){
		  /* test if 'compressed' */
	          if(c==0xf7){
	            n=fgetc(fp);
	            if(!n) {
		      fileok=1;
	              break;
	            }
	            c=fgetc(fp);
	            if(fpo) {		/* extract */
	              if(n!=EOF && c!=EOF) {
	                while(n--) {
	                  fputc(c,fpo);
	                }
	              }
		    }
	          } else {
	            if(fpo) {
	              fputc(c,fpo);
	            }
	          }
	        }
	        if(fpo) {
	          fclose(fpo);
	          fpo=NULL;
	        }
		if(!fileok) {
		  fprintf(stderr,"Unexpected end of file!\n");
		}
	      } 
   	    } while(c!=EOF);
	  } else
	    fprintf(stderr,"Wrong Version!\n");
	  fclose(fp);
	 } else {
	  fprintf(stderr,"File %s not found!\n",argv[1]);
	 } 
	}
	return(0);
}
