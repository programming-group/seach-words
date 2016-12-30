#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define Length 7679//可乎？ 
char Str1[Length][25];//存储字典里的词 
double Score[15];//备选的得分
char Answer[15][25];//备选的答案
void search();//持续进行精确匹配，如无该词进入模糊匹配 
void jaccard(char str2[]);// 将str2与所有词比较， 将分值最高的十个词存入 answer，将score里每个元素赋值为0
void change(char str2[], double vector[], char letter[]);     //定义一个用来将单词转换成26维单位向量的函数  得加到框架最初定义函数部分 
void cosine(char str2[]);//将str2与answer中十个词比较，分值也应在0与1之间，将分值分别加到score
int min_of_three(int n1,int n2,int n3);
int edit(int i,int j,char source[20],char target[20]);
void editdistance(char str2[]);//将str2与answer中十个词比较，比如需要三步变换，就把score的分值换成log3（10*原score）(面前是简单减去)
void print();//将answer分值最高的五个词与其得分按得分高低输出，输出格式需美观
int main(){
	FILE*fp;
	int i=1,j;
	char ch;	
	if((fp=fopen("D:\\别董大\\how to cheat\\英文查词\\新增de词汇.csv","r"))==NULL){//在自己电脑上编辑时改为本地的存储路径 
		printf("object not found");
		exit(0);
	}
	while(!feof(fp)){
		fgets(Str1[i],25,fp);
		strlwr(Str1[i]);
		j=strlen(Str1[i])-1;
		Str1[i][j]='\0';
		if(i==7678){
			Str1[i][3]='m';
			Str1[i][4]='\0';
			break;
		} 
		i++;
	}
	printf("请输入待查单词(输入$结束查询)：");
	search();
	
	return 0;
} 
void search(){
	int i=1;
	char str2[20];
	scanf("%s",&str2);
	if(str2[0]=='$') exit(0);

	else{
		strlwr(str2);
		while(i<=7678){
			if(strcmp(Str1[i],str2)==0)break;
			i++;
		}
		if(i==7679){
			printf("未在词典中找到\n"); 
			jaccard(str2);
			cosine(str2);
			editdistance(str2);
			print();
		}
		else{
			printf("在词典的第%d行，是",i);
			puts(Str1[i]);
		}
		printf("请输入待查单词(输入$结束查询)：");
		search();
	}
	return;
}
//vocabulary has \n or EOF
//注意匹配时不区分大小写，Str1[1]-[1991]为词典里的词，Str1[1992]为anyone，Str1[1993]为bike，全部小写 1882 warm-hearted
//Str1里每个字符串格式为az\0
void jaccard(char str2[]){
	int object[26],source[26],posi[8000],i,j,k,its=0,uni=0;//its=intersection,uni=union
	double aim[8000],temp;
	for(i=1;i<=7678;i++)
		posi[i]=i;
	for(i=0;i<26;i++)
		object[i]=source[i]=0;
	for(i=0;i<strlen(str2);i++){ //26个字母 有则为1 无则为0 
		for(j=0;j<26;j++){
			if(str2[i]=='a'+j) object[j]=1;
		}
	}
	for(i=1;i<=7678;i++){//i 1-1993
		for(j=0;j<strlen(Str1[i]);j++){
			for(k=0;k<26;k++){
				if(Str1[i][j]=='a'+k) source[k]=1;
			}	
		}
		for(k=0;k<26;k++){
			if((object[k]+source[k])==2) its++;
			if((object[k]+source[k])==1) uni++;
		}
		uni=uni+its;
		aim[i]=(double)its/uni;
		its=0;
		uni=0;
		for(k=0;k<26;k++)
			source[k]=0;
	} 
	for(i=1;i<7678;i++){
		for(j=1;j<7679-i;j++){
		if(aim[j]<aim[j+1]){
			temp=aim[j];
			aim[j]=aim[j+1];
			aim[j+1]=temp;
			temp=posi[j];
			posi[j]=posi[j+1];
			posi[j+1]=temp;
		}
	}
	}
	for(i=0;i<15;i++) {
		strcpy(Answer[i], Str1[posi[i+1]]);
		Score[i]=0.1*aim[i+1];
//		printf("%s %lf\n",answer[i],score[i]);  //正常
	}     
	return;
} 
void cosine(char str2[])
{
	char letter[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
	                   'n','o','p','q','r','s','t','u','v','w','x','y','z'};     //储存26个字母
	double vector1[26];               //用来储存待查询单词的26维单位向量 
	double vector2[26];              // 用来储存待匹配单词的26维单位向量 
	change( str2, vector1, letter);   //利用函数将str2转换成一个26维单位向量 
	for(int i = 0; i < 15; i ++)
	{
		change(Answer[i], vector2, letter);   //利用函数将Answer[i]里储存的那个待匹配单词转换成26维单位向量
		for(int j = 0; j < 26; j ++)
		{
			Score[i] = Score[i] + vector1[j] * vector2[j];            //计算每个cosine值并存入score[i]	
		} 
	}
} 


void change(char str2[], double vector[], char letter[])
{
	for(int i = 0; i < 26; i ++)
	{
		vector[i] = 0.0;             //向量数组各元素的初始化 
	}
	int len = strlen(str2);
	for (int i = 0; i < len; i ++)
	{
		for(int j = 0; j < 26; j ++)
		{
			if( i == len-1)                          
			{
				if(str2[i] == letter[j])
				{
					vector[j] = vector[j] + 1 + j;          
				}
			}
			else
			{
				if(str2[i] == letter[j])
				{
					for(int k = 0; k < 26; k ++)
					{
						if(str2[i+1] == letter[k])                            //优化后  abcde……分别对应 1234567…… 
						{
							vector[j] = vector[j] + 1 + j + fabs(k - j);      //每个分量取值 = 相应字母的取值 + 与之紧邻后续字母值之差的绝对值 
						}
					}
				}
			}
		}
	}
	double maxsum = 0.0;
	for(int i = 0; i < 26; i ++)
	{
		maxsum = maxsum + vector[i]*vector[i]; 
	} 
	double lenth = sqrt(maxsum);             //26维向量的模长；
	for(int i = 0; i < 26; i ++)            //将向量单位化 
	{
		vector[i] = 0.6*vector[i]/ lenth ; //
	} 
}

void print(){
	int i,j,len=0;
	double temp;
	char tran[20];
	for(i=0;i<14;i++)
		for(j=0;j<14-i;j++){
			if(Score[j]<Score[j+1]){
				temp=Score[j];
				Score[j]=Score[j+1];
				Score[j+1]=temp;
				strcpy(tran,Answer[j]);
				strcpy(Answer[j],Answer[j+1]);
				strcpy(Answer[j+1],tran);
			}
		}
	for(i=0;i<5;i++){
		if(len<strlen(Answer[i]))len=strlen(Answer[i]);
	}
	for(i=0;i<5;i++){
		printf("%s",Answer[i]);
		for(j=0;j<len-strlen(Answer[i])+1;j++) printf(" ");
		printf("%.5lf\n",Score[i]);
	}
		 

	return;
} 
//cosine的优化 要考虑字母顺序：体现在代码上就是 将单词转换成向量时  每个分量的取值除了包括相应字母的 ASC码值
//再加上它与之紧邻后续字母值之差的绝对值 
//第二个优化是：原来向量分量用的是字母对应ASC码值  现在字母 abcde……分别对应 1234567……  是为了减小相对误差

void editdistance(char str2[]){
	int i,m,n,k;;
	for(i=0;i<15;i++){
		m=strlen(Answer[i]);
		n=strlen(str2);
		k=edit(m-1,n-1,Answer[i],str2);
		Score[i]=Score[i]-(double)k/m;
	}
	return;
}

  

int min_of_three(int n1,int n2,int n3){
    int min;
    min=n1;
    if(min>n2) min=n2;
    if(min>n3) min=n3;
    return min;
}
int edit(int i,int j,char source[20],char target[20]){
	if(i==-1) return j+1;
	if(j==-1) return i+1;
	else if(source[i]==target[j]) edit(i-1,j-1,source,target);
	else{
		int t,p,d,k;
		t=edit(i-1,j-1,source,target);
		p=edit(i,j-1,source,target);
		d=edit(i-1,j,source,target);
		k=1+min_of_three(t,p,d); 
		return k;
	}
}
