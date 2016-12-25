#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
char str1[2000][20];//�洢�ֵ���Ĵ� 
double score[15];//��ѡ�ĵ÷�
char answer[15][20];//��ѡ�Ĵ�
void search();//�������о�ȷƥ�䣬���޸ôʽ���ģ��ƥ�� 
void jaccard(char str2[]);// ��str2�����дʱȽϣ� ����ֵ��ߵ�ʮ���ʴ��� answer����score��ÿ��Ԫ�ظ�ֵΪ0
void change(char str2[], double vector[], char letter[]);     //����һ������������ת����26ά��λ�����ĺ���  �üӵ����������庯������ 
void cosine(char str2[]);//��str2��answer��ʮ���ʱȽϣ���ֵҲӦ��0��1֮�䣬����ֵ�ֱ�ӵ�score
int min_of_three(int n1,int n2,int n3);
int edit_distance(char *a, char *b) ;
void editdistance(char str2[]);//��str2��answer��ʮ���ʱȽϣ�������Ҫ�����任���Ͱ�score�ķ�ֵ����log3��10*ԭscore��
void print();//��answer��ֵ��ߵ����������÷ְ��÷ָߵ�����������ʽ������
int main(){
	FILE*fp;
	int i=1,j;
	char ch;	
	if((fp=fopen("D:\\�𶭴�\\how to cheat\\Ӣ�Ĳ��\\vocabulary.csv","r"))==NULL){//���Լ������ϱ༭ʱ��Ϊ���صĴ洢·�� 
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
		while(i<=1993){
			if(strcmp(str1[i],str2)==0)break;
			i++;
		}
		if(i==1994){
			printf("δ�ڴʵ����ҵ�\n"); 
			jaccard(str2);
			cosine(str2);
	
			editdistance(str2);
			print();
		}
		else{
			printf("�ڴʵ�ĵ�%d�У���",i);
			puts(str1[i]);
		}
		printf("��������鵥��(����$������ѯ)��");
		search();
	}
	return;
}
//vocabulary has \n or EOF
//ע��ƥ��ʱ�����ִ�Сд��str1[1]-[1991]Ϊ�ʵ���Ĵʣ�str1[1992]Ϊanyone��str1[1993]Ϊbike��ȫ��Сд 1882 warm-hearted
//str1��ÿ���ַ�����ʽΪaz\0
void jaccard(char str2[]){
	int object[26],source[26],posi[2000],i,j,k,its=0,uni=0;//its=intersection,uni=union
	double aim[2000],temp;
	for(i=1;i<=1993;i++)
		posi[i]=i;
	for(i=0;i<26;i++)
		object[i]=source[i]=0;
	for(i=0;i<strlen(str2);i++){ //26����ĸ ����Ϊ1 ����Ϊ0 
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
		change( answer[i], vector2, letter);   //���ú�����answer[i]�ﴢ����Ǹ���ƥ�䵥��ת����26ά��λ����
		for(int j = 0; j < 26; j ++)
		{
			score[i] = score[i] + vector1[j] * vector2[j];            //����ÿ��cosineֵ������score[i]	
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
//cosine���Ż� Ҫ������ĸ˳�������ڴ����Ͼ��� ������ת��������ʱ  ÿ��������ȡֵ���˰�����Ӧ��ĸ�� ASC��ֵ
//�ټ�������֮���ں�����ĸֵ֮��ľ���ֵ 
//�ڶ����Ż��ǣ�ԭ�����������õ�����ĸ��ӦASC��ֵ  ������ĸ abcde�����ֱ��Ӧ 1234567����  ��Ϊ�˼�С������

void editdistance(char str2[]){

    int i,j,k=0;   
    char t[30];
    double n[10];
    for(i=0;i<15;i++){
        n[i]=(double)edit_distance(str2,answer[i])/(double)strlen(str2);    
                            //���������������Բ�����������ֵ����һ�������������� 
    }
    struct similarity{
        char str[30];
        double similar;
    };         //����ṹ�� 
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
    }                   //�Խṹ�����ð������ 
    for(i=0;i<15;i++){
    	strcpy(answer[i],answers[i].str);
    	score[i]=score[i]-answers[i].similar;
    //����ǰ������ƶ���ߵĵ��� 
    }
    return;
} 
int edit_distance(char *a, char *b)    //�������貽�� 
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
            // �㷨�� a, b �ַ����±�� 1 ��ʼ��c ���Դ� 0 ��ʼ������ -1
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

