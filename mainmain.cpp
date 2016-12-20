#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char str1[2000][20];//存储字典里的词 
double score[10];//备选的得分
char answer[10];//备选的答案
void search();//持续进行精确匹配，如无该词进入模糊匹配 
void jaccard(char str2);// 将str2与所有词比较， 将分值最高的十个词存入 answer，将score里每个元素赋值为0
void cosine(char str2);//将str2与answer中十个词比较，将分值分别存进score，注意每个分值在0与1之间
void cosine2(char str2);//将str2与answer中十个词比较，分值也应在0与1之间，将分值分别加到score
void editdistance(char str2);//将str2与answer中十个词比较，比如需要三步变换，就把score的分值换成log3（10*原score）
void print();//将answer分值最高的五个词与其得分按得分高低输出，输出格式需美观
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
	strcpy(str1[1993],"bike\n");
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
			jaccard(str2);
			cosine(str2);
			cosine2(str2);
			editdistance(str2);
			print;
		}
		else{
			printf("在词典的第%d行，是",i);
			puts(str1[i]);
		}
		printf("请输入待查单词(输入$结束查询)：");
		search();
	}
	return;
}
//vocabulary has \n or EOF
//注意匹配时不区分大小写，str1[1]-[1991]为词典里的词，str1[1992]为anyone，str1[1993]为bike，全部小写
//str1里每个字符串格式为az\n\0

