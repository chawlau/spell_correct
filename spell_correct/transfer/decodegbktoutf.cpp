#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>
#include<unistd.h>
#include<cstdlib>
#include<cstdio>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/dir.h>
#include<dirent.h>
#include<fcntl.h>
int gbk2utf8(char *utfStr,const char *srcStr)
{

    if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))
    {
        printf("Bad Parameter\n");
        return -1;
    }
    int unicodeLen=mbstowcs(NULL,srcStr,0);
    if(unicodeLen<=0)
    {
        printf("Can not Transfer!\n");
        return -1;
    }
    wchar_t *unicodeStr=(wchar_t*)calloc(sizeof(wchar_t),unicodeLen+1);
    mbstowcs(unicodeStr,srcStr,strlen(srcStr));//将gbk转成unicode
    if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))
    {
        printf("Bad Parameter\n");
        return -1;
    }
    int utfLen=wcstombs(NULL,unicodeStr,0);
    if(utfLen<=0)
    {
        printf("Can not Transfer!!!\n");
        return -1;
    }
    wcstombs(utfStr,unicodeStr,utfLen);
    utfStr[utfLen]=0;
    free(unicodeStr);
    return utfLen;
}
void transfer(const char* src_path,const char* des_path)
{
    std::ifstream fin(src_path);
    std::ofstream fout;
    fout.open(des_path,std::ofstream::out | std::ofstream::app); 
    std::string line;
    while(getline(fin,line))
    {
        std::istringstream sin(line);
        char* utfw=new char[1000000]();
        std::string srcw;
        while(sin>>srcw)
        {
            gbk2utf8(utfw,srcw.c_str());
            char buf[1024];
            sprintf(buf,"%s%s",utfw," ");
            fout<<buf;
        }
        fout<<std::endl;
        free(utfw);
    }
    std::cout<<src_path<<"is transfered over!"<<std::endl;
    fin.close();
    fout.close();
}
void transfer_dir(char* dir_path,char* des_path)
{
    struct dirent* p_entry;
    struct stat my_stat;
    DIR* dir;
    memset(&my_stat,0,sizeof(my_stat));
    if(-1==lstat(dir_path,&my_stat))   //分析该目录状态
    {
        perror("lstat\n");
    }
    if(S_ISREG(my_stat.st_mode))//如果是普通文件，直接解析
    {
        transfer(dir_path,des_path);
    }
    else if(S_ISDIR(my_stat.st_mode))//如果是目录打开
    {
        dir=opendir(dir_path);
        if(dir==NULL)
        {
            perror("dir open failed\n");
        }
        while(memset(&my_stat,0,sizeof(my_stat)),(p_entry=readdir(dir))!=NULL)
        {
            if(strcmp(".",p_entry->d_name)==0||strcmp("..",p_entry->d_name)==0)
                continue;
            char buf[1024];
            sprintf(buf,"%s/%s",dir_path,p_entry->d_name);//将目录绝对路径赋值buf
            transfer_dir(buf,des_path);
        }
    }
}
int main(int argc,char* argv[]) 
{
    transfer_dir(argv[1],argv[2]);   
}
