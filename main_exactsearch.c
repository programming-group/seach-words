#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void search();
char str1[2000][20];//存储字典里的词 
int score[10];
char answer[10];
int main(){
	FILE*fp;
	int i=1;
	char ch;	
	if((fp=fopen("D:\\别董大\\how to cheat\\英文查词\\vocabulary.csv","r"))==NULL){//在自己电脑上编辑时改为本地的存储路径 
		printf("object not found");
		exit(0);
	}
	while(!feof(fp)){
		fgets(str1[i],20,fp);
		strlwr(str1[i]);
		if(i==1991){
			str1[i][3]='\n';
			str1[i][4]='\0';
			break;
		} 
		i++;
	}
	strcpy(str1[66],"anybody\n");
	strcpy(str1[157],"bicykle\n");
	strcpy(str1[1992],"anyone\n");
	strcpy(str1[1993],"bike\n");//为什么不能str1[157]="bicykle\n\0";
	printf("请输入待查单词(输入$结束查询)：");
	search();
	return 0;
} 
void search(){
	int i=0;
	char str2[20];
	scanf("%s",&str2);
	if(str2[0]=='$') exit(0);

	else{
		while(str2[i]!='\0'){
			if(str2[i]>='A' && str2[i]<='Z')
            	str2[i]+=32;
    
			i++;
		}		
		str2[i]='\n';
		str2[i+1]='\0';
		i=1;
		while(i<=1993){
			if(strcmp(str1[i],str2)==0)break;
			i++;
		}
		if(i==1994){
			printf("未在词典中找到\n"); 
		
		}
		else{
			if(i==1992)i=66;
			else if(i==1993)i=157;
			printf("在词典的第%d行，是",i);
			puts(str1[i]);
		}
		printf("请输入待查单词(输入$结束查询)：");
		search();
	}
	return;
}
//vocabulary has \n or EOF
//不区别大小写，显示小写。  每个字符串为存储为az\n\0
