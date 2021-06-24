/*
* @Author: jlu
* @Date:   2021-03-24 10:14:20
* @Last Modified by:   jlu
* @Last Modified time: 2021-04-21 09:10:59
*/

#include <iostream>
#include <string.h>
#include <cstdio>
#include <cstdlib>
using namespace std;

int isASCII_character(char temp);//是字符吗
int isASCII_number(char temp);//是数字吗
int isnumber_characters_string(void);//是否是数字与字符组成的字符串
int isIdentifier(void);//判断是否是标识符
char ReadCharacter(void);//每次读取一个单词
void scnner(void);//词法分析函数
int isReservedWord(void);//是否是保留字

int index=0;//字符索引
char filepath[10000]="123.txt";
FILE* OUTPUT=NULL;
int OUTPUTINTDEX=0;//输出文件索引

//词法分析字符栈
char stack[100000];
int top=0;
int ERROR=1;
int main(int argc, char *argv[]){
	
	
	if (argc!=2)
	{
		printf("NO Input File Or Input File Error!\n");
		ERROR=0;
		exit(0);
	}
	else
	{
		strcpy(filepath,argv[1]);
		printf("Input File %s Success!\n",filepath );
	}
	if((OUTPUT=fopen("token.txt","w"))==NULL)
	{
		printf("cannot open this file\n");
		ERROR=0;
		exit(0);
	}
	scnner();//词法分析
	fclose(OUTPUT);
    return ERROR;
}

char ReadCharacter()
{
    FILE* fp=NULL;
	if((fp=fopen(filepath,"r"))==NULL)
	{
		printf("cannot open this file\n");
		ERROR=0;
		exit(0);
	}
	char temp;
	fseek(fp,index,0);
	//展示没用的模块
	if (feof(fp))
	{
		return 0;
	}
	fscanf(fp,"%c",&temp);
	fclose(fp);
	return temp;
}



int isASCII_character(char temp)
{
	if ( ('a' <= temp && temp <='z')  || ('A' <= temp && temp <='Z') )
	{

	    stack[top++]=temp;
	    index++;
		return 1;
	}

	return 0;
}

int isASCII_number(char temp)
{
	if ( '0' <= temp && temp <='9'  )
	{
		stack[top++]=temp;
		index++;
		return 1;
	}

	return 0;
}

int isnumber_characters_string(void)
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=0;//成功标志
	//判断是否是  字母或者 字符
	if (isASCII_number(temp) || isASCII_character(temp))
	{
		//递归判断
		isnumber_characters_string();
	}
	// //遇到结尾符号返回true
	// if( temp==' ' || temp==0 || temp=='\n' || temp==';')
 //    {
	// 	return 1;
 //    }
	// else
	// {
	// 	return flag;
	// }
	return 1;
}

int isIdentifier(void)
{
	char temp =ReadCharacter();//读取一个字符
	//判断是否是  字母或者 字符
	if (isASCII_character(temp) && isnumber_characters_string())
	{
		return 1;
	}
	index-=top ;
	top=0 ;
	return 0;
}



int isReservedWord(void)
{

	char reservedArray[22][10];
	strcpy(reservedArray[0] , "program"  );
	strcpy(reservedArray[1] , "integer"  );
	strcpy(reservedArray[2] , "char"     );
	strcpy(reservedArray[3] , "var"      );
	strcpy(reservedArray[4] , "type"     );
	strcpy(reservedArray[5] , "intc"     );
	strcpy(reservedArray[6] , "record"   );
	strcpy(reservedArray[7] , "end"      );
	strcpy(reservedArray[8] , "procedure");
	strcpy(reservedArray[9] , "begin"    );
	strcpy(reservedArray[10], "if"       );
	strcpy(reservedArray[11], "then"     );
	strcpy(reservedArray[12], "else"     );
	strcpy(reservedArray[13], "fi"       );
	strcpy(reservedArray[14], "while"    );
	strcpy(reservedArray[15], "do"       );
	strcpy(reservedArray[16], "endwh"    );
	strcpy(reservedArray[17], "read"     );
	strcpy(reservedArray[18], "write"    );
	strcpy(reservedArray[19], "return"   );
	strcpy(reservedArray[20], "array"    );
	strcpy(reservedArray[21], "of"       );
	stack[top]='\0';
	for (int i = 0; i < 22; ++i)
	{
		if (strcmp(reservedArray[i],stack)==0)
		{
			return 1;
		}
	}
	return 0;
}

int isINTC(void)
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=1;//成功标志
	if (isASCII_number(temp))
	{
		temp =ReadCharacter();//读取一个字符
		if (temp=='+' || temp=='-' ||temp=='*' ||temp=='/'
			||temp=='<' ||temp=='=' ||temp=='(' ||temp==')'
			||temp=='[' ||temp==']' ||temp=='.' ||temp==';'
			||temp==' ' ||temp=='\t' ||temp=='\r'||temp=='\n' || temp==',')
		{
			return 1;
		}
		return isINTC();
	}
	else if (isASCII_character(temp))
	{
		return 0;
	}
	// //遇到结尾符号返回true
	// if( isASCII_character(temp))
 //    {
	// 	return 0;
 //    }
	// else
	// {
	// 	return flag;
	// }
	// printf("be index:%d  top:%d\n",index,top );
	// index-=top ;
	// printf("be index:%d  top:%d\n",index,top );
	// top=0 ;
	return 0;
}

int isSingleCharacter(void)
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=1;//成功标志
	if (temp=='+' || temp=='-' ||temp=='*' ||temp=='/'
		||temp=='<' ||temp=='=' ||temp=='(' ||temp==')'
		||temp=='[' ||temp==']' ||temp=='.' ||temp==';'
		||temp==' ' ||temp=='\t' ||temp=='\r'||temp=='\n' || temp==',')
	{
		// printf("%d\n",temp );
		stack[top++]=temp;
		index++;
		return 1;
	}
	// index-=top ;
	// top=0 ;
	// printf("%c\n",temp );
	return 0;
}


int isDoubleCharacter(void)
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=1;//成功标志
	if (temp==':')
	{
		stack[top++]=temp;
		index++;
		temp =ReadCharacter();//读取一个字符
		if (temp == '=' )
		{
			stack[top++]=temp;
			index++;
			return 1;
		}
	}
	index-=top ;
	top=0 ;
	// printf("%c\n",temp );
	return 0;
}


int ishead()
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=1;//成功标志
	if (temp=='{')
	{
		// printf("%d\n",temp );
		stack[top++]=temp;
		index++;
		return 1;
	}
	// index-=top ;
	// top=0 ;
	// printf("%c\n",temp );
	return 0;
}

int istail()
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=1;//成功标志
	if (temp=='}')
	{
		// printf("%d\n",temp );
		stack[top++]=temp;
		index++;
		return 1;
	}
	// index-=top ;
	// top=0 ;
	// printf("%c\n",temp );
	return 0;
}

void getString(void)
{
	char temp =ReadCharacter();//读取一个字符
	char* strings=new char[1000000];
	int strings_top=0;
	while(temp!='\'')
	{
		if (temp==' ')
		{
			strings[strings_top++]='\\';
			strings[strings_top++]='S';
		}
		else
			strings[strings_top++]=temp;
		index++;
		temp=ReadCharacter();
		if (temp=='\\')
		{
			index++;
			if (temp=='\'')
			{
				strings[strings_top++]='\'';
				index++;
			}
			else
			{
				index--;
			}
		}
	}
	strings[strings_top]='\0';
	fprintf(OUTPUT,"< STR , %s >",strings );
	delete strings;
}

int isStrSeparate()
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=1;//成功标志
	if (temp=='\'' )
	{
		stack[top++]=temp;
		index++;
		stack[top]='\0';
		fprintf(OUTPUT,"< SEP , %s >",stack);
		getString();
		temp =ReadCharacter();//读取一个字符
		top=0;
		stack[top++]=temp;
		index++;
		return 1;
	}
	// index-=top ;
	// top=0 ;
	// printf("%c\n",temp );

	return 0;
}


int isArrayUnder()
{
	char temp =ReadCharacter();//读取一个字符
	// int flag=1;//成功标志
	if (temp=='.')
	{

		stack[top++]=temp;
		index++;
		temp =ReadCharacter();//读取一个字符
		if (temp == '.' )
		{
			stack[top++]=temp;
			index++;
			return 1;
		}
	}
	index-=top ;
	top=0 ;
	// printf("%c\n",temp );
	return 0;

}

void scnner(void)
{
	int lines=1;
	int zhushi=0;
	while(1)
	{
		top=0;
		if (zhushi==0 && isIdentifier())//标识符
		{
			stack[top]='\0';
			if (isReservedWord())
			{
				fprintf(OUTPUT,"< RE , %s >",stack);
			}
			else
			{
				fprintf(OUTPUT,"< ID , %s >",stack);
			}

		}
		else if (zhushi==0 && isINTC())//无符号整数
		{
			stack[top]='\0';
			fprintf(OUTPUT,"< INTC , %s >",stack);
		}
		else if (zhushi==0 && isStrSeparate())//字符串分隔符
		{
			stack[top]='\0';
			fprintf(OUTPUT,"< SEP , %s >",stack);
		}
		else if (zhushi==0 && isDoubleCharacter())//赋值符号
		{
			stack[top]='\0';
			fprintf(OUTPUT,"< DCHS , %s >",stack);
		}
		else if (zhushi==0 && isArrayUnder())
		{
			stack[top]='\0';
			fprintf(OUTPUT,"< AU , %s >",stack);
		}
		else if (zhushi==0 && isSingleCharacter())
		{
			stack[top]='\0';
			if (stack[0]=='\n')
			{
				lines++;
				fprintf(OUTPUT,"< SCHS , \\n >",stack);
			}
			else if (stack[0]=='\r' || stack[0]=='\t' || stack[0]==' ')
			{

			//
			}
			else
				fprintf(OUTPUT,"< SCHS , %s >",stack);
			// printf("%s",stack);
		}
		else if (ishead())
		{
			zhushi++;
			stack[top]='\0';
			// printf("< LH , %s >\n",stack);
		}
		else if (istail())
		{
			zhushi--;
			stack[top]='\0';
			// printf("< RH , %s >\n",stack);
		}
		else if (zhushi!=0)
		{
			char tp =ReadCharacter();//读取一个字符
			if (tp=='\n')
			{
				fprintf(OUTPUT,"< SCHS , \\n >");
				lines++;
			}
			index++;
		}

		else
		{
			// index++;
			char temp=ReadCharacter();
			while (temp!='+' && temp!='-' &&temp!='*' &&temp!='/'
				&&temp!='<' &&temp!='=' &&temp!='(' &&temp!=')'
				&&temp!='[' &&temp!=']' &&temp!='.' &&temp!=';'
				&&temp!=' ' &&temp!='\t' &&temp!='\r'&&temp!='\n' && temp!=',')
			{
				// printf("%d\n",temp );
				stack[top++]=temp;
				index++;
				temp=ReadCharacter();
			}
			stack[top]='\0';
			printf("ERROR\n");
			printf("<ERROR,lines:%d and \"%s\" >\n",lines/2+1,stack);
			ERROR=0;
			// printf("There is something wrong on Lines:%d\n", );
		}

		//判断词法分析是否结束
		FILE* fp=NULL;
		if((fp=fopen(filepath,"r"))==NULL)
		{
			printf("cannot open this file\n");
			ERROR=0;
			exit(0);
		}
		fseek(fp,index,0);
		char ch=fgetc(fp);
		if (ch==EOF)
		{
			fclose(fp);
			break;
		}
		fclose(fp);
	}
}
