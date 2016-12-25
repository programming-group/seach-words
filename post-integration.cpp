#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
char str1[2000][20];//存储字典里的词 
double score[15];//备选的得分
char answer[15][20];//备选的答案
void search();//持续进行精确匹配，如无该词进入模糊匹配 
void jaccard(char str2[]);// 将str2与所有词比较， 将分值最高的十个词存入 answer，将score里每个元素赋值为0
void change(char str2[], double vector[], char letter[]);     //定义一个用来将单词转换成26维单位向量的函数  得加到框架最初定义函数部分 
void cosine(char str2[]);//将str2与answer中十个词比较，分值也应在0与1之间，将分值分别加到score
int min_of_three(int n1,int n2,int n3);
int edit_distance(char *a, char *b) ;
void editdistance(char str2[]);//将str2与answer中十个词比较，比如需要三步变换，就把score的分值换成log3（10*原score）
void print();//将answer分值最高的五个词与其得分按得分高低输出，输出格式需美观
int main(){
	FILE*fp;
	int i=1,j;
	char ch;	
	if((fp=fopen("D:\\别董大\\how to cheat\\英文查词\\vocabulary.csv","r"))==NULL){//在自己电脑上编辑时改为本地的存储路径 
		printf("object not found");
		exit(0);
	}
	while(!feof(fp)){
		fgets(str1[i],20,fp);
		strlwr(str1[i]);
		j=strlen(str1[i])-1;
		str1[i][j]='\0';
		if(i==1991){
			str1[i][2]='o';
			str1[i][3]='\0';
			break;
		} 
		i++;
	}
	strcpy(str1[66],"anybody");
	strcpy(str1[157],"bicykle");
	strcpy(str1[1992],"anyone");
	strcpy(str1[1993],"bike");
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
		while(i<=1993){
			if(strcmp(str1[i],str2)==0)break;
			i++;
		}
		if(i==1994){
			printf("未在词典中找到\n"); 
			jaccard(str2);
			cosine(str2);
	
			editdistance(str2);
			print();
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
//注意匹配时不区分大小写，str1[1]-[1991]为词典里的词，str1[1992]为anyone，str1[1993]为bike，全部小写 1882 warm-hearted
//str1里每个字符串格式为az\0
void jaccard(char str2[]){
	int object[26],source[26],posi[2000],i,j,k,its=0,uni=0;//its=intersection,uni=union
	double aim[2000],temp;
	for(i=1;i<=1993;i++)
		posi[i]=i;
	for(i=0;i<26;i++)
		object[i]=source[i]=0;
	for(i=0;i<strlen(str2);i++){ //26个字母 有则为1 无则为0 
		for(j=0;j<26;j++){
			if(str2[i]=='a'+j) object[j]=1;
		}
	}
	for(i=1;i<=1993;i++){//i 1-1993
		for(j=0;j<strlen(str1[i]);j++){
			for(k=0;k<26;k++){
				if(str1[i][j]=='a'+k) source[k]=1;
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
	for(i=1;i<1993;i++){
		for(j=1;j<1994-i;j++){
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
		strcpy(answer[i], str1[posi[i+1]]);
		score[i]=0.1*aim[i+1];
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
		change( answer[i], vector2, letter);   //利用函数将answer[i]里储存的那个待匹配单词转换成26维单位向量
		for(int j = 0; j < 26; j ++)
		{
			score[i] = score[i] + vector1[j] * vector2[j];            //计算每个cosine值并存入score[i]	
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
		vector[i] = 0.9*vector[i] / lenth; 
	} 
}

void print(){
	int i,j,len=0;
	double temp;
	char tran[20];
	for(i=0;i<14;i++)
		for(j=0;j<14-i;j++){
			if(score[j]<score[j+1]){
				temp=score[j];
				score[j]=score[j+1];
				score[j+1]=temp;
				strcpy(tran,answer[j]);
				strcpy(answer[j],answer[j+1]);
				strcpy(answer[j+1],tran);
			}
		}
	for(i=0;i<5;i++){
		if(len<strlen(answer[i]))len=strlen(answer[i]);
	}
	for(i=0;i<5;i++){
		printf("%s",answer[i]);
		for(j=0;j<len-strlen(answer[i])+1;j++) printf(" ");
		printf("%.5lf\n",score[i]);
	}
		 

	return;
} 
//cosine的优化 要考虑字母顺序：体现在代码上就是 将单词转换成向量时  每个分量的取值除了包括相应字母的 ASC码值
//再加上它与之紧邻后续字母值之差的绝对值 
//第二个优化是：原来向量分量用的是字母对应ASC码值  现在字母 abcde……分别对应 1234567……  是为了减小相对误差

void editdistance(char str2[]){

    int i,j,k=0;   
    char t[30];
    double n[10];
    for(i=0;i<15;i++){
        n[i]=(double)edit_distance(str2,answer[i])/(double)strlen(str2);    
                            //挨个儿计算相似性并将相似性数值存在一个浮点数组里面 
    }
    struct similarity{
        char str[30];
        double similar;
    };         //定义结构体 
    struct similarity answers[15];
    for(i=0;i<15;i++){
        strcpy(answers[i].str,answer[i]);
        answers[i].similar=n[i];
    }
    
    for(i=0;i<14;i++){
        for(j=0;j<14-i;j++){
            struct similarity tmp;
            if(answers[j].similar>answers[j+1].similar){
                tmp=answers[j];
                
                answers[j]=answers[j+1];
                
                answers[j+1]=tmp;
                
            }
        } 
    }                   //对结构体进行冒泡排序 
    for(i=0;i<15;i++){
    	strcpy(answer[i],answers[i].str);
    	score[i]=score[i]-answers[i].similar;
    //存入前五个相似度最高的单词 
    }
    return;
} 
int edit_distance(char *a, char *b)    //计算所需步数 
{
    int min_of_three(int n1,int n2,int n3);
    int lena = strlen(a);
    int lenb = strlen(b);
    int d[lena+1][lenb+1];
    int i, j;

    for (i = 0; i <= lena; i++) {
        d[i][0] = i;
    }
    for (j = 0; j <= lenb; j++) {
        d[0][j] = j;
    }

    for (i = 1; i <= lena; i++) {
        for (j = 1; j <= lenb; j++) {
            // 算法中 a, b 字符串下标从 1 开始，c 语言从 0 开始，所以 -1
            if (a[i-1] == b[j-1]) {
                d[i][j] = d[i-1][j-1];
            } else {
                d[i][j] = min_of_three(d[i-1][j]+1, d[i][j-1]+1, d[i-1][j-1]+1);
            }
        }
    }

    return d[lena][lenb];
}

int min_of_three(int n1,int n2,int n3){
    int min;
    min=n1;
    if(min>n2) min=n2;
    if(min>n3) min=n3;
    return min;
}

