#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <fstream>
using namespace std;
const int MAX = 3000;
int ERROR=1;
struct Token
{
	string Typename;
	string value;
	int    isInterger;
	Token* next;
};
class Grammer_Analysis
{
public:
	Grammer_Analysis()
	{
		grammer_in_head=new Token;//哨兵头
		ReadToken();//先读取token序列
	}
	~Grammer_Analysis(){}
	//读取token序列
	void ReadToken(void)
	{
		FILE* fp=NULL;//文件指针
		if((fp=fopen(filepath,"r"))==NULL)
		{
			printf("cannot open this file\n");
			ERROR=0;
			exit(0);
		}
		Token* current=NULL;
		Token* bef_current=grammer_in_head;
		char key[100];
		char value[100];
		while(!feof(fp))
		{
			fscanf(fp,"< %s , %s >%*[\n]",key,value);
			// printf("KEY : %s   VALUE : %s\n",key,value );
			/*if (strcmp(key,"STR")==0 || strcmp(value,"\'")==0  )
			{
				continue;
			}*/
                current=new Token;
                current->Typename=key;
                current->value=value;
                current->next=NULL;
                bef_current->next=current;
                bef_current=current;
		}

		current=new Token;
		current->next=NULL;
		bef_current->next=current;

		bef_current=grammer_in_head;
		grammer_in_head=grammer_in_head->next;
		delete bef_current;
		fclose(fp);
	}
	//读取状态转换矩阵
	
	//读取一个字符
	int ReadCH(void)
	{
		if (grammer_in_head->next==NULL)
		{
			return -2;
		}
		string temp_name = grammer_in_head->Typename;
		string value = grammer_in_head->value;
		Token *p =grammer_in_head;
		grammer_in_head=grammer_in_head->next;
		delete p;
		nowValue = value;
		if (temp_name=="STR")
		{
			nowString = value;
			return 155;
		}
		else if (temp_name=="ID")
		{
            nowId = value;
			return 102;
		}
		else if (value=="program")
		{
			return 101;
		}
		else if (value=="type")
		{
			return 103;
		}
		else if (value=="var")
		{
			return 104;
		}
		else if (value=="procedure")
		{
			return 105;
		}
		else if (value=="begin")
		{
			return 106;
		}
		else if (value=="integer")
		{
			return 107;
		}
		else if (value=="char")
		{
			return 108;
		}
		else if (value=="array")
		{
			return 109;
		}
		else if (value=="record")
		{
			return 110;
		}
		else if (temp_name=="INTC")
		{
			nowINTC = value;
			return 111;
		}
		else if (value=="end")
		{
			return 112;
		}
		else if (value==";")
		{
			return 113;
		}
		else if (value==",")
		{
			return 114;
		}
		else if (value==")")
		{
			return 115;
		}
		else if (value=="if")
		{
			return 116;
		}
		else if (value=="while")
		{
			return 117;
		}
		else if (value=="return")
		{
			return 118;
		}
		else if (value=="read")
		{
			return 119;
		}
		else if (value=="write")
		{
			return 120;
		}
		else if (value=="else")
		{
			return 121;
		}
		else if (value=="fi")
		{
			return 122;
		}
		else if (value=="endwh")
		{
			return 123;
		}
		else if (value==":=")
		{
			return 124;
		}
		else if (value=="(")
		{
			return 125;
		}
		else if (value=="[")
		{
			return 126;
		}
		else if (value==".")
		{
			return 127;
		}
		else if (value=="<")
		{
			return 128;
		}

		else if (value=="=")
		{
			return 129;
		}
		else if (value=="]")
		{
			return 130;
		}
		else if (value=="then")
		{
			return 131;
		}
		else if (value=="+")
		{
			return 132;
		}
		else if (value=="-")
		{
			return 133;
		}else if (value=="do")
		{
			return 134;
		}
		else if (value=="*")
		{
			return 135;
		}
		else if (value=="/")
		{
			return 136;
		}
		else if (value=="\\n")
		{
			return 137;
		}
		else if (value=="..")
		{
			return 138;
		}
		else if (value=="of")
		{
			return 139;
		}
		else if (value=="'")
		{
			return 140;
		}
		else
		{
			printf("error token<%s,%s>\n",temp_name.c_str(),value.c_str() );
			ERROR=0;
			return -1;
		}
	}


	int encode(string tempName)
	{
		if (tempName=="ID")
		{
			return 102;
		}
		else if (tempName=="program")
		{
			return 101;
		}
		else if (tempName=="type")
		{
			return 103;
		}
		else if (tempName=="var")
		{
			return 104;
		}
		else if (tempName=="procedure")
		{
			return 105;
		}
		else if (tempName=="begin")
		{
			return 106;
		}
		else if (tempName=="integer")
		{
			return 107;
		}
		else if (tempName=="char")
		{
			return 108;
		}
		else if (tempName=="array")
		{
			return 109;
		}
		else if (tempName=="record")
		{
			return 110;
		}
		else if (tempName=="INTC")
		{
			return 111;
		}
		else if (tempName=="end")
		{
			return 112;
		}
		else if (tempName==";")
		{
			return 113;
		}
		else if (tempName==",")
		{
			return 114;
		}
		else if (tempName==")")
		{
			return 115;
		}
		else if (tempName=="if")
		{
			return 116;
		}
		else if (tempName=="while")
		{
			return 117;
		}
		else if (tempName=="return")
		{
			return 118;
		}
		else if (tempName=="read")
		{
			return 119;
		}
		else if (tempName=="write")
		{
			return 120;
		}
		else if (tempName=="else")
		{
			return 121;
		}
		else if (tempName=="fi")
		{
			return 122;
		}
		else if (tempName=="endwh")
		{
			return 123;
		}
		else if (tempName==":=")
		{
			return 124;
		}
		else if (tempName=="(")
		{
			return 125;
		}
		else if (tempName=="[")
		{
			return 126;
		}
		else if (tempName==".")
		{
			return 127;
		}
		else if (tempName=="<")
		{
			return 128;
		}

		else if (tempName=="=")
		{
			return 129;
		}
		else if (tempName=="]")
		{
			return 130;
		}
		else if (tempName=="then")
		{
			return 131;
		}
		else if (tempName=="+")
		{
			return 132;
		}
		else if (tempName=="-")
		{
			return 133;
		}else if (tempName=="do")
		{
			return 134;
		}
		else if (tempName=="*")
		{
			return 135;
		}
		else if (tempName=="/")
		{
			return 136;
		}
		else if (tempName=="Program")
		{
			return 1;
		}
		else if (tempName=="ProgramHead")
		{
			return 2;
		}
		else if (tempName=="ProgramName")
		{
			return 3;
		}
		else if (tempName=="DeclarePart")
		{
			return 4;
		}
		else if (tempName=="TypeDecpart")
		{
			return 5;
		}
		else if (tempName=="TypeDec")
		{
			return 6;
		}
		else if (tempName=="TypeDecList")
		{
			return 7;
		}
		else if (tempName=="TypeDecMore")
		{
			return 8;
		}
		else if (tempName=="TypeId")
		{
			return 9;
		}
		else if (tempName=="TypeDef")
		{
			return 10;
		}
		else if (tempName=="BaseType")
		{
			return 11;
		}
		else if (tempName=="StructureType")
		{
			return 12;
		}
		else if (tempName=="ArrayType")
		{
			return 13;
		}
		else if (tempName=="Low")
		{
			return 14;
		}
		else if (tempName=="Top")
		{
			return 15;
		}
		else if (tempName=="RecType")
		{
			return 16;
		}
		else if (tempName=="FieldDecList")
		{
			return 17;
		}
		else if (tempName=="FieldDecMore")
		{
			return 18;
		}
		else if (tempName=="IdList")
		{
			return 19;
		}
		else if (tempName=="IdMore")
		{
			return 20;
		}
		else if (tempName=="VarDecpart")
		{
			return 21;
		}
		else if (tempName=="VarDec")
		{
			return 22;
		}
		else if (tempName=="VarDecList")
		{
			return 23;
		}
		else if (tempName=="VarDecMore")
		{
			return 24;
		}
		else if (tempName=="VarIdList")
		{
			return 25;
		}
		else if (tempName=="VarIdMore")
		{
			return 26;
		}
		else if (tempName=="ProcDecPart")
		{
			return 27;
		}
		else if (tempName=="ProcDec")
		{
			return 28;
		}
		else if (tempName=="ProcDecMore")
		{
			return 29;
		}
		else if (tempName=="ProcName")
		{
			return 30;
		}
		else if (tempName=="ParamList")
		{
			return 31;
		}
		else if (tempName=="ParamDecList")
		{
			return 32;
		}
		else if (tempName=="ParamMore")
		{
			return 33;
		}
		else if (tempName=="Param")
		{
			return 34;
		}
		else if (tempName=="FormList")
		{
			return 35;
		}
		else if (tempName=="FidMore")
		{
			return 36;
		}
		else if (tempName=="ProcDecParts")
		{
			return 37;
		}
		else if (tempName=="ProcBody")
		{
			return 38;
		}
		else if (tempName=="ProgramBody")
		{
			return 39;
		}
		else if (tempName=="StmList")
		{
			return 40;
		}
		else if (tempName=="StmMore")
		{
			return 41;
		}
		else if (tempName=="Stm")
		{
			return 42;
		}
		else if (tempName=="AssCall")
		{
			return 43;
		}
		else if (tempName=="AssignmentRest")
		{
			return 44;
		}
		else if (tempName=="ConditionalStm")
		{
			return 45;
		}
		else if (tempName=="LoopStm")
		{
			return 46;
		}
		else if (tempName=="InputStm")
		{
			return 47;
		}
		else if (tempName=="Invar")
		{
			return 48;
		}
		else if (tempName=="OutputStm")
		{
			return 49;
		}
		else if (tempName=="ReturnStm")
		{
			return 50;
		}
		else if (tempName=="CallStmRest")
		{
			return 51;
		}
		else if (tempName=="ActParamList")
		{
			return 52;
		}
		else if (tempName=="ActParamMore")
		{
			return 53;
		}
		else if (tempName=="RelExp")
		{
			return 54;
		}
		else if (tempName=="OtherRelE")
		{
			return 55;
		}
		else if (tempName=="Exp")
		{
			return 56;
		}
		else if (tempName=="OtherTerm")
		{
			return 57;
		}
		else if (tempName=="Term")
		{
			return 58;
		}
		else if (tempName=="OtherFactor")
		{
			return 59;
		}
		else if (tempName=="Factor")
		{
			return 60;
		}
		else if (tempName=="Variable")
		{
			return 61;
		}
		else if (tempName=="VariMore")
		{
			return 62;
		}
		else if (tempName=="FieldVar")
		{
			return 63;
		}
		else if (tempName=="FieldVarMore")
		{
			return 64;
		}
		else if (tempName=="CmpOp")
		{
			return 65;
		}
		else if (tempName=="AddOp")
		{
			return 66;
		}
		else if (tempName=="MultOp")
		{
			return 67;
		}
		else if (tempName=="\n")
		{
			return 137;
		}
		else if (tempName=="..")
		{
			return 138;
		}
		else if (tempName=="of")
		{
			return 139;
		}
		else if (tempName=="'")
		{
			return 140;
		}
		else
		{
			printf("Unkonwn error：%s\n",tempName );
			ERROR=0;
			return -1;
		}
	}
	string decode(int num)
	{
		switch(num)
		{
			case 101: return "program";
			case 102: return "ID";
			case 103: return "typedef";
			case 104: return "";
			case 105: return "int";
			case 106: return "{";
			case 107: return "int";
			case 108: return "char";
			case 109: return "array";
			case 110: return "struct";
			case 111: return "INTC";
			case 112: return "}";
			case 113: return ";";
			case 114: return ",";
			case 115: return ")";
			case 116: return "if";
			case 117: return "while";
			case 118: return "return";
			case 119: return "read";
			case 120: return "write";
			case 121: return "else";
			case 122: return "}";
			case 123: return "}";
			case 124: return ":=";
			case 125: return "(";
			case 126: return "[";
			case 127: return ".";
			case 128: return "<";
			case 129: return "=";
			case 130: return "]";
			case 131: return "{";
			case 132: return "+";
			case 133: return "-";
			case 134: return "{";
			case 135: return "*";
			case 136: return "/";

			case 137: return "\n";
			case 138: return "..";
			case 139: return "of";
			case 140: return "'";
			case 1: return "Program";
			case 2: return "ProgramHead";
			case 3: return "ProgramName";
			case 4: return "DeclarePart";
			case 5: return "TypeDecpart";
			case 6: return "TypeDec";
			case 7: return "TypeDecList";
			case 8: return "TypeDecMore";
			case 9: return "TypeId";
			case 10: return "TypeDef";
			case 11: return "BaseType";
			case 12: return "StructureType";
			case 13: return "ArrayType";
			case 14: return "Low";
			case 15: return "Top";
			case 16: return "RecType";
			case 17: return "FieldDecList";
			case 18: return "FieldDecMore";
			case 19: return "IdList";
			case 20: return "IdMore";
			case 21: return "VarDecpart";
			case 22: return "VarDec";
			case 23: return "VarDecList";
			case 24: return "VarDecMore";
			case 25: return "VarIdList";
			case 26: return "VarIdMore";
			case 27: return "ProcDecPart";
			case 28: return "ProcDec";
			case 29: return "ProcDecMore";
			case 30: return "ProcName";
			case 31: return "ParamList";
			case 32: return "ParamDecList";
			case 33: return "ParamMore";
			case 34: return "Param";
			case 35: return "FormList";
			case 36: return "FidMore";
			case 37: return "ProcDecParts";
			case 38: return "ProcBody";
			case 39: return "ProgramBody";
			case 40: return "StmList";
			case 41: return "StmMore";
			case 42: return "Stm";
			case 43: return "AssCall";
			case 44: return "AssignmentRest";
			case 45: return "ConditionalStm";
			case 46: return "LoopStm";
			case 47: return "InputStm";
			case 48: return "Invar";
			case 49: return "OutputStm";
			case 50: return "ReturnStm";
			case 51: return "CallStmRest";
			case 52: return "ActParamList";
			case 53: return "ActParamMore";
			case 54: return "RelExp";
			case 55: return "OtherRelE";
			case 56: return "Exp";
			case 57: return "OtherTerm";
			case 58: return "Term";
			case 59: return "OtherFactor";
			case 60: return "Factor";
			case 61: return "Variable";
			case 62: return "VariMore";
			case 63: return "FieldVar";
			case 64: return "FieldVarMore";
			case 65: return "CmpOp";
			case 66: return "AddOp";
			case 67: return "MultOp";
		}
		ERROR=0;
		return "Unkonwn error";
	}


    void rdToken() 
    {
		if(grammer_in_head->next == NULL)
		{
            flag = 0;
			return;
		}
		nowInput = ReadCH();
		while (nowInput == 137)
		{
			line ++;
			nowInput = ReadCH();
		}

	}




    void normal()
    {
		if(nowInput == encode("type"))
		{
			typeFlag = 1;
			typeType();
			typeFlag = 0;
			return;
		}
		if(nowInput == encode("record"))
		{
			recordType();
			return;
		}
		if(nowInput == encode("array"))
		{
			arrayType();
			return;
		}
		if(nowInput == encode("if") && ifFlag == 0)
		{
			ifFlag = 1; //ifFlag用处：防止ifSentences开头出现的死循环，来回调用两个函数
			ifSentences();
			return;
		}
		if(nowInput == encode("while") && whileFlag == 0)
		{
			whileFlag = 1;
			whileSentences();
			return;
		}

		if (nowInput == encode("'"))
		{
			code[currentLine].append("'");
		}
		else if (nowInput == 102)
		{
			code[currentLine].append(nowId);
		}
		else if(nowInput == 155)
		{
			code[currentLine].append(nowString);
		}
		else if (nowInput == 124)
		{
			code[currentLine].append("=");
		}
		else if (nowInput == encode("INTC"))
		{
			code[currentLine].append(nowINTC);
		}
		else
		{
			code[currentLine].append(decode(nowInput));
		}
		if(nowInput == encode(";") || nowInput == encode("begin") || nowInput == encode("end"))
		{
			code[currentLine ++].append("\n");
		}
		lastInput = nowInput;
		rdToken();
		if (nowInput != encode(";") && nowInput != encode("[") && nowInput != encode("]") && lastInput != encode("[") && nowInput != encode(",") && nowInput != encode("(") && nowInput != encode(")") && lastInput != encode("(")
		&& nowInput != encode("'") && lastInput != encode("'"))
		{
			code[currentLine].append("  ");
		}
	}


	void arrayType()
	{
		string value = "";
		for (int i = 0; i < 7; i++)
		{
			if(nowInput == encode("INTC") && nowINTC != "1")
			{
				value = nowINTC; //读取数组的容量
				if(typeFlag == 1)
				{
					numValue = value;
				}
			}
			rdToken();
		}

		normal();
		
		while(nowInput != encode(";"))
		{
			if (nowInput == encode("ID"))
			{
				normal();
				code[currentLine].append("[");
				code[currentLine].append(value);
				code[currentLine].append("]");
			}
			else
			{
				normal();
			}
		}
		//normal();
		//code[currentLine].append("[");
		//code[currentLine].append(value);
		//code[currentLine].append("]");
		
		
	}
	void recordType()
	{
		rdToken();
		code[currentLine].append("struct");
		code[currentLine ++].append("\n");
		code[currentLine ++].append("{\n");
		while (1)
		{
			if(nowInput == encode("end"))
			{
				code[currentLine].append("}");
				rdToken();
				break;
			}
			normal();
		}
		while(nowInput != encode(";"))
		{
			normal();
		}

	}

	void typeType()
	{
		rdToken();
		while (1)
		{
			if(nowInput == encode("var") || nowInput == encode("procedure") || nowInput == encode("begin"))
			{
				return;
			}
			code[currentLine].append("typedef	");
			string subject = "";
			string object = "";
			
			int subFlag = 0;
			while (nowInput != encode(";"))
			{
				if(nowInput == encode("=")) 
				{
					rdToken(); //过滤掉=号
					subFlag = 1; //转换为主体
				}
				if(subFlag == 0)   //等号左边
				{
					if(nowInput == encode("ID"))
					{
						if(lastInput == encode(","))
						{
							object.append(",");

						}
						object.append(nowId);
					}
					rdToken();
					
				}
				else    //等号右边
				{
					if(nowInput ==  encode("array"))
					{
						tyArrayFlag = 1;
					}
					normal();
				}
				
			}
			code[currentLine].append("	");
			code[currentLine].append(object);
			if(tyArrayFlag == 1)
			{
				code[currentLine].append("[");
				code[currentLine].append(numValue);
				code[currentLine].append("]");
				tyArrayFlag = 0;
			}
			rdToken();
			code[currentLine ++].append(";\n");
			
		}
		
	}
	void ifSentences()
	{
		normal(); //将token if读出
		ifFlag = 0;
		code[currentLine].append("(");
		while (nowInput != encode("then"))
		{
			if(nowInput == encode("="))
			{
				rdToken();
				code[currentLine].append("==");
			}
			else
			{
				normal();
			}
		}
		code[currentLine].append(")");
		code[currentLine ++].append("\n");
		while (1)
		{
			if(nowInput == encode("else"))
			{
				if (lastInput != encode("end") && lastInput != encode("endwh") && lastInput != encode("fi"))
				{
					code[currentLine ++].append(";\n");
				}
	
				code[currentLine ++].append("}\n");
				normal();
				code[currentLine ++].append("\n");
				code[currentLine ++].append("{\n");
			}	
			if(nowInput == encode("fi"))
			{
				if (lastInput != encode("end") && lastInput != encode("endwh") && lastInput != encode("fi"))
				{
					code[currentLine ++].append(";\n");
				}
				normal();
				if(nowInput == encode(";"))
				{
					rdToken();
				}
				code[currentLine ++].append("\n");
				return;
			}
			normal();
		}
			
	}

	void whileSentences()
	{
		normal(); //将 "while" 对应的token读出来
		whileFlag = 0;
		code[currentLine].append("(");
		while (nowInput != encode("do"))
		{
			if(nowInput == encode("="))
			{
				rdToken();
				code[currentLine].append("==");
			}
			else
			{
				normal();
			}
		}
		code[currentLine].append(")");
		code[currentLine ++].append("\n");
		while (1)
		{
			if(nowInput == encode("endwh"))
			{
				if (lastInput != encode("end") && lastInput != encode("endwh") && lastInput != encode("fi"))
				{
					code[currentLine ++].append(";\n");
				}
				normal();
				if(nowInput == encode(";"))
				{
					rdToken(); //如果endwh后面遇到一个“;”，则将其吞掉
				}
				code[currentLine ++].append("\n");
				return;
			}
			normal();
		}
	}
    /****全局声明部分*****/
    void allDec()
    {
		while (nowInput != encode("procedure"))
		{
			normal();
		}
		

    }

    /***函数声明部分******/
    void decfunc()
    {
		while (1)
		{
			if(nowInput == encode(";") && lastInput == encode(")"))
			{
				break;
			}
			if(nowInput == encode(";"))
			{
				rdToken();
				code[currentLine].append(",");
			}
			normal();
		}
		rdToken(); //过滤掉一个";"
		code[currentLine ++].append("\n");
		code[currentLine ++].append("{\n");
		while (1)
		{
			if(nowInput == encode("end"))
			{
				if (lastInput != encode("end") && lastInput != encode("endwh") && lastInput != encode("fi"))
				{
					code[currentLine ++].append(";\n");
				}
				normal();
				return;
			}
			if(nowInput == encode("begin"))
			{
				rdToken(); //此时过滤掉一个"begin"
			}
			normal();
			
		}
		
    }

    /****主程序部分********/
    void mainFunc()
    {
		code[currentLine ++].append("int main()\n");
		normal();
		while (1)
		{
			if(nowInput == encode("end"))
			{
				if (lastInput != encode("end") && lastInput != encode("endwh") && lastInput != encode("fi"))
				{
					code[currentLine ++].append(";\n");
				}
				code[currentLine ++].append("return 0;\n");
				normal();
				return;
			}
			normal();

		}
		
    }
	void preTreat()
	{
		code[currentLine ++].append("#include <stdio.h> \n");
        code[currentLine ++].append("#include <stdlib.h> \n");
		
	}
	void defineWrite()
	{
		code[currentLine ++].append("void write(int a)\n");
		code[currentLine ++].append("{\n");
		code[currentLine ++].append("printf(\"%d\",a);\n");
		code[currentLine ++].append("}\n");
		code[currentLine ++].append("void write(char c)\n");
		code[currentLine ++].append("{\n");
		code[currentLine ++].append("printf(\"%c\",c);\n");
		code[currentLine ++].append("}\n");
	}
	void defineRead()
	{
		code[currentLine ++].append("void read(int &a)\n");
		code[currentLine ++].append("{\n");
		code[currentLine ++].append("scanf(\"%d\",&a);\n");
		code[currentLine ++].append("}\n");
		code[currentLine ++].append("void read(char &c)\n");
		code[currentLine ++].append("{\n");
		code[currentLine ++].append("scanf(\"%c\",&c);\n");
		code[currentLine ++].append("}\n");
	}

    void output()
    {
        preTreat();
        rdToken();
		rdToken();
		rdToken();
		allDec();
		defineWrite();
		defineRead();
		while (nowInput == encode("procedure"))
		{
			decfunc();
		}
		
        mainFunc();
		

    }
    void print()
   {
    ofstream fp("out.cpp");
	for(int i=0;i<=currentLine;i++){
		fp<<code[i].c_str();
	}
}



	
private:
	Token* grammer_in_head=NULL;//输入流链表（token）
	char filepath[10000]="token.txt";
	
	int line=2;//行号
    string code[MAX] = {""};
    int currentLine = 0;
    string nowId = "";
	string nowINTC = "";
    int nowInput = 0;
	int lastInput = 0;
    int flag = 1;
	string nowValue;
	int ifFlag = 0;
	int whileFlag = 0;

	int typeFlag = 0;
	int tyArrayFlag = 0;
	string numValue = "";
	string nowString = "";

};



int main(){
	Grammer_Analysis ga;
    ga.output();
    ga.print();
    return 0;
}



