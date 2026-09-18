#include<regex.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(void)
{
	int fd = open("url.html",O_RDWR);
	int size = lseek(fd,0,SEEK_END);
	char *mmap_ptr = NULL;

	mmap_ptr = mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);

	close(fd);

	regex_t reg;
	const char * regStr = "<a[^>]\\+\\?href=\"\\([^\"]\\+\\?\\)\"[^>]\\+\\?>\\([^<]\\+\\?\\)</a>";
	regcomp(&reg,regStr,0);

	int reg_num = 3;
	regmatch_t match[reg_num];
	
	char title[1024];
	char url[1024];
	while(regexec(&reg,mmap_ptr,reg_num,match,0)==0)
	{
		bzero(title,sizeof(title));
		bzero(url,sizeof(url));
		snprintf(title,match[2].rm_eo-match[2].rm_so + 1,"%s",mmap_ptr+match[2].rm_so);
		snprintf(url,match[1].rm_eo-match[1].rm_so + 1,"%s",mmap_ptr+match[1].rm_so);
		printf("%s:%s\n",title,url);

		mmap_ptr+=match[0].rm_eo;
	}

	regfree(&reg);

	return 0;
}
