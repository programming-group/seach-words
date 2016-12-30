#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define Length 7679//�ɺ��� 
char Str1[Length][25];//�洢�ֵ���Ĵ� 
double Score[15];//��ѡ�ĵ÷�
char Answer[15][25];//��ѡ�Ĵ�
void search();//�������о�ȷƥ�䣬���޸ôʽ���ģ��ƥ�� 
void jaccard(char str2[]);// ��str2�����дʱȽϣ� ����ֵ��ߵ�ʮ���ʴ��� answer����score��ÿ��Ԫ�ظ�ֵΪ0
void change(char str2[], double vector[], char letter[]);     //����һ������������ת����26ά��λ�����ĺ���  �üӵ����������庯������ 
void cosine(char str2[]);//��str2��answer��ʮ���ʱȽϣ���ֵҲӦ��0��1֮�䣬����ֵ�ֱ�ӵ�score
int min_of_three(int n1,int n2,int n3);
int edit(int i,int j,char source[20],char target[20]);
void editdistance(char str2[]);//��str2��answer��ʮ���ʱȽϣ�������Ҫ�����任���Ͱ�score�ķ�ֵ����log3��10*ԭscore��(��ǰ�Ǽ򵥼�ȥ)
void print();//��answer��ֵ��ߵ����������÷ְ��÷ָߵ�����������ʽ������
int main(){
	FILE*fp;
	int i=1,j;
	char ch;	
	if((fp=fopen("D:\\�𶭴�\\how to cheat\\Ӣ�Ĳ��\\����de�ʻ�.csv","r"))==NULL){//���Լ������ϱ༭ʱ��Ϊ���صĴ洢·�� 
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
	printf("��������鵥��(����$������ѯ)��");
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
			printf("δ�ڴʵ����ҵ�\n"); 
			jaccard(str2);
			cosine(str2);
			editdistance(str2);
			print();
		}
		else{
			printf("�ڴʵ�ĵ�%d�У���",i);
			puts(Str1[i]);
		}
		printf("��������鵥��(����$������ѯ)��");
		search();
	}
	return;
}
//vocabulary has \n or EOF
//ע��ƥ��ʱ�����ִ�Сд��Str1[1]-[1991]Ϊ�ʵ���Ĵʣ�Str1[1992]Ϊanyone��Str1[1993]Ϊbike��ȫ��Сд 1882 warm-hearted
//Str1��ÿ���ַ�����ʽΪaz\0
void jaccard(char str2[]){
	int object[26],source[26],posi[8000],i,j,k,its=0,uni=0;//its=intersection,uni=union
	double aim[8000],temp;
	for(i=1;i<=7678;i++)
		posi[i]=i;
	for(i=0;i<26;i++)
		object[i]=source[i]=0;
	for(i=0;i<strlen(str2);i++){ //26����ĸ ����Ϊ1 ����Ϊ0 
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
//		printf("%s %lf\n",answer[i],score[i]);  //����
	}     
	return;
} 
void cosine(char str2[])
{
	char letter[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
	                   'n','o','p','q','r','s','t','u','v','w','x','y','z'};     //����26����ĸ
	double vector1[26];               //�����������ѯ���ʵ�26ά��λ���� 
	double vector2[26];              // ���������ƥ�䵥�ʵ�26ά��λ���� 
	change( str2, vector1, letter);   //���ú�����str2ת����һ��26ά��λ���� 
	for(int i = 0; i < 15; i ++)
	{
		change(Answer[i], vector2, letter);   //���ú�����Answer[i]�ﴢ����Ǹ���ƥ�䵥��ת����26ά��λ����
		for(int j = 0; j < 26; j ++)
		{
			Score[i] = Score[i] + vector1[j] * vector2[j];            //����ÿ��cosineֵ������score[i]	
		} 
	}
} 


void change(char str2[], double vector[], char letter[])
{
	for(int i = 0; i < 26; i ++)
	{
		vector[i] = 0.0;             //���������Ԫ�صĳ�ʼ�� 
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
						if(str2[i+1] == letter[k])                            //�Ż���  abcde�����ֱ��Ӧ 1234567���� 
						{
							vector[j] = vector[j] + 1 + j + fabs(k - j);      //ÿ������ȡֵ = ��Ӧ��ĸ��ȡֵ + ��֮���ں�����ĸֵ֮��ľ���ֵ 
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
	double lenth = sqrt(maxsum);             //26ά������ģ����
	for(int i = 0; i < 26; i ++)            //��������λ�� 
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
//cosine���Ż� Ҫ������ĸ˳�������ڴ����Ͼ��� ������ת��������ʱ  ÿ��������ȡֵ���˰�����Ӧ��ĸ�� ASC��ֵ
//�ټ�������֮���ں�����ĸֵ֮��ľ���ֵ 
//�ڶ����Ż��ǣ�ԭ�����������õ�����ĸ��ӦASC��ֵ  ������ĸ abcde�����ֱ��Ӧ 1234567����  ��Ϊ�˼�С������

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
