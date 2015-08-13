#include "opt.h"

int extract(const char *filename, int do_extract, int flags,const char* outputpath) {
  char* str=NULL;
  int len=strlen(filename),ret=-1;
  char* type="gzip"; /*for gz*/
  if(len>4) {
    int i,c;
    for(c=0,i=len;filename[i]!='.' && c<5;--i,++c) {
      if(filename[i]=='b'||filename[i]=='B') {
        type="bzip2";
        break;
      }else if(filename[i]=='x'||filename[i]=='X'){
        type="xz";
        break;
      }else if(filename[i]=='7') {
        type="7za";
        break;
      }
    }
  }
  if(verbose>0)
    fprintf(stderr,"extracttype=%s\n",type);
#ifndef HAVE_WINDOWS_H
  if(strcmp(type,"gzip")==0 || strcmp(type,"bzip2")==0 || strcmp(type,"xz")==0) {
    str=cat(type," -dc ",filename," | tar -",do_extract?"x":"t",
            flags?"p":"","f - -C ",outputpath,NULL);
  }else if(strcmp(type,"7za")==0) {
    ensure_directories_exist((char*)outputpath);
    str=cat("7za ",do_extract?"x":"t"," -o",outputpath," ",filename,NULL);
  }
#else
  char* _uname_m=uname_m();
  char* _uname=uname();
  char* _homedir=configdir();
  char* exe=s_escape_string(cat(_homedir,"impls",SLASH,_uname_m,SLASH,_uname,SLASH,"7za",SLASH,"9.20",SLASH,"7za.exe",NULL));
  char *outputpath2=q(outputpath);
  char *filename2=q(filename);
  substitute_char('\\','/',outputpath2);
  outputpath2=s_escape_string(outputpath2);
  filename2=s_escape_string(filename2);
  ensure_directories_exist(outputpath2);
  if(strcmp(type,"gzip")==0 || strcmp(type,"bzip2")==0 || strcmp(type,"xz")==0) {
    str=cat(exe," ",do_extract?"x ":"l ",filename2," -so |",exe," x -ttar -si -y -o",outputpath2,NULL);
  }else if(strcmp(type,"7za")==0) {
    ensure_directories_exist(outputpath2);
    str=cat(exe," ",do_extract?"x":"t"," -y -o",outputpath2," ",filename2,NULL);
  }
  s(outputpath2),s(filename2),s(_homedir),s(_uname),s(_uname_m);
#endif
  if(verbose>0)
    fprintf(stderr,"extractcmd=%s\n",str);
  if(str) {
    ret=System(str);
    s(str);
  }
  return ret;
}

int cmd_tar(int argc,char **argv,struct sub_command* cmd) {
  const char *filename = NULL;
  const char *outputpath = NULL;
  int flags=0, mode, opt;

  mode = 'x';
  /* Among other sins, getopt(3) pulls in printf(3). */
  while (*++argv != NULL && **argv == '-') {
    const char *p = *argv + 1;
    while ((opt = *p++) != '\0') {
      switch (opt) {
      case 'f':
        if (*p != '\0')
          filename = p;
        else
          filename = *++argv;
        p += strlen(p);
        break;
      case 'C':
        if (*p != '\0')
          outputpath = p;
        else
          outputpath = *++argv;
        p += strlen(p);
        break;
      case 'p':
        flags = 1;
        break;
      case 't':
        mode = opt;
        break;
      case 'v':
        verbose++;
        break;
      case 'x':
        mode = opt;
        break;
      }
    }
  }
  switch (mode) {
  case 't':
    extract(filename, 0, flags,outputpath);
    break;
  case 'x':
    extract(filename, 1, flags,outputpath);
    break;
  }
  return (0);
}
