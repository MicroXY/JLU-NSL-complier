/*
* @Author: 远浪
* @Date:   2021-04-02 19:35:29
* @Last Modified by:   jlu
* @Last Modified time: 2021-04-28 10:02:11
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;
int ERROR=1;
//int表
struct integerTable
{
	string name;//变量类型名
	int len;//所占字节数
	int vaule;//整数值
};
//char表
struct charTable
{
	string name;//变量类型名
	int len;//所占字节数
	char vaule;//字符值
};

//const INTC 表
struct constTable
{
	string name;//变量类型名
	int len;//所占字节数
	int vaule;//常量值
};

// 结构体域表
struct structfiledTable
{
	string name;//变量类型名
	string varname;//名称
	int len;//所占子结束
	int basekind;//基类型1：int ,2 char,3 INTC,4 Struct,5 Array,6 procedure,负数表示typeTable 的行数对应的类型的基类型
	void* typePtr;//基类型指针
	structfiledTable* next;//域的下一项
};

//结构体表
struct structTable
{
	string name;//变量类型名
	int len;//所占子结束
	int lenth;//结构体域的项数
	structfiledTable* next;//指向域的指针
};

// 数组表
struct arrayTable
{
	string name;//变量类型名
	int len;//数组占字节数
	int basekind;//基类型1：int ,2 char,3 INTC,4 Struct,5 Array,6 procedure,负数表示typeTable 的行数对应的类型的基类型
	// void* typePtr;//基类型指针
	int low;
	int high;//数组上下界
};

struct parameterTable
{
	string varname;//变量类型名
	int len;//数组占字节数
	int basekind;//基类型1：int ,2 char,3 INTC,4 Struct,5 Array,6 procedure,负数表示typeTable 的行数对应的类型的基类型
	void* typePtr;//基类型指针
	parameterTable* next;//下一个参数
};

//函数过程表
struct procedureTable
{
	string name;//变量类型名
	int basekind;//返回值基类型
	void* typePtr;//返回值类型指针
	int len;//恒为0
	int num;//参数个数
	int level;
	parameterTable* next;//参数列表
};

//自定义类型变量表
struct typeTable
{
	string name;//变量类型名
	string typenames;//类型名
	int len;//数组占字节数
	int basekind;//基类型1：int ,2 char,3 INTC,4 Struct,5 Array,6 procedure,负数表示typeTable 的行数对应的类型的基类型
	int level;
	int isend;//0不是，1是
	int headenddestionation;
	void* typePtr;//类型指针
};

//全局符号表
struct globalTable
{
	string varname;//变量名
	string name;//变量类型名
	void* typePtr;//变量基类型
	int dic;//变量访问方式 0 直接变量 1间接变量
	int level;//层数
	int offset;//偏移
	int isend;//0不是，1是
	int headenddestionation;
};

struct token
{
	string key;
	string vaule;
};

class Semantic_Analysis
{
public:
	Semantic_Analysis()
	{}
	~Semantic_Analysis()
	{}
	//读取一个token
	token ReadToken(string regex="< %s , %s >%*[\n]")
	{
		FILE* fp=NULL;//文件指针
		if((fp=fopen(filepath.c_str(),"r"))==NULL)
		{
			printf("cannot open this file\n");
			ERROR=0;
			exit(0);
		}
		char key[100];
		char vaule[100];

		int temp=token_off;//虚拟偏移
		//定位文件指针到要读取的地方
		while(!feof(fp) && temp!=0)
		{
			fscanf(fp,"< %*[^ ] , %*[^ ] >%*[\n]");
			temp--;
		}
		token tp;
		if (feof(fp))//文件结束
		{
			fclose(fp);
			tp.key="EOF";
			tp.vaule="EOF";
			return tp;
		}
		// printf("REXGEX: %s\n",regex.c_str() );
		fscanf(fp,regex.c_str(),key,vaule);


		fclose(fp);
		tp.key=key ;
		tp.vaule=vaule;
		return tp;
	}
	//读取下一个token
	void NextToken(int num=1)
	{
		for (int i = 0; i < num; ++i)
		{
			token_off=token_off+1;
			token temp=ReadToken();
			while(temp.vaule=="\\n")
			{
				lines++;
				token_off=token_off+1;
				temp=ReadToken();
			}
		}
	}
	void BackToken(int num=1)
	{
		for (int i = 0; i < num; ++i)
		{
			token_off=token_off-1;
			token temp=ReadToken();
			while(temp.vaule=="\\n")
			{
				lines++;
				token_off=token_off-1;
				temp=ReadToken();
			}
		}
	}

	int TypeDomainBaseTypeAnalysis(token temp_id,token tk)
	{
		int flag=checkTypeTable(temp_id,tk);//检查表
		if (flag==1)
		{
			addTypeTable(temp_id,tk);//加入表
		}
		return flag;
	}

	int TypeDomainArrayTypeAnalysis(token id,token type)
	{
		//数组相关操作
		NextToken(2);//略过[
		int low=atoi(ReadToken().vaule.c_str());
		NextToken(2);//略过 ..
		int high=atoi(ReadToken().vaule.c_str());
		NextToken(3);//略过] of
		token basetype=ReadToken();

		//数组语义检查
		for (int i = typetablenum-1; i >= 0 ; --i)
		{
			if (typetable[i].isend==1)
			{
				i=typetable[i].headenddestionation;
				if (i==-1)
				{
					break;
				}
			}
			if (typetable[i].typenames==id.vaule && typetable[i].level==level)
			{
				printf("ERROR Repeatily Define Type ! lines:%d  type %s already define before!!!\n",lines/2, typetable[i].typenames.c_str() );
				ERROR=0;
				return 0;
			}
		}
		if (low > high)
		{
			printf("ERROR Array Low > High ! lines:%d !!!\n",lines/2 );
			ERROR=0;
			return 0;
		}
		//添加表的过程
		typetable[typetablenum].name="type";
		typetable[typetablenum].typenames=id.vaule;
		typetable[typetablenum].level=level;
		typetable[typetablenum].isend=0;
		typetable[typetablenum].headenddestionation=temptypenum;//

		if (basetype.vaule=="integer")
		{
			typetable[typetablenum].len=(high-low+1)*4;
			typetable[typetablenum].basekind=5;
			arrayTable* tps=new arrayTable;
			tps->name="array";
			tps->len=(high-low+1)*4;
			tps->basekind=1;
			tps->low=low;
			tps->high=high;
			typetable[typetablenum].typePtr=(void *)tps;
		}
		else if (basetype.vaule=="char")
		{
			typetable[typetablenum].len=(high-low+1)*1;
			typetable[typetablenum].basekind=5;
			arrayTable* tps=new arrayTable;
			tps->name="array";
			tps->len=(high-low+1)*1;
			tps->basekind=2;
			tps->low=low;
			tps->high=high;
			typetable[typetablenum].typePtr=(void *)tps;
		}
		else
		{
			printf("Unkonw ERROR!\n");
			ERROR=0;
		}
		typetablenum++;
	}

	//检查表
	int checkTypeTable(token id,token type)
	{
		int exists=0;//基类型变量是否存在
		if (type.vaule=="integer" || type.vaule=="char" )
		{
			exists=1;//基类型变量真的存在
		}
		for (int i = typetablenum-1; i >= 0 ; --i)
		{
			if (typetable[i].isend==1)
			{
				i=typetable[i].headenddestionation;
				if (i==-1)
				{
					break;//表头了
				}
			}
			if (typetable[i].typenames==id.vaule && typetable[i].level==level)
			{
				printf("ERROR Repeatily Define Type ! lines:%d  type %s already define before!!!\n",lines/2, typetable[i].typenames.c_str() );
				ERROR=0;
				return 0;
			}
			if (exists==0 && typetable[i].typenames==type.vaule)
			{
				exists=1;
			}
		}
		if (exists==0)
		{
			printf("YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n");
			printf("ERROR Unknown Type ! lines:%d  type %s doesn't define before!!!\n",lines/2, type.vaule.c_str() );
			ERROR=0;
			return 0;
		}
		return 1;
	}
	//加入TYPE表
	void addTypeTable(token id ,token type )
	{
		typetable[typetablenum].name="type";
		typetable[typetablenum].typenames=id.vaule;
		typetable[typetablenum].level=level;
		typetable[typetablenum].isend=0;//没有结束
		typetable[typetablenum].headenddestionation=temptypenum;//向回指
		//基类型选择
		if (type.vaule=="integer")
		{
			typetable[typetablenum].basekind=1;
			typetable[typetablenum].len=4;
			integerTable* tps=new integerTable;
			tps->name="integer";
			tps->len=4;
			tps->vaule=NULL;
			typetable[typetablenum].typePtr=(void*)tps;
		}
		else if (type.vaule=="char")
		{
			typetable[typetablenum].basekind=2;
			typetable[typetablenum].len=1;
			charTable* tps=new charTable;
			tps->name="char";
			tps->len=1;
			tps->vaule=NULL;
			typetable[typetablenum].typePtr=(void*)tps;
		}
		else if (type.key=="ID")
		{
			int l=0;
			for (int i = 0; i < typetablenum; ++i)
			{
				if (typetable[i].typenames==type.vaule)
				{
					l=i;
				}
			}
			typetable[typetablenum].basekind=typetable[l].basekind;
			typetable[typetablenum].len     =typetable[l].len;
			typetable[typetablenum].typePtr =typetable[l].typePtr;
		}
		else
		{
			ERROR=0;
			printf("Unkonw ERROR!!!\n");
		}
		typetablenum++;
	}

	//类型定义域
	void TypeDomain(void)
	{
		NextToken();//略去type
		while(1)
		{
			token temp_id = ReadToken();
			if ( temp_id.key == "RE" && (temp_id.vaule =="var" || temp_id.vaule =="procedure" || temp_id.vaule =="begin") )
			{
				break;
			}
			NextToken(2);//略去=
			token tk=ReadToken();
			if ( tk.key=="ID" || (tk.key=="RE" && ( tk.vaule=="integer" || tk.vaule=="char" )) )
			{
				TypeDomainBaseTypeAnalysis(temp_id,tk);
			}
			else if (tk.key=="RE" && tk.vaule=="array")
			{
				TypeDomainArrayTypeAnalysis(temp_id,tk);
			}
			else if (tk.key=="RE" && tk.vaule=="record")
			{
				NextToken();
				int f=1;
				//数组语义检查
				for (int i = typetablenum -1; i >= 0; --i)
				{
					if (typetable[i].isend==1)
					{
						i=typetable[i].headenddestionation;
						if (i==-1)
						{
							break;
						}
					}
					if (typetable[i].typenames==temp_id.vaule && typetable[i].level==level)
					{
						printf("ERROR Repeatily Define Type ! lines:%d  type %s already define before!!!\n",lines/2, typetable[i].typenames.c_str() );
						f=0;
						ERROR=0;
					}
				}
				typetable[typetablenum].name="type";
				typetable[typetablenum].typenames=temp_id.vaule;

				typetable[typetablenum].level=level;
				typetable[typetablenum].isend=0;
				typetable[typetablenum].headenddestionation=temptypenum;
				typetable[typetablenum].len=0;
				typetable[typetablenum].basekind=4;
				structTable* tps=new structTable;
				tps->name="struct";
				tps->len=0;
				tps->lenth=0;
				tps->next=NULL;
				typetable[typetablenum].typePtr=(void *)tps;

				token left=ReadToken();
				//结束标志
				while (left.vaule!="end")
				{
					if (left.key=="RE" && ( left.vaule=="integer" || left.vaule=="char" ))
					{
						NextToken();
						token tp_name=ReadToken();
						while(tp_name.vaule!=";")//;号
						{
							if (tp_name.key=="ID")
							{
								void* struct_void=typetable[typetablenum].typePtr;
								structTable* t_void=(structTable*)struct_void;
								structfiledTable* p = t_void->next;
								if ( t_void->lenth==0 )
								{
									t_void->next=new structfiledTable;
									t_void->next->name="structfiled";
									t_void->next->varname=tp_name.vaule;
									if ( left.vaule=="integer" )
									{
										t_void->next->len=4;
										t_void->next->basekind=1;
										integerTable* iT=new integerTable;
										iT->name="integer";
										iT->len=4;
										iT->vaule=NULL;
										t_void->next->typePtr=(void *)iT;
										t_void->len+=4;
										typetable[typetablenum].len+=4;
									}
									else if ( left.vaule=="char" )
									{
										t_void->next->len=1;
										t_void->next->basekind=2;
										charTable* cT = new charTable;
										cT->name="char";
										cT->len=1;
										cT->vaule=NULL;
										t_void->next->typePtr=(void *)cT;
										t_void->len+=1;
										typetable[typetablenum].len+=1;
									}
									t_void->next->next=NULL;
								}
								else
								{
									while(p->next!=NULL)
									{
										p=p->next;
									}
									p->next=new structfiledTable;
									p->next->name="structfiled";
									p->next->varname=tp_name.vaule;
									if ( left.vaule=="integer" )
									{
										p->next->len=4;
										p->next->basekind=1;
										t_void->len+=4;
										typetable[typetablenum].len+=4;
										integerTable* iT=new integerTable;
										iT->name="integer";
										iT->len=4;
										iT->vaule=NULL;
										p->next->typePtr=(void *)iT;
									}
									else if ( left.vaule=="char" )
									{
										p->next->len=1;
										p->next->basekind=2;
										t_void->len+=1;
										typetable[typetablenum].len+=1;
										charTable* cT = new charTable;
										cT->name="char";
										cT->len=1;
										cT->vaule=NULL;
										p->next->typePtr=(void *)cT;
									}
									p->next->next=NULL;
								}
								t_void->lenth++;
							}
							NextToken();
							tp_name = ReadToken();
						}
					}
					else if (left.key=="RE" && left.vaule=="array")
					{
						//数组相关操作
						NextToken(2);//略过[
						int low=atoi(ReadToken().vaule.c_str());
						NextToken(2);//略过 ..
						int high=atoi(ReadToken().vaule.c_str());
						NextToken(3);//略过] of
						token basetype=ReadToken();
						if (low > high)
						{
							printf("ERROR Array Low > High ! lines:%d !!!\n",lines/2 );
							f=0;
							ERROR=0;
						}
						NextToken();
						token tp_name=ReadToken();// 读取变量名
						while( tp_name.vaule!=";")//;号
						{
							if (tp_name.key=="ID")
							{
								//数组加入表
								void* struct_void=typetable[typetablenum].typePtr;
								structTable* t_void=(structTable*)struct_void;
								structfiledTable* p = t_void->next;
								if ( t_void->lenth==0 )
								{
									t_void->next=new structfiledTable;
									t_void->next->name="structfiled";
									t_void->next->varname=tp_name.vaule;
									if ( basetype.vaule=="integer" )
									{
										t_void->next->len=(high-low+1)*4;
										t_void->next->basekind=5;
										t_void->len+=(high-low+1)*4;
										typetable[typetablenum].len+=(high-low+1)*4;
										arrayTable* aT=new arrayTable;
										aT->name="integer";
										aT->len=(high-low+1)*4;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										t_void->next->typePtr=(void *)aT;
									}
									else if ( basetype.vaule=="char" )
									{
										t_void->next->len=(high-low+1)*1;
										t_void->next->basekind=5;
										t_void->len+=(high-low+1)*1;
										typetable[typetablenum].len+=(high-low+1)*1;
										arrayTable* aT=new arrayTable;
										aT->name="char";
										aT->len=(high-low+1)*1;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										t_void->next->typePtr=(void *)aT;
									}
									t_void->next->next=NULL;
								}
								else
								{
									while(p->next!=NULL)
									{
										p=p->next;
									}
									p->next=new structfiledTable;
									p->next->name="structfiled";
									p->next->varname=tp_name.vaule;
									if ( basetype.vaule=="integer" )
									{
										p->next->len=(high-low+1)*4;
										p->next->basekind=5;
										t_void->len+=(high-low+1)*4;
										typetable[typetablenum].len+=(high-low+1)*4;
										arrayTable* aT=new arrayTable;
										aT->name="integer";
										aT->len=(high-low+1)*4;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										p->next->typePtr=(void *)aT;
									}
									else if ( basetype.vaule=="char" )
									{
										p->next->len=(high-low+1)*1;
										p->next->basekind=5;
										t_void->len+=(high-low+1)*1;
										typetable[typetablenum].len+=(high-low+1)*1;
										arrayTable* aT=new arrayTable;
										aT->name="char";
										aT->len=(high-low+1)*1;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										p->next->typePtr=(void *)aT;
									}
									p->next->next=NULL;
								}
								t_void->lenth++;
							}
							NextToken();
							tp_name=ReadToken();// 读取下一个变量名
						}
					}
					else
					{
						// error();
						ERROR=0;
						printf("SomeWrong!!!\n");
					}
					NextToken();
					left=ReadToken();
				}
				if (f==1)//无效
				{
					typetablenum++;
				}
			}
			NextToken(2);//略去末尾的;
		}
	}


	// 1.查看自定义变量是否定义过，向上查找遇到#就跳到#所指向的表
	// 2.查看变量名是否重复定义
	// 3.找到类型的基类型
	// 4.加入global表（考虑off和level）
	int VarDomainBaseTypeAnalysis(token type,token varname)//type 类型名，varname变量名
	{
		if ( (type.key=="ID") || (type.key=="RE" && (type.vaule=="integer" || type.vaule=="char")) )//自定义类型
		{
			int exists=0;//基类型变量是否存在
			int index=-1;
			if (type.key=="RE")
			{
				if ( type.vaule == "integer" )
				{
					index=-2;
				}
				else
					index=-3;
				exists=1;
			}

			for (int i = typetablenum-1 ; i >= 0 ; --i)
			{
				if (typetable[i].isend==1)
				{
					i=typetable[i].headenddestionation;

					if (i==-1)
					{
						break;
					}
				}
				if (typetable[i].typenames==varname.vaule && typetable[i].level==level)
				{
					printf("11111ERROR OverWrite Defined Type ! lines:%d  type %s already define before!!!\n",lines/2, varname.vaule.c_str() );
					// return 0;
				}
				if (exists==0 && typetable[i].typenames==type.vaule)
				{
					exists=1;
					index=i;
				}
			}
			if (exists==0)
			{
				printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n");
				ERROR=0;
				printf("ERROR Unknown Type ! lines:%d  type %s doesn't define before!!!\n",lines/2, type.vaule.c_str() );
				// return 0;
			}
			//上面是检查部分
			//下面加入global表
			//全局符号表
			for (int i = globaltablenum-1; i>=0  ; --i)
			{
				if (globaltable[i].isend==1)
				{
					i=globaltable[i].headenddestionation;
					if (i==-1)
					{
						break;
					}
				}
				if (globaltable[i].varname==varname.vaule && globaltable[i].level==level )
				{
					ERROR=0;
					printf("ERROR Repeatily Define Var ! lines:%d  Var %s already define before!!!\n",lines/2, varname.vaule.c_str() );
					// return 0;
				}
			}
			globaltable[globaltablenum].varname=varname.vaule;
			globaltable[globaltablenum].dic=1;//变量访问方式 0 直接变量 1间接变量
			globaltable[globaltablenum].level=level;
			globaltable[globaltablenum].offset=var_off;
			globaltable[globaltablenum].isend=0;
			globaltable[globaltablenum].headenddestionation=tempglobalnum;

			if (type.key=="ID")
			{
				globaltable[globaltablenum].name="type";
				globaltable[globaltablenum].typePtr=&typetable[index];
				if (index<0)
				{
					;
				}
				else
					var_off+=typetable[index].len;
			}
			else if (type.vaule=="integer")
			{
				globaltable[globaltablenum].name=type.vaule;
				integerTable* iT=new integerTable;
				iT->name="integer";
				iT->len=4;
				iT->vaule=NULL;
				globaltable[globaltablenum].typePtr=iT;
				var_off+=4;
			}
			else if (type.vaule=="char")
			{
				globaltable[globaltablenum].name=type.vaule;
				charTable* cT=new charTable;
				cT->name="char";
				cT->len=1;
				cT->vaule=NULL;
				globaltable[globaltablenum].typePtr=cT;
				var_off+=1;
			}
		}
		else if (type.vaule==",")
		{
			;
		}
		else
		{
			ERROR=0;
			printf( "KEY:%s  VAULE:%s\n" , type.key.c_str(),type.vaule.c_str() );
			printf( "KEY:%s  VAULE:%s\n" , varname.key.c_str(),varname.vaule.c_str() );
			printf("lines: %s Unknow ERROR!!!\n",lines/2);
			// return 0;
		}
		globaltablenum++;
		return 1;
	}

	int VarDomainArrayTypeAnalysis(token type,token varname)
	{
		//数组相关操作
		NextToken();//略过[
		int low=atoi(ReadToken().vaule.c_str());
		NextToken(2);//略过 ..
		int high=atoi(ReadToken().vaule.c_str());
		NextToken(3);//略过] of
		token basetype=ReadToken();
		if (low > high)
		{
			ERROR=0;
			printf("ERROR Array Low > High ! lines:%d !!!\n",lines/2 );

		}
		NextToken();

		token realvarname=ReadToken();

		while(realvarname.vaule!=";")
		{
			if (realvarname.key=="ID")
			{
				int f=1;
				for (int i = typetablenum-1 ; i >= 0 ; --i)
				{
					if (typetable[i].isend==1)
					{
						i=typetable[i].headenddestionation;
						if (i==-1)
						{
							break;
						}
					}
					if (typetable[i].typenames==realvarname.vaule )
					{
						printf("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG\n");
						ERROR=0;
						printf("ERROR Repeatily Define Type ! lines:%d  type %s already define before!!!\n",lines/2, realvarname.vaule.c_str() );
						f=0;
					}
				}
				for (int i = globaltablenum-1; i>=0  ; --i)
				{
					if (globaltable[i].isend==1)
					{
						i=globaltable[i].headenddestionation;
						if (i==-1)
						{
							break;
						}
					}
					if (globaltable[i].varname==realvarname.vaule && globaltable[i].level==level )
					{
						ERROR=0;
						printf("ERROR Repeatily Define Var ! lines:%d  Var %s already define before!!!\n",lines/2, realvarname.vaule.c_str() );
						f=0;
					}
				}

				globaltable[globaltablenum].varname=realvarname.vaule;
				globaltable[globaltablenum].dic=0;//变量访问方式 0 直接变量 1间接变量
				globaltable[globaltablenum].level=level;
				globaltable[globaltablenum].offset=var_off;
				globaltable[globaltablenum].isend=0;
				globaltable[globaltablenum].headenddestionation=tempglobalnum;

				//添加表的过程
				if (basetype.vaule=="integer")
				{
					globaltable[globaltablenum].name="array";
					arrayTable* aT=new arrayTable;
					aT->name="array";
					aT->len=(high-low+1)*4;
					aT->basekind=1;
					aT->low=low;
					aT->high=high;
					globaltable[globaltablenum].typePtr=aT;
					var_off+=(high-low+1)*4;
				}
				else if (basetype.vaule=="char")
				{
					globaltable[globaltablenum].name="array";
					arrayTable* aT=new arrayTable;
					aT->name="array";
					aT->len=(high-low+1)*1;
					aT->basekind=2;
					aT->low=low;
					aT->high=high;
					globaltable[globaltablenum].typePtr=aT;
					var_off+=(high-low+1)*1;
				}
				else
				{
					ERROR=0;
					printf("Unkonw ERROR lines: %d!\n",lines/2);
					f=0;
				}
				if (f==1)
				{
					globaltablenum++;
				}

			}
			NextToken();
			realvarname=ReadToken();
		}
		return 1;
	}
	//变量定义域
	void VarDomain(void)
	{
		NextToken();//略去var
		while(1)
		{
			token temp_id = ReadToken();//类型名
			if ( temp_id.key == "RE" && (temp_id.vaule =="procedure" || temp_id.vaule =="begin") )
			{
				break;
			}
			NextToken();
			token tk=ReadToken();//定义的变量名
			if ( temp_id.key=="ID" || (temp_id.key=="RE" && ( temp_id.vaule=="integer" || temp_id.vaule=="char" )) )
			{
				while(tk.vaule!=";")
				{
					if ( tk.key=="ID" )
					{
						// 1.查看自定义变量是否定义过，向上查找遇到#就跳到#所指向的表
						// 2.查看变量名是否重复定义
						// 3.找到类型的基类型
						// 4.加入global表（考虑off和level）
						VarDomainBaseTypeAnalysis(temp_id,tk);
					}
					else if (tk.vaule==",")
					{
						/* code */
					}
					else
					{
						ERROR=0;
						printf("Unknow ERROR lines:%d !!!\n",lines/2);
					}
					NextToken();//下一个变量
					tk=ReadToken();//定义的变量名
				}
			}
			else if (temp_id.key=="RE" && temp_id.vaule=="array")
			{
				// 1.读取数组基本信息
				// 2.检查数组越界
				// 3.检查变量名是否重复定义
				// 4.加入global表（考虑off和level）
				VarDomainArrayTypeAnalysis(temp_id,tk);

			}
			else if (temp_id.key=="RE" && temp_id.vaule=="record")
			{
				// 1.读取结构体基本信息
				// 2.检查结构体内数组越界
				// 3.检查结构体变量名是否重复定义
				// 4.加入global表（考虑off和level）
				structTable* tps=new structTable;
				tps->name="struct";
				tps->len=0;
				tps->lenth=0;
				tps->next=NULL;

				int lens=0;//记录结构体大小
				token left=tk;
				//结束标志
				while (left.vaule!="end")
				{
					if (left.key=="RE" && ( left.vaule=="integer" || left.vaule=="char" ))
					{
						NextToken();
						token tp_name=ReadToken();
						while(tp_name.vaule!=";")//;号
						{
							if (tp_name.key=="ID")
							{
								structTable* t_void=tps;
								structfiledTable* p = t_void->next;
								if ( t_void->lenth==0 )
								{
									t_void->next=new structfiledTable;
									t_void->next->name="structfiled";
									t_void->next->varname=tp_name.vaule;
									if ( left.vaule=="integer" )
									{
										t_void->next->len=4;
										t_void->next->basekind=1;
										integerTable* iT=new integerTable;
										iT->name="integer";
										iT->len=4;
										iT->vaule=NULL;
										t_void->next->typePtr=(void *)iT;
										t_void->len+=4;
										lens+=4;
									}
									else if ( left.vaule=="char" )
									{
										t_void->next->len=1;
										t_void->next->basekind=2;
										charTable* cT = new charTable;
										cT->name="char";
										cT->len=1;
										cT->vaule=NULL;
										t_void->next->typePtr=(void *)cT;
										t_void->len+=1;
										lens+=1;
									}
									t_void->next->next=NULL;
								}
								else
								{
									while(p->next!=NULL)
									{
										p=p->next;
									}
									p->next=new structfiledTable;
									p->next->name="structfiled";
									p->next->varname=tp_name.vaule;

									if ( left.vaule=="integer" )
									{
										p->next->len=4;
										p->next->basekind=1;
										t_void->len+=4;
										lens+=4;
										integerTable* iT=new integerTable;
										iT->name="integer";
										iT->len=4;
										iT->vaule=NULL;
										p->next->typePtr=(void *)iT;
									}
									else if ( left.vaule=="char" )
									{
										p->next->len=1;
										p->next->basekind=2;
										t_void->len+=1;
										lens+=1;
										charTable* cT = new charTable;
										cT->name="char";
										cT->len=1;
										cT->vaule=NULL;
										p->next->typePtr=(void *)cT;
									}
									p->next->next=NULL;
								}
								t_void->lenth++;
							}
							NextToken();
							tp_name = ReadToken();
						}
					}
					else if (left.key=="RE" && left.vaule=="array")
					{
						//数组相关操作
						NextToken(2);//略过[
						int low=atoi(ReadToken().vaule.c_str());
						NextToken(2);//略过 ..
						int high=atoi(ReadToken().vaule.c_str());
						NextToken(3);//略过] of
						token basetype=ReadToken();
						if (low > high)
						{
							ERROR=0;
							printf("ERROR Array Low > High ! lines:%d !!!\n",lines/2 );
						}
						NextToken();
						token tp_name=ReadToken();// 读取变量名
						while( tp_name.vaule!=";")//;号
						{
							if (tp_name.key=="ID")
							{
								//数组加入表
								// void* struct_void=globaltable[globaltablenum].typePtr;
								structTable* t_void=tps;
								structfiledTable* p = t_void->next;
								if ( t_void->lenth==0 )
								{
									t_void->next=new structfiledTable;
									t_void->next->name="structfiled";
									t_void->next->varname=tp_name.vaule;
									if ( basetype.vaule=="integer" )
									{
										t_void->next->len=(high-low+1)*4;
										t_void->next->basekind=5;
										t_void->len+=(high-low+1)*4;
										lens+=(high-low+1)*4;
										arrayTable* aT=new arrayTable;
										aT->name="integer";
										aT->len=(high-low+1)*4;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										t_void->next->typePtr=(void *)aT;
									}
									else if ( basetype.vaule=="char" )
									{
										t_void->next->len=(high-low+1)*1;
										t_void->next->basekind=5;
										t_void->len+=(high-low+1)*1;
										lens+=(high-low+1)*1;
										arrayTable* aT=new arrayTable;
										aT->name="char";
										aT->len=(high-low+1)*1;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										t_void->next->typePtr=(void *)aT;
									}
									t_void->next->next=NULL;
								}
								else
								{
									while(p->next!=NULL)
									{
										p=p->next;
									}
									p->next=new structfiledTable;
									p->next->name="structfiled";
									p->next->varname=tp_name.vaule;
									if ( basetype.vaule=="integer" )
									{
										p->next->len=(high-low+1)*4;
										p->next->basekind=5;
										t_void->len+=(high-low+1)*4;
										lens+=(high-low+1)*4;
										arrayTable* aT=new arrayTable;
										aT->name="integer";
										aT->len=(high-low+1)*4;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										p->next->typePtr=(void *)aT;
									}
									else if ( basetype.vaule=="char" )
									{
										p->next->len=(high-low+1)*1;
										p->next->basekind=5;
										t_void->len+=(high-low+1)*1;
										lens+=(high-low+1)*1;
										arrayTable* aT=new arrayTable;
										aT->name="char";
										aT->len=(high-low+1)*1;
										aT->basekind=1;
										aT->low=low;
										aT->high=high;
										p->next->typePtr=(void *)aT;
									}
									p->next->next=NULL;
								}
								t_void->lenth++;
							}
							NextToken();
							tp_name=ReadToken();// 读取下一个变量名
						}
					}
					else
					{
						ERROR=0;
						printf("KEY: %s ,VAULE: %s\n", left.key.c_str(),left.vaule.c_str() );
						// error();
						printf("SomeWrong lines:%s!!!\n",lines/2);
					}
					NextToken();
					left=ReadToken();
				}

				NextToken();
				token realvarname=ReadToken();
				while(realvarname.vaule!=";")
				{
					if (realvarname.key=="ID")
					{
						for (int i = typetablenum-1 ; i >= 0 ; --i)
						{
							if (typetable[i].isend==1)
							{
								i=typetable[i].headenddestionation;
								if (i==-1)
								{
									break;
								}
							}
							if (typetable[i].typenames==realvarname.vaule )
							{
								ERROR=0;
								printf("AAAAERROR Repeatily Define Type ! lines:%d  type %s already define before!!!\n",lines/2, realvarname.vaule.c_str() );
								// return 0;
							}
						}

						for (int i = globaltablenum-1; i>=0  ; --i)
						{
							if (globaltable[i].isend==1)
							{
								i=globaltable[i].headenddestionation;
								if (i==-1)
								{
									break;
								}
							}
							if (globaltable[i].varname==realvarname.vaule && globaltable[i].level==level )
							{
								ERROR=0;
								printf("ERROR Repeatily Define Var ! lines:%d  Var %s already define before!!!\n",lines/2, realvarname.vaule.c_str() );
								// return 0;
							}
						}
						globaltable[globaltablenum].name="record";
						globaltable[globaltablenum].dic=0;//变量访问方式 0 直接变量 1间接变量
						globaltable[globaltablenum].level=level;
						globaltable[globaltablenum].offset=var_off;
						globaltable[globaltablenum].isend=0;
						globaltable[globaltablenum].headenddestionation=tempglobalnum;
						globaltable[globaltablenum].typePtr=(void *)tps;
						globaltable[globaltablenum].varname=realvarname.vaule;
						globaltablenum++;
						var_off+=lens;
					}
					NextToken();//下一个变量
					realvarname=ReadToken();//定义的变量名
				}

			}
			NextToken();//略去末尾的;
		}
	}

	int ParameVarDomain(parameterTable* parameT ,int V)
	{
		int num=0;
		token type = ReadToken();//类型名
		if (type.vaule=="integer" || type.vaule=="char")
		{
			NextToken();//下一个
			token second_token=ReadToken();//读取定义的变量名
			while(second_token.vaule!=";" && second_token.vaule!=")")
			{
				if (second_token.key=="ID")
				{
					//检查名称合法性
					//1.type表检查
					for (int i = typetablenum-1 ; i >=0 ; --i)
					{
						if (typetable[i].isend==1)
						{
							i=typetable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (typetable[i].typenames==second_token.vaule)
						{
							ERROR=0;
							printf("ERROR ,lines:%d  ,ID can't do var, %s is a type not a ID\n",lines/2,second_token.vaule.c_str() );
						}
					}
					//2.global表检查
					for (int i = globaltablenum-1; i >=0 ; --i)
					{
						if (globaltable[i].isend==1)
						{
							i=globaltable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (globaltable[i].varname==second_token.vaule && globaltable[i].level==level )
						{
							ERROR=0;
							printf("ERROR Lines:%d Var Repeatly Define, %s has Defined Before \n",lines/2,second_token.vaule.c_str() );
						}
					}
					//加入参数列表
					parameterTable* parT = new parameterTable;
					parT->varname=second_token.vaule;
					globaltable[globaltablenum].varname=second_token.vaule;
					globaltable[globaltablenum].dic=1;
					globaltable[globaltablenum].level=level;
					globaltable[globaltablenum].offset=var_off;
					globaltable[globaltablenum].isend=0;
					globaltable[globaltablenum].headenddestionation=tempglobalnum;
					if (type.vaule=="integer")
					{
						parT->len=4;
						parT->basekind=1;
						integerTable* iT=new integerTable;
						iT->name="integer";
						iT->len=4;
						iT->vaule=NULL;
						parT->typePtr=iT;
						parT->next=NULL;
						globaltable[globaltablenum].name="integer";
						globaltable[globaltablenum].typePtr=iT;
						var_off+=4;
					}
					else if (type.vaule=="char")
					{
						parT->len=1;
						parT->basekind=2;
						charTable* cT=new charTable;
						cT->name="char";
						cT->len=1;
						cT->vaule=NULL;
						parT->typePtr=cT;
						parT->next=NULL;
						globaltable[globaltablenum].name="char";
						globaltable[globaltablenum].typePtr=cT;
						var_off+=1;
					}
					parameT->next=parT;
					parameT=parameT->next;//连接下一个
					//加入global表
					globaltablenum++;
					num++;
				}
				NextToken();
				second_token=ReadToken();
			}
			if (second_token.vaule==";")
			{
				BackToken();
				if (ReadToken().vaule==")")
				{
					// printf("aaaaaaaaaaaaaaaaaaa\n");
					BackToken();
				}
				else
				{
					NextToken();
				}
			}
			else if (second_token.vaule==")")
			{
				BackToken();
			}
		}
		else if (type.vaule=="array"  )
		{
			//数组相关操作
			NextToken(2);//略过[
			int low=atoi(ReadToken().vaule.c_str());
			NextToken(2);//略过 ..
			int high=atoi(ReadToken().vaule.c_str());
			NextToken(3);//略过] of
			token basetype=ReadToken();
			if (low > high)
			{
				ERROR=0;
				printf("ERROR Array Low > High ! lines:%d !!!\n",lines/2 );

			}
			NextToken();
			token second_token=ReadToken();//var 名
			while(second_token.vaule!=";"&& second_token.vaule!=")")
			{
				if (second_token.key=="ID")
				{
					//1.检查变量名称合法性
					//1.type表检查
					for (int i = typetablenum-1 ; i >=0 ; --i)
					{
						if (typetable[i].isend==1)
						{
							i=typetable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (typetable[i].typenames==second_token.vaule)
						{
							ERROR=0;
							printf("ERROR ,lines:%d  ,ID can't do var, %s is a type not a ID\n",lines/2,second_token.vaule.c_str() );
						}
					}
					//2.global表检查
					for (int i = globaltablenum-1; i >=0 ; --i)
					{
						if (globaltable[i].isend==1)
						{
							i=globaltable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (globaltable[i].varname==second_token.vaule && globaltable[i].level==level )
						{
							ERROR=0;
							printf("ERROR Lines:%d Var Repeatly Define, %s has Defined Before \n",lines/2,second_token.vaule.c_str() );
						}
					}
					//加入parameterTable表
					parameterTable* parT=new parameterTable;
					parT->varname=second_token.vaule;
					parT->basekind=5;
					parT->next=NULL;
					//加入globalTable表
					globaltable[globaltablenum].varname=second_token.vaule;
					globaltable[globaltablenum].name="array";

					globaltable[globaltablenum].dic=1;
					globaltable[globaltablenum].level=level;
					globaltable[globaltablenum].offset=var_off;
					globaltable[globaltablenum].isend=0;
					globaltable[globaltablenum].headenddestionation=tempglobalnum;

					if (basetype.vaule=="integer")
					{
						arrayTable* aT=new arrayTable;
						aT->name="array";
						aT->len=(high-low+1)*4;
						aT->basekind=1;
						aT->low=low;
						aT->high=high;
						globaltable[globaltablenum].typePtr=aT;
						var_off+=(high-low+1)*4;
						parT->len=(high-low+1)*4;
						parT->typePtr=aT;
					}
					else if (basetype.vaule=="char")
					{
						arrayTable* aT=new arrayTable;
						aT->name="array";
						aT->len=(high-low+1)*1;
						aT->basekind=2;
						aT->low=low;
						aT->high=high;
						globaltable[globaltablenum].typePtr=aT;
						var_off+=(high-low+1)*1;
						parT->len=(high-low+1)*1;
						parT->typePtr=aT;
					}
					parameT->next=parT;
					parameT=parameT->next;
					globaltablenum++;
					num++;
				}
				NextToken();
				second_token=ReadToken();
			}
			if (second_token.vaule==";")
			{
				BackToken();
				if (ReadToken().vaule==")")
				{
					// printf("ddddddddddddddd\n");
					BackToken();
				}
				else
				{
					NextToken();
				}
			}
			else if (second_token.vaule==")")
			{
				BackToken();
			}
		}
		else if (type.vaule=="record" )
		{
			NextToken();
			token sub_type_token=ReadToken();
			//初始化struct表
			structTable* strT=new structTable;

			strT->name="struct";
			strT->len=0;
			strT->lenth=0;
			structfiledTable* strfiledT=new structfiledTable;
			strfiledT->next=NULL;
			strT->next=strfiledT;

			//建立基本结构体表
			while(sub_type_token.vaule!="end")
			{
				if (sub_type_token.vaule=="array")
				{
					//数组相关操作
					NextToken(2);//略过[
					int low=atoi(ReadToken().vaule.c_str());
					NextToken(2);//略过 ..
					int high=atoi(ReadToken().vaule.c_str());
					NextToken(3);//略过] of
					token basetype=ReadToken();
					if (low > high)
					{
						ERROR=0;
						printf("ERROR Array Low > High ! lines:%d !!!\n",lines/2 );
					}
					NextToken();
					token sub_var_token=ReadToken();
					while( sub_var_token.vaule!=";" )
					{
						if (sub_var_token.key=="ID")
						{
							structfiledTable* sub_strfiledT =new structfiledTable;
							sub_strfiledT->name="structfiled";
							sub_strfiledT->varname=sub_var_token.vaule;
							sub_strfiledT->len=(basetype.vaule=="integer")? (high-low+1)*4:(high-low+1)*1;
							sub_strfiledT->basekind=5;
							arrayTable* aT=new arrayTable;
							aT->name="array";
							aT->len=(basetype.vaule=="integer")? (high-low+1)*4:(high-low+1)*1;
							aT->basekind=(basetype.vaule=="integer")?1:2;
							aT->low=low;
							aT->high=high;
							sub_strfiledT->typePtr=aT;
							sub_strfiledT->next=NULL;
							strfiledT->next=sub_strfiledT;
							strfiledT=strfiledT->next;
							strT->len+=aT->len;
							strT->lenth++;
						}
						NextToken();
						sub_var_token=ReadToken();
					}
				}
				else if (sub_type_token.vaule=="integer")
				{
					NextToken();
					token sub_var_token=ReadToken();
					while( sub_var_token.vaule!=";" )
					{
						if (sub_var_token.key=="ID")
						{
							structfiledTable* sub_strfiledT =new structfiledTable;
							sub_strfiledT->name="structfiled";
							sub_strfiledT->varname=sub_var_token.vaule;
							sub_strfiledT->len=4;
							sub_strfiledT->basekind=1;
							integerTable* iT=new integerTable;
							iT->name="integer";
							iT->len=4;
							iT->vaule=NULL;
							sub_strfiledT->typePtr=iT;
							sub_strfiledT->next=NULL;
							strfiledT->next=sub_strfiledT;
							strfiledT=strfiledT->next;
							strT->len+=4;
							strT->lenth++;
						}

						NextToken();
						sub_var_token=ReadToken();
					}
				}
				else if (sub_type_token.vaule=="char")
				{
					NextToken();
					token sub_var_token=ReadToken();
					while( sub_var_token.vaule!=";" )
					{
						if (sub_var_token.key=="ID")
						{
							structfiledTable* sub_strfiledT =new structfiledTable;
							sub_strfiledT->name="structfiled";
							sub_strfiledT->varname=sub_var_token.vaule;
							sub_strfiledT->len=1;
							sub_strfiledT->basekind=2;
							charTable* iT=new charTable;
							iT->name="char";
							iT->len=1;
							iT->vaule=NULL;
							sub_strfiledT->typePtr=iT;
							sub_strfiledT->next=NULL;
							strfiledT->next=sub_strfiledT;
							strfiledT=strfiledT->next;
							strT->len+=1;
							strT->lenth++;
						}
						NextToken();
						sub_var_token=ReadToken();
					}
				}
				else
				{
					ERROR=0;
					printf("Some Wrong lines:%d!!!\n",lines/2);
				}
				NextToken();
				sub_type_token=ReadToken();
			}

			structfiledTable* temp_str=strT->next;
			strT->next=strT->next->next;
			delete temp_str;

			NextToken();
			token second_token=ReadToken();
			//继续读取定义变量名
			while(second_token.vaule!=";" && second_token.vaule!=")")
			{
				if (second_token.key=="ID")
				{
					//检查名称合法性
					//1.type表检查
					for (int i = typetablenum-1 ; i >=0 ; --i)
					{
						if (typetable[i].isend==1)
						{
							i=typetable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (typetable[i].typenames==second_token.vaule)
						{
							ERROR=0;
							printf("ERROR ,lines:%d  ,ID can't do var, %s is a type not a ID\n",lines/2,second_token.vaule.c_str() );
						}
					}
					//2.global表检查
					for (int i = globaltablenum-1; i >=0 ; --i)
					{
						if (globaltable[i].isend==1)
						{
							i=globaltable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (globaltable[i].varname==second_token.vaule && globaltable[i].level==level )
						{
							ERROR=0;
							printf("ERROR Lines:%d Var Repeatly Define, %s has Defined Before \n",lines/2,second_token.vaule.c_str() );
						}
					}
					//加入参数列表
					parameterTable* parT = new parameterTable;
					parT->varname=second_token.vaule;
					globaltable[globaltablenum].varname=second_token.vaule;
					globaltable[globaltablenum].dic=1;
					globaltable[globaltablenum].level=level;
					globaltable[globaltablenum].offset=var_off;
					globaltable[globaltablenum].isend=0;
					globaltable[globaltablenum].headenddestionation=tempglobalnum;
					parT->len=strT->len;
					parT->basekind=4;
					parT->typePtr=strT;
					parT->next=NULL;
					globaltable[globaltablenum].name="record";
					globaltable[globaltablenum].typePtr=strT;
					var_off+=strT->len;
					parameT->next=parT;
					parameT=parameT->next;//连接下一个
					//加入global表
					globaltablenum++;
					num++;
				}
				NextToken();
				second_token=ReadToken();
			}
			if (second_token.vaule==";")
			{
				BackToken();

				// printf("gggggggggggg\n");
				if (ReadToken().vaule==")")
				{
					BackToken();
				}
				else
				{
					NextToken();
				}
			}
			else if (second_token.vaule==")")
			{
				BackToken();
			}
		}
		else if (type.key  =="ID"     )
		{
			NextToken();
			token second_token=ReadToken();
			// printf("se1: %s\n",second_token.vaule.c_str() );
			while(second_token.vaule!=";"&& second_token.vaule!=")")
			{
				if (second_token.key=="ID")
				{
					//检查名称合法性
					//1.自定义类型是否存在
					int exists=0;
					int ll=0;
					for (int i = typetablenum-1; i >= 0; --i)
					{
						if (typetable[i].isend==1)
						{
							i=typetable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (typetable[i].typenames==type.vaule)
						{
							ll=i;
							exists=1;
						}
					}
					if (exists==0)
					{
						printf("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
						ERROR=0;
						printf("ERROR ,lines:%d  ,Unkonw type, type %s is unkonw \n",lines/2,type.vaule.c_str() );
					}
					//2.type表检查
					for (int i = typetablenum-1 ; i >=0 ; --i)
					{
						if (typetable[i].isend==1)
						{
							i=typetable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (typetable[i].typenames==second_token.vaule)
						{
							ERROR=0;
							printf("ERROR ,lines:%d  ,ID can't do var, %s is a type not a ID\n",lines/2,second_token.vaule.c_str() );
						}
					}
					//3.global表检查
					for (int i = globaltablenum-1; i >=0 ; --i)
					{
						if (globaltable[i].isend==1)
						{
							i=globaltable[i].headenddestionation;
							if (i==-1)
							{
								break;
							}
						}
						if (globaltable[i].varname==second_token.vaule && globaltable[i].level==level )
						{
							ERROR=0;
							printf("ERROR Lines:%d Var Repeatly Define, %s has Defined Before \n",lines/2,second_token.vaule.c_str() );
						}
					}
					//加入参数列表
					parameterTable* parT = new parameterTable;
					parT->varname=second_token.vaule;
					parT->len=typetable[ll].len;
					parT->basekind=typetable[ll].basekind;
					parT->typePtr=typetable[ll].typePtr;
					parT->next=NULL;
					//加入global表

					globaltable[globaltablenum].name="type";
					globaltable[globaltablenum].typePtr=&typetable[ll];
					globaltable[globaltablenum].varname=second_token.vaule;
					globaltable[globaltablenum].dic=1;//V
					globaltable[globaltablenum].level=level;
					globaltable[globaltablenum].offset=var_off;
					globaltable[globaltablenum].isend=0;
					globaltable[globaltablenum].headenddestionation=tempglobalnum;
					var_off+=parT->len;
					parameT->next=parT;
					parameT=parameT->next;//连接下一个

					globaltablenum++;
					num++;
				}
				NextToken();
				second_token=ReadToken();
				if (second_token.vaule==")")
				{
					BackToken();
					break;
				}
			}
			if (second_token.vaule==";")
			{
				BackToken();
				// printf("desfd\n");
				if (ReadToken().vaule==")")
				{
					BackToken();
				}
				else
				{
					NextToken();
				}
			}
			else if (second_token.vaule==")")
			{
				BackToken();
			}
		}
		else
		{
			ERROR=0;
			printf("lines: %d ,Unknow ERROR ON ParameVarDomain!!!\n",lines/2);
			return 0;
		}
		return num;
	}

	//参数列表解析
	int parameterAnalysis(parameterTable* parameT)
	{
		NextToken(2);//略过(
		token first_token=ReadToken();
		int num=0;
		while(first_token.vaule!=")")
		{
			if (first_token.vaule=="var")
			{
				NextToken();//略过var
				num+=ParameVarDomain(parameT,1);
				// printf("lines:%d  KEY: %s VALUE: %s \n",lines/2,ReadToken().key.c_str() ,ReadToken().vaule.c_str());
				while(parameT->next!=NULL)
				{
					parameT=parameT->next;
				}
			}
			else if (first_token.key=="ID" || first_token.vaule=="char" || first_token.vaule=="integer" || first_token.vaule=="array" || first_token.vaule=="record")
			{
				num+=ParameVarDomain(parameT,0);

				// printf("lines:%d  KEY: %s VALUE: %s \n",lines/2,ReadToken().key.c_str() ,ReadToken().vaule.c_str());
				while(parameT->next!=NULL)
				{
					parameT=parameT->next;
				}
			}
			NextToken();
			first_token=ReadToken();//下一个token
		}
		return num;
	}

	//函数过程定义域
	void FunDomain( void )
	{
		NextToken();//略过procedure
		token funname=ReadToken();
		if(checkID(funname)!=-1)
		{
			ERROR=0;
			printf("ERROR lines:%d ,repeadily define procedure %s\n",lines/2,funname.vaule.c_str() );
		}
		globaltable[globaltablenum].varname=funname.vaule;
		globaltable[globaltablenum].name="procedure";
		globaltable[globaltablenum].dic=0;
		globaltable[globaltablenum].level=level-1;
		globaltable[globaltablenum].offset=var_off;//这个数据是错的
		globaltable[globaltablenum].isend=0;
		globaltable[globaltablenum].headenddestionation=tempglobalnum;
		procedureTable* pT=new procedureTable;
		globaltable[globaltablenum++].typePtr=pT;
		pT->name="procedure";
		pT->len=0;
		pT->level=level-1;
		parameterTable* parameT=new parameterTable;
		parameT->next=NULL;
		//将函数信息加入global表
		pT->num=parameterAnalysis (parameT);//参数列表分析
		parameterTable* temp_ppp=parameT;
		pT->next=parameT->next;
		delete temp_ppp;

		NextToken(2);//当前为)???
		token tk=ReadToken(  );
		while(tk.key!="EOF" && tk.vaule!="EOF")
		{
			int temp=filter(tk);
			if (temp==4)//type
			{
				TypeDomain();//类型区域建表

				// printf("Type Analysis commplete!!!\n");

				// for (int i = 0; i < typetablenum; ++i)
				// {
				// 	if (typetable[i].isend==0)
				// 	{
				// 		printf("TypeName: %s I: %d \n",typetable[i].typenames.c_str(),i);
				// 	}
				// }
				// exit(0);
			}
			else if (temp==5)//var，注意偏移
			{
				VarDomain();//变量区域建表
				// printf("Var Analysis commplete!!!\n");
				// for (int i = 0; i < globaltablenum; ++i)
				// {
				// 	if (globaltable[i].isend==0)
				// 	{
				// 		printf("VarName: %s I: %d \n",globaltable[i].varname.c_str(),i);
				// 	}
				// }
				// exit(0);
			}
			else if (temp==6)//procedure 函数过程
			{
				level++;//深度加1
				int store_off=var_off;
				var_off=0;
				int temp_num_type=temptypenum;
				int temp_num_global=tempglobalnum;
				tempglobalnum=globaltablenum;
				temptypenum=typetablenum-1;
				FunDomain();//过程区域建表
				typetable[typetablenum].isend=1;
				typetable[typetablenum++].headenddestionation=temptypenum;

				globaltable[globaltablenum].isend=1;
				globaltable[globaltablenum++].headenddestionation=tempglobalnum;
				temptypenum=temp_num_type;
				tempglobalnum=temp_num_global;
				var_off=store_off;//偏移恢复
				level--;//深度减1
			}
			else if(temp==7)//begin 函数体
			{
				BodyDomain();//函数体语义检查
			}
			else if(temp==8)//end 函数体
			{
				break;
			}
			else
			{
				NextToken();
			}
			tk=ReadToken();
		}
	}

	int checkID(token id)
	{
		int index=-1;
		for (int i = globaltablenum-1; i >=0 ; --i)
		{
			if (globaltable[i].isend==1)
			{
				i=globaltable[i].headenddestionation;
				if (i==-1)
				{
					break;
				}
			}
			if (globaltable[i].varname==id.vaule)
			{
				index=i;
			}
		}
		return index;
	}

	string ReadOne(token first)
	{
		if (first.key=="ID")
		{
			int l=checkID(first);
			if (l==-1)
			{
				ERROR=0;
				printf("ERROR lines: %d , Undefined var %s \n",lines/2,first.vaule.c_str() );
				exit(0);
			}

			string sename=globaltable[l].name;
			// printf("senam:    %s\n",sename.c_str() );
			if ( sename=="type" )
			{
				typeTable* tT=( typeTable* )globaltable[l].typePtr;
				int options=tT->basekind;
				// printf("pop:%d\n",options );
				if (options==1)
				{
					return "integer";
				}
				else if (options==2)
				{
					return "char";
				}
				else if (options==4)//struct
				{
					structTable* strT=(structTable*)tT->typePtr;
					NextToken();//sself,.
					//结构体没有.报错
					if (ReadToken().vaule!=".")
					{
						ERROR=0;
						printf("ERROR lines:%d array doesn't have . but is %s \n",lines/2,ReadToken().vaule.c_str() );
						exit(0);
					}
					NextToken();
					token sub=ReadToken();
					structfiledTable* sub_strfiledT=strT->next;
					int find=0;
					int isarray=0;
					while(sub_strfiledT!=NULL)
					{
						if (sub_strfiledT->varname==sub.vaule)
						{
							find=1;
							if (sub_strfiledT->basekind==5)
							{
								isarray=1;
							}
							break;
						}
						sub_strfiledT=sub_strfiledT->next;
					}
					if (find==0)
					{
						ERROR=0;
						printf("ERROR lines:%d ,record sub-domain %s dosen't exists\n",lines/2,sub.vaule.c_str() );
						exit(0);
					}
					if (isarray==1)
					{
						arrayTable* aT=(arrayTable*)sub_strfiledT->typePtr;
						string baseK=(aT->basekind==1)?"integer":"char";
						NextToken(2);//self,[
						string re=Exp();
						if (re!="integer")
						{
							ERROR=0;
							printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
							exit(0);
						}
						return baseK;//右边的表达式
					}
					else
					{
						string baseK=(sub_strfiledT->basekind==1)?"integer":"char";
						return baseK;
					}
				}
				else if (options==5)//array
				{
					arrayTable* aT=(arrayTable*)tT->typePtr;
					string baseK=(aT->basekind==1)?"integer":"char";
					NextToken();//self,[
					//数组没有[]报错
					if (ReadToken().vaule!="[")
					{
						ERROR=0;
						printf("ERROR lines:%d array doesn't have [ but is %s \n",lines/2,ReadToken().vaule.c_str() );
						exit(0);
					}
					NextToken();
					string re=Exp();

					if (re!="integer")
					{
						ERROR=0;
						printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
						exit(0);
					}
					return baseK;
				}
				else
				{
					ERROR=0;
					printf("ERROR lines: %d \n",lines/2 );
					exit(0);
				}
			}
			else if ( sename=="array" )
			{
				arrayTable* aT=(arrayTable*)globaltable[l].typePtr;
				string baseK=(aT->basekind==1)?"integer":"char";
				NextToken();//self,[
				//数组没有[]报错
				if (ReadToken().vaule!="[")
				{
					ERROR=0;
					printf("ERROR lines:%d array doesn't have [ but is %s \n",lines/2,ReadToken().vaule.c_str() );
					exit(0);
				}
				NextToken();
				string re=Exp();
				if (re!="integer")
				{
					ERROR=0;
					printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
					exit(0);
				}
				return baseK;
			}
			else if ( sename=="record" )
			{
				NextToken();//.
				//结构体没有.报错
				if (ReadToken().vaule!=".")
				{
					ERROR=0;
					printf("ERROR lines:%d array doesn't have . but is %s \n",lines/2,ReadToken().vaule.c_str() );
					exit(0);
				}
				NextToken();//.
				token sub=ReadToken();
				structTable* strT=(structTable*)globaltable[l].typePtr;

				structfiledTable* sub_strfiledT=strT->next;
				int find=0;
				int isarray=0;
				while(sub_strfiledT!=NULL)
				{
					if (sub_strfiledT->varname==sub.vaule)
					{
						find=1;
						if (sub_strfiledT->basekind==5)
						{
							isarray=1;
						}
						break;
					}
					sub_strfiledT=sub_strfiledT->next;
				}
				if (find==0)
				{
					ERROR=0;
					printf("ERROR lines:%d ,record sub-domain %s dosen't exists\n",lines/2,sub.vaule.c_str() );
					exit(0);
				}
				if (isarray==1)
				{
					arrayTable* aT=(arrayTable*)sub_strfiledT->typePtr;
					string baseK=(aT->basekind==1)?"integer":"char";
					NextToken();//self,[
					//数组没有[]报错
					if (ReadToken().vaule!="[")
					{
						ERROR=0;
						printf("ERROR lines:%d array doesn't have [ but is %s \n",lines/2,ReadToken().vaule.c_str() );
						exit(0);
					}
					NextToken();
					string re=Exp();
					if (re!="integer")
					{
						ERROR=0;
						printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
						exit(0);
					}
					return baseK;
				}
				else
				{
					string baseK=(sub_strfiledT->basekind==1)?"integer":"char";
					return baseK;
				}
			}
			else if (sename=="integer")
			{
				return "integer";
			}
			else if (sename=="char")
			{
				return "char";
			}
		}
		else if (first.key=="INTC")
		{
			return "integer";
		}
		return "pass";
	}


	string Exp()//表达式分析
	{
		//读取到不该读的东西才停止
		token temp=ReadToken();
		int flag=0;
		string end_string;
		int left_kuohao=0;
		while( temp.vaule!="<" && temp.vaule!="=" &&
			   temp.vaule!="then" && temp.vaule!="do" &&
			   temp.vaule!=";" && temp.vaule!="end" &&
			   temp.vaule!="]" && temp.vaule!="fi" &&
			   temp.vaule!="else" && temp.vaule!="endwh" &&
			   temp.vaule!="while" && temp.vaule!="read" &&
			   temp.vaule!="write" && temp.vaule!="return" &&
			   temp.vaule!="," && temp.vaule!="EOF"
			 )
		{
			if (temp.vaule=="(")
			{
				left_kuohao++;

			}
			else if (temp.vaule==")")
			{
				left_kuohao--;
				if ( left_kuohao<0)
				{
					BackToken();
					break;
				}
			}
			// printf("#####KEY: %s ,VAULE : %s \n",temp.key.c_str(),temp.vaule.c_str() );
			string re=ReadOne(temp);
			// printf(":::::%s\n",re.c_str() );
			// printf("key:%s,value%s\n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
			if (flag==0 && re!="pass")
			{
				end_string=re;
				flag=1;
			}
			else if (re!=end_string && re!="pass")
			{
				ERROR=0;
				printf("ERROR lines:%d , operate different types need %s but give is %s \n",lines/2, re.c_str() , end_string.c_str() );
			}
			NextToken();
			temp=ReadToken();
			// printf("$$$$KEY: %s ,VAULE : %s \n",temp.key.c_str(),temp.vaule.c_str() );
		}

		if (flag==0)
		{
			ERROR=0;
			printf("EXP() Lines:%d  Unknow error\n",lines/2 );
			return "pass";
		}
		return end_string;
	}

	void ifExp(void)
	{
		NextToken();//if
		string re1=Exp();
		token op=ReadToken();
		NextToken();//op
		string re2=Exp();
		NextToken();//then
		while(ReadToken().vaule!="else")
		{
			// printf("KEY:%s,VAULE:%s\n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
			BodyExp(ReadToken());
			if (ReadToken().vaule!="if" && ReadToken().vaule!="while" &&
				ReadToken().vaule!="read" && ReadToken().vaule!="write" &&
				ReadToken().vaule!="return"  && ReadToken().key!="ID" &&
				ReadToken().key!="INTC" && ReadToken().vaule!="else")
			{
				NextToken();
			}
		}
		// printf("zzzKEY:%s,VAULE:%s\n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
		NextToken();//else
		while(ReadToken().vaule!="fi")
		{
			// printf("KEY:%s,VAULE:%s\n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
			BodyExp(ReadToken());
			if (ReadToken().vaule!="if" && ReadToken().vaule!="while" &&
				ReadToken().vaule!="read" && ReadToken().vaule!="write" &&
				ReadToken().vaule!="return"  && ReadToken().key!="ID" &&
				ReadToken().key!="INTC" && ReadToken().vaule!="fi")
			{
				NextToken();
			}
			// printf("xsxsKEY:%s,VAULE:%s\n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
		}
		NextToken();//fi
	}
	void whileExp(void)
	{
		NextToken();//while
		string re1=Exp();
		token op=ReadToken();
		NextToken();//op
		string re2=Exp();
		NextToken();//do
		while(ReadToken().vaule!="endwh")
		{
			// printf("KEY:%s,VAULE:%s\n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
			BodyExp(ReadToken());
			if (ReadToken().vaule!="if" && ReadToken().vaule!="while" &&
				ReadToken().vaule!="read" && ReadToken().vaule!="write" &&
				ReadToken().vaule!="return"  && ReadToken().key!="ID" &&
				ReadToken().key!="INTC" && ReadToken().vaule!="endwh")
			{
				NextToken();
			}
			// printf("QQQQKEY:%s,VAULE:%s\n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
		}
		NextToken();//endwh
	}

	void readExp(void)
	{
		NextToken();//read
		NextToken();//(
		token temp=ReadToken();//
		//检查
		if (checkID(temp)==-1)
		{
			ERROR=0;
			printf("ERROR lines: %d , Undefined var %s \n",lines/2,temp.vaule.c_str() );
		}
		NextToken();//)
	}

	void writeExp(void)
	{
		NextToken();//write
		NextToken();//(
		string re=Exp();//特殊情况）被过滤了
		NextToken();//)
	}

	void assignExp(string left_need)
	{
		string re=Exp();
		if (re!=left_need)
		{
			ERROR=0;
			printf("ERROR lines: %d , assignment statement need %s but give %s \n",lines/2,left_need.c_str(),re.c_str() );
			exit(0);
		}
	}

	void returnExp(void)
	{
		NextToken();//return
		NextToken();//(
		string re=Exp();//特殊情况）被过滤了
		NextToken();//)
	}


	void callExp(token funcname,int index)
	{
		NextToken(2);//self,(
		procedureTable* proT=(procedureTable*)globaltable[index].typePtr;
		int num=proT->num;
		token temp=ReadToken();
		// structfiledTable* parameT=proT->next;//暂时不用
		while(temp.vaule!=")")
		{
			num--;
			string re=Exp();
			if (ReadToken().vaule==")")
			{
				break;
			}
			NextToken();
			temp=ReadToken();

		}
		if (num!=0)
		{
			ERROR=0;
			printf("ERROR lines: %d, function's parameter doesn't match totally! \n",lines/2 );
		}
		NextToken();
		// printf("KEY:%s ,VAULE:%s\n",ReadToken().vaule.c_str(),ReadToken().vaule.c_str() );
	}

	void identifyEXP(token id)
	{
		// printf("lines: %d KEY:%s vaule:%s\n",lines/2,id.key.c_str(),id.vaule.c_str() );
		int l=checkID(id);
		if (l==-1)
		{
			ERROR=0;
			printf("ERROR lines: %d , Undefined var %s \n",lines/2,id.vaule.c_str() );
			exit(0);
		}
		string sename=globaltable[l].name;
		if( sename == "procedure" )//是函数
		{
			callExp(id,l);
		}
		else if ( sename=="type" )
		{
			typeTable* tT=( typeTable* )globaltable[l].typePtr;
			int options=tT->basekind;
			if (options==1)
			{
				NextToken(2);//self,:=
				assignExp("integer");
			}
			else if (options==2)
			{
				NextToken(2);//self,:=
				assignExp("char");
			}
			else if (options==4)//struct
			{
				structTable* strT=(structTable*)tT->typePtr;
				NextToken();//sself,.
				//结构体没有.报错
				if (ReadToken().vaule!=".")
				{
					ERROR=0;
					printf("ERROR lines:%d array doesn't have . but is %s \n",lines/2,ReadToken().vaule.c_str() );
					exit(0);
				}
				NextToken();
				token sub=ReadToken();
				structfiledTable* sub_strfiledT=strT->next;
				int find=0;
				int isarray=0;
				while(sub_strfiledT!=NULL)
				{
					if (sub_strfiledT->varname==sub.vaule)
					{
						find=1;
						if (sub_strfiledT->basekind==5)
						{
							isarray=1;
						}
						break;
					}
					sub_strfiledT=sub_strfiledT->next;
				}
				if (find==0)
				{
					ERROR=0;
					printf("ERROR lines:%d ,record sub-domain %s dosen't exists\n",lines/2,sub.vaule.c_str() );
					exit(0);
				}
				if (isarray==1)
				{
					arrayTable* aT=(arrayTable*)sub_strfiledT->typePtr;
					string baseK=(aT->basekind==1)?"integer":"char";
					NextToken(2);//self,[
					string re=Exp();
					if (re!="integer")
					{
						ERROR=0;
						printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
						exit(0);
					}
					NextToken();//]
					NextToken();//:=
					assignExp(baseK);//右边的表达式
				}
				else
				{
					string baseK=(sub_strfiledT->basekind==1)?"integer":"char";
					NextToken(2);//self,:=
					assignExp(baseK);
				}
			}
			else if (options==5)//array
			{
				arrayTable* aT=(arrayTable*)tT->typePtr;
				string baseK=(aT->basekind==1)?"integer":"char";
				NextToken();//self,[
				//数组没有[]报错
				if (ReadToken().vaule!="[")
				{
					ERROR=0;
					printf("ERROR lines:%d array doesn't have [ but is %s \n",lines/2,ReadToken().vaule.c_str() );
					exit(0);
				}
				NextToken();
				string re=Exp();
				if (re!="integer")
				{
					ERROR=0;
					printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
					exit(0);
				}
				NextToken(2);//],:=
				assignExp(baseK);
			}
			else
			{
				ERROR=0;
				printf("ERROR lines: %d \n",lines/2 );
				exit(0);
			}
		}
		else if ( sename=="array" )
		{
			arrayTable* aT=(arrayTable*)globaltable[l].typePtr;
			string baseK=(aT->basekind==1)?"integer":"char";
			NextToken();//self,[
			//数组没有[]报错
			if (ReadToken().vaule!="[")
			{
				ERROR=0;
				printf("ERROR lines:%d array doesn't have [ but is %s \n",lines/2,ReadToken().vaule.c_str() );
				exit(0);
			}
			NextToken();
			string re=Exp();
			if (re!="integer")
			{
				ERROR=0;
				printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
				exit(0);
			}
			NextToken();//]
			assignExp(baseK);//右边的表达式
		}
		else if ( sename=="record" )
		{
			NextToken();//.
			//结构体没有.报错
			if (ReadToken().vaule!=".")
			{
				ERROR=0;
				printf("ERROR lines:%d array doesn't have . but is %s \n",lines/2,ReadToken().vaule.c_str() );
				exit(0);
			}
			NextToken();//.
			token sub=ReadToken();
			structTable* strT=(structTable*)globaltable[l].typePtr;

			structfiledTable* sub_strfiledT=strT->next;
			int find=0;
			int isarray=0;
			while(sub_strfiledT!=NULL)
			{
				if (sub_strfiledT->varname==sub.vaule)
				{
					find=1;
					if (sub_strfiledT->basekind==5)
					{
						isarray=1;
					}
					break;
				}
				sub_strfiledT=sub_strfiledT->next;
			}
			if (find==0)
			{
				ERROR=0;
				printf("ERROR lines:%d ,record sub-domain %s dosen't exists\n",lines/2,sub.vaule.c_str() );
				exit(0);
			}
			if (isarray==1)
			{
				arrayTable* aT=(arrayTable*)sub_strfiledT->typePtr;
				string baseK=(aT->basekind==1)?"integer":"char";
				NextToken();//self,[
				//数组没有[]报错
				if (ReadToken().vaule!="[")
				{
					ERROR=0;
					printf("ERROR lines:%d array doesn't have [ but is %s \n",lines/2,ReadToken().vaule.c_str() );
					exit(0);
				}
				NextToken();
				string re=Exp();
				if (re!="integer")
				{
					ERROR=0;
					printf("ERROR lines: %d array index need integer but is %s \n",lines/2,re.c_str() );
					exit(0);
				}
				NextToken();//]
				NextToken();//:=
				assignExp(baseK);//右边的表达式
			}
			else
			{
				string baseK=(sub_strfiledT->basekind==1)?"integer":"char";
				NextToken(2);//self,:=
				assignExp(baseK);
			}
		}
		else if (sename=="integer")
		{
			NextToken(2);//self,:=
			assignExp("integer");
		}
		else if (sename=="char")
		{
			NextToken(2);//self,:=
			assignExp("char");
		}
	}

	void BodyExp(token temp)//可能需要死循环
	{
		// 1.if语句判断
		if (temp.vaule=="if")
		{
			ifExp();
			// printf("IF Complete\n");
		}
		// 2.while语句
		else if (temp.vaule=="while")
		{
			whileExp();
			// printf("lines %d while Complete\n",lines/2);
		}
		// 3.read语句
		else if (temp.vaule=="read")
		{
			readExp();
			// printf(" lines %d read Complete\n",lines/2);
		}
		// 4.write语句
		else if (temp.vaule=="write")
		{
			writeExp();

			// printf(" lines %d  write Complete\n",lines/2);
		}
		// 6.返回语句
		else if (temp.vaule=="return")
		{
			returnExp();
		}
		// 5.赋值语句
		else if (temp.key=="ID")
		{
			identifyEXP(temp);
			// printf("dd\n");
			// printf("KEY:%s  VAULE:%s \n",ReadToken().key.c_str(),ReadToken().vaule.c_str() );
		}
		else if (temp.vaule==";")
		{
			//pass
		}
		else if (temp.key=="INTC")
		{
			ERROR=0;
			printf("ERROR BodyEXP lines : %d !!!, INTC can't be assigned \n",lines/2);
			exit(0);
		}
		else
		{
			ERROR=0;
			// printf("KEY:%s,VALUE:%s\n",temp.key.c_str(),temp.vaule.c_str() );
			printf("ERROR BodyEXP Unknow ERROR lines : %d !!!\n",lines/2);
			// exit(0);
		}
	}
	//程序体
	void BodyDomain(void)
	{
		NextToken();
		token temp=ReadToken();
		while(temp.vaule!="end" && temp.vaule!="EOF")
		{
			BodyExp(temp);
			if (ReadToken().vaule=="end")
			{
				break;
			}

			NextToken();
			temp=ReadToken();
		}
		// exit(0);
	}
	//过滤器 ID 1,RE 相对应编号,换行 2 , 没用的 3
	int filter(token temp)
	{
		if (temp.key=="ID")
		{
			return 1;
		}
		else if (temp.key=="SCHS" && temp.vaule=="\\n")
		{
			return 2;
		}
		else if (temp.key=="RE" && temp.vaule=="type")
		{
			return 4;
		}
		else if (temp.key=="RE" && temp.vaule=="var")
		{
			return 5;
		}
		else if (temp.key=="RE" && temp.vaule=="procedure")
		{
			return 6;
		}
		else if (temp.key=="RE" && temp.vaule=="begin")
		{
			return 7;
		}
		else if (temp.key=="RE" && temp.vaule=="end")
		{
			return 8;
		}
		else if (temp.key=="RE")//其他保留字
		{
			return 9;
		}
		else
		{
			return 3;
		}
	}
	//分析主函数
	void Analysis(void)
	{
		token tk=ReadToken();
		while(tk.key!="EOF" && tk.vaule!="EOF")
		{
			int temp=filter(tk);
			if (temp==4)//type
			{
				TypeDomain();//类型区域建表

				// printf("Type Analysis commplete!!!\n");

				// for (int i = 0; i < typetablenum; ++i)
				// {
				// 	if (typetable[i].isend==0)
				// 	{
				// 		printf("TypeName: %s I: %d \n",typetable[i].typenames.c_str(),i);
				// 	}
				// }
				// // exit(0);
				// printf("end lines:%d \n",lines/2);
			}
			else if (temp==5)//var，注意偏移
			{
				VarDomain();//变量区域建表
				// printf("Var Analysis commplete!!!\n");
				// for (int i = 0; i < globaltablenum; ++i)
				// {
				// 	if (globaltable[i].isend==0)
				// 	{
				// 		// printf("VarName: %s I: %d \n",globaltable[i].varname.c_str(),i);
				// 		if (globaltable[i].name=="record")
				// 		{
				// 			structTable* strT=(structTable*)globaltable[i].typePtr;
				// 			structfiledTable* strfiledT=strT->next;
				// 			while(strfiledT!=NULL)
				// 			{
				// 				printf("name:%s  varname:%s len: %d basekind:%d     \n",strfiledT->name.c_str(),strfiledT->varname.c_str(),strfiledT->len,strfiledT->basekind );
				// 				strfiledT=strfiledT->next;
				// 			}
				// 		}
				// 	}
				// }
				// // exit(0);
				// printf("end lines:%d \n",lines/2);
			}
			else if (temp==6)//procedure 函数过程
			{

				// printf("Warming!!!================================================\n");
				// printf("FUN Analysis start!!!\n" );
				// printf("Warming!!!================================================\n");
				level++;//深度加1
				int store_off=var_off;
				var_off=0;
				int temp_num_type=temptypenum;
				int temp_num_global=tempglobalnum;
				tempglobalnum=globaltablenum;
				temptypenum=typetablenum-1;
				FunDomain();//过程区域建表
				typetable[typetablenum].isend=1;
				typetable[typetablenum++].headenddestionation=temptypenum;

				globaltable[globaltablenum].isend=1;
				globaltable[globaltablenum++].headenddestionation=tempglobalnum;

				temptypenum=temp_num_type;
				tempglobalnum=temp_num_global;
				var_off=store_off;//偏移恢复
				level--;//深度减1





				// printf("FUN Analysis commplete!!!\n" );
				// printf("Warming!!!================================================\n");
				// printf("type table info list !!!\n" );
				// for (int i = 0; i < typetablenum; ++i)
				// {
				// 	//if (typetable[i].isend==0)
				// 	{
				// 		printf("I: %d Name: %s , TypeNames: %s , len:%d ,basekind:%d ,level:%d,isend:%d,headenddestionation:%d,\n",i,typetable[i].name.c_str(),typetable[i].typenames.c_str(),typetable[i].len,typetable[i].basekind,typetable[i].level,typetable[i].isend,typetable[i].headenddestionation   );
				// 	}
				// }
				// printf("Warming!!!================================================\n");
				// printf("Warming!!!================================================\n");
				// printf("global table info list !!!\n" );
				// for (int i = 0; i < globaltablenum; ++i)
				// {
				// 	//if (globaltable[i].isend==0)
				// 	{
				// 		printf("I: %d VarName: %s ,Name:%s ,dic:%d ,level:%d ,offset:%d ,isend:%d,headenddestionation:%d \n",i,globaltable[i].varname.c_str(),globaltable[i].name.c_str(),globaltable[i].dic,globaltable[i].level,globaltable[i].offset,globaltable[i].isend,globaltable[i].headenddestionation);
				// 	}
				// }
				// printf("Warming!!!================================================\n");
			}
			else if(temp==7)//begin 函数体
			{
				BodyDomain();//函数体语义检查
				// printf("analiysis complete!!!\n");
			}
			else//可以掠过的东西
			{
				NextToken();
			}
			tk=ReadToken();
		}
	}


private:
	string filepath="token.txt";//语义分析token文件路径
	int token_off=0;//token文件读取偏移

	int lines=2;//行号  二倍使用要除以二
	int level=0;//当前扫描的深度
	int var_off=0;
	typeTable typetable[10000];//自定义类型变量表
	int typetablenum=0;//自定义类型变量表数据量
	int temptypenum=-1;
	globalTable globaltable[10000];//全局符号表
	int globaltablenum=0;//自定义类型变量表数据量
	int tempglobalnum=-1;
};


int main(){
	Semantic_Analysis SA;
	SA.Analysis();
	// printf("Sematic Analysis Complete!!!\n");
    return ERROR;
}

