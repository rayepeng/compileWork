#define _CRT_SECURE_NO_WARNINGS
// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <map>
#include <math.h>
#include <vector>
using namespace  std;

map<string, int> resultofInt; //存整数的
map<string, float>result2; //存浮点数的
vector<string> UndefineVar; //存储未定义的变量
string line; //每次读取文件中的一行
int flag_is_int = 1;
float factor_value(string var);
float term_value(string var);
float expression_value(string var);
//实现trim函数
/**
 *由于输入的数据中不可避免地存在很多空白的字符所以一定要将空白符去除掉
 * @parma string &s 输入的字符串
 * @return string 返回被去除了首尾空格的字符串
 */
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

/**
 * @param string s传入的表达式
 * @return void 
 */
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
		resultofInt[var] = expression_value(var);
	}
	if(result2.count(var))
	{
		flag = 2;
		result2[var] = expression_value(var);
	}
}

/**
 * @param string var 输入的表达式
 * @return float 表达式的值
 */
float expression_value(string var)
{
	//表达式是由tem+-term得到的
	//计算出第一项的值
	line = trim(line);
	float result = term_value(var);
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
				result += term_value(var);
			}
			else
			{
				result -= term_value(var);
			}
		}
		else
		{
			more = false;
		}

	}
	return result;
}

/**
 * @param string var 表达式中的项
 * @return float 项的值
 */
float term_value(string var)
{
	//计算项的值
	//term = factor*factor || factor / factor
	float result = factor_value(var);
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
				result *= factor_value(var);
			else
			{
				result /= factor_value(var);
			}

		}
		else
		{
			more = false;
		}
	}
	return result;
}

/**
 * @param string var 项中的因子
 * @return float 项的值
 */
float factor_value(string var)
{
	// (expression_value) || 整数
	line = trim(line);
	float result = 0;
	char op = line[0];
	if (op == '(')
	{
		line = trim(line);
		line = line.erase(0, 1);
		result = expression_value(var);
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
			string varFind = lineTemp.substr(0, count);
			varFind = trim(varFind);
			if(resultofInt.count(varFind))
			{
				result = resultofInt[varFind];
				return  result;
			}
			else if(result2.count(varFind))
			{
				result = result2[varFind];
				return  result;
			}else
			{
				cout << "变量" << varFind << "未定义" << endl;
				//如果被赋值的变量中存在未定义的变量那么这个变量也是未定义的
				UndefineVar.push_back(varFind);
				UndefineVar.push_back(var);
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

/**
 *解析文本中的一行
 * @param string s文件中一行
 * @return void
 */
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

/**
 * @param string s
 * @return void
 */
void lexical(string s)
{
	if(s.substr(0,3) == "int")
	{
		if(s.find('=') != -1)
		{
			//说明在赋值
			cout << "暂时不支持声明变量的时候赋值";
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
			cout << "暂时不支持不支持声明变量的时候赋值";
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
		cout << "不支持double类型的数据" << endl;
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
		vector<string>::iterator it;
		it = find(UndefineVar.begin(), UndefineVar.end(), var);
		if(it != UndefineVar.end())
		{
			//说明变量是未定义的
			return;
		}
		if(resultofInt.count(var))
		{
			cout << resultofInt[var] << endl;
		}else if(result2.count(var))
		{
			cout << result2[var] << endl;
		}
	}

}

int main(int argc, char *argv[])
{
	// FILE* f = fopen("test", "r");
	// string data;
	// fscanf(f, "%s", data);
	string data;
	ifstream infile;
	// if(argc != 2)
	// {
	// 	cout << "没有指定输入的参数";
	// }
	/*printf("%s", argv[1]);
	return 1;*/

	// cout << "请输入文件名,如果不输入则默认是目录下的test文件 >> " ;
	infile.open(argv[1]);
	// string filename;
	// cin >> filename;
	// if(filename.length() != 0)
	// {
	// 	infile.open(filename);
	// }else
	// {
	// 	cout << "没有指定输入默认打开test文件" << endl;
	// 	infile.open("test");
	// }
	// infile.open("test");
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
}