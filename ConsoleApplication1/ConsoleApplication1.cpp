#define _CRT_SECURE_NO_WARNINGS
// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <map>
#include <math.h>
using namespace  std;

map<string, int> resultofInt; //存整数的
map<string, float>result2; //存浮点数的

int flag_is_int = 1;
float factor_value();
float term_value();
float expression_value();
//实现trim函数
string trim(string &s)
{
	if(s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

string line;
void eval(string s)
{
	line = s.substr(s.find('=')+1).substr(0, s.find_last_of(';'));
	line = trim(line);
	string var = s.substr(0, s.find('=') - 1);
	var = trim(var);
	map<string, int>::iterator iter;
	int flag = 0;
	if(resultofInt.count(var))
	{
		flag = 1;
		resultofInt[var] = expression_value();
	}
	if(result2.count(var))
	{
		flag = 2;
		result2[var] = expression_value();
	}
}

float expression_value()
{
	//表达式是由tem+-term得到的
	//计算出第一项的值
	line = trim(line);
	float result = term_value();
	bool more = true;
	while (more)
	{
		line = trim(line);
		char op = line[0];
		if (op == '+' || op == '-')
		{
			line = line.erase(0,1);
			if (op == '+')
			{
				result += term_value();
			}
			else
			{
				result -= term_value();
			}
		}
		else
		{
			more = false;
		}

	}
	return result;
}

float term_value()
{
	//计算项的值
	//term = factor*factor || factor / factor
	float result = factor_value();
	bool more = true;
	while (more)
	{
		//读取op之前一定要先去除首尾空格
		line = trim(line);
		char op = line[0];
		while(op == ' ')
		{
			line = line.erase(0, 1);
			op = line[0];
		}
		if (op == '*' || op == '/')
		{
			line = line.erase(0, 1);
			if (op == '*')
				result *= factor_value();
			else
			{
				result /= factor_value();
			}

		}
		else
		{
			more = false;
		}
	}
	return result;
}


float factor_value()
{
	// (expression_value) || 整数
	line = trim(line);
	float result = 0;
	char op = line[0];
	if (op == '(')
	{
		line = trim(line);
		line = line.erase(0, 1);
		result = expression_value();
		line = trim(line);
		line = line.erase(0, 1);
		
	}
	else
	{
		int countFloat = 0;
		bool isFloat = false;

		//这里有点小问题如果是变量参与运算
		//考虑到变量参与运算，那么op就不是digit
		if(!isdigit(op) && op != '*' && op != '/')
		{
			string lineTemp = line;
			int count = 1;
			//考虑变量名字很长
			for(int i = 1; i < lineTemp.length(); i++)
			{
				if(lineTemp[i] == '*' || lineTemp[i] == '/')
				{
					break;
				}
				count++;
			}
			line.erase(0, count); //将变量这个字符清除掉
			string var = lineTemp.substr(0, count);
			var = trim(var);
			if(resultofInt.count(var))
			{
				result = resultofInt[var];
				return  result;
			}
			else if(result2.count(var))
			{
				result = result2[var];
				return  result;
			}
		}
		while (isdigit(op) || op == '.')
		{
			
			if(op == '.' )
			{
				isFloat = true;
				line = line.erase(0, 1);
				op = line[0];
				continue;

			}
			if(isFloat)
			{
				//说明是浮点数，先按照整数提取的来
				result = 10 * result + op - '0';
				line = line.erase(0, 1);
				op = line[0];
				countFloat++; //统计浮点数后面有多少位
			}
			else
			{
				result = 10 * result + op - '0';
				line = line.erase(0, 1);
				op = line[0];
			}
		}
		if(countFloat)
		{
			result = result / pow(10, countFloat);
		}
	}
	return result;
}
void extract_val(string s)
{
	//int i = 1; flag代表是哪种类型的
	if(flag_is_int == 1)
	{
		//先找到分号的位置，然后提取出来注意substr函数有点不一样
		int fenhao = s.find_first_of(';');
		string var = s.substr(3, fenhao-strlen("int"));
		var = trim(var);
		//要查重
		if(resultofInt.count(var))
		{
			cout << "变量重复"<<var<<"定义！" << endl;
		}
		else
		{
			resultofInt.insert(pair<string, int>(var, 0));
		}
		
	}
	if(flag_is_int == 2)
	{
		int fenhao = s.find_last_of(';');
		string var = s.substr(5, fenhao - strlen("float"));
		var = trim(var);
		if(result2.count(var))
		{
			cout << "变量重复" << var << "定义！" << endl;
		}
		else
		{
			result2.insert(pair<string, float>(var, 0.0));
		}
		
	}

}
void lexical(string s)
{
	if(s.substr(0,3) == "int")
	{
		if(s.find('=') != -1)
		{
			//说明在赋值
			cout << "暂时不支持";
		}
		else
		{
			flag_is_int = 1;
			extract_val(s);
		}
	}
	else if(s.substr(0, 5) == "float"){
		if(s.find('=') != -1)
		{
			cout << "暂时不支持";
		}
		else
		{
			//flag_is_int=2代表浮点数,然后开始解析表达式
			flag_is_int = 2;
			extract_val(s);
		}
	}
	else if(s.substr(0, 6) == "double")
	{
		
	}

	else if(s.find('=')!=-1)
	{
		eval(s);
	}
	else if(s.substr(0, 5) == "write")
	{
		//提取变量
		/**
		 * 输出这里还有一点问题，只在result中寻找了
		 */
		string var = s.substr(s.find("(") + 1, s.find_last_of(')') - s.find('(')-1);
		if(resultofInt.count(var))
		{
			cout << resultofInt[var] << endl;
		}else if(result2.count(var))
		{
			cout << result2[var] << endl;
		}
	}

}

int main(char argc, char *argv[])
{
	// FILE* f = fopen("test", "r");
	// string data;
	// fscanf(f, "%s", data);
	string data;
	ifstream infile;
	infile.open("test");
	if(!infile.is_open())
	{
		cout << "打开文件错误！！" << endl;
	}
	// getline(infile, data);

	// 开始读写文件
	 while(!infile.eof())
	 {
	 	getline(infile, data);
	 	data = trim(data);
	 	lexical(data);
	 }

	 system("pause");
	 
	// map<string, int> s;
	// s["a"] = 1;
	// map<string, int>::iterator iter;
	// iter = s.find("b");
	// if(s.count("b"))
	// {
	// 	cout << "hhh";
	// }
}