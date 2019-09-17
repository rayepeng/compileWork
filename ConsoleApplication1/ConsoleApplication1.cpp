#define _CRT_SECURE_NO_WARNINGS
// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <map>

using namespace  std;

map<string, int> result;
map<string, float>result2;

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


int eval(string s)
{

	return 1;
}

void extract_val(string s, int flag)
{
	//int i = 1; flag代表是哪种类型的
	if(flag == 1)
	{
		int fenhao = s.find_first_of(';');
		string var = s.substr(3, fenhao-3);
		var = trim(var);
		//要查重
		result.insert(pair<string, int>(var, 0));
	}
	if(flag == 2)
	{
		int fenhao = s.find_last_of(';');
		string var = s.substr(5, fenhao - 3);
		var = trim(var);
		result2.insert(pair<string, float>(var, 0.0));
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
			extract_val(s, 1);
		}
	}
	else if(s.substr(0, 5) == "float"){
		if(s.find('=') != -1)
		{
			cout << "暂时不支持";
		}
		else
		{
			extract_val(s, 2);
		}
	}
	else if(s.substr(0, 6) == "double")
	{
		
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

	//开始读写文件
	while(!infile.eof())
	{
		getline(infile, data);
		data = trim(data);
		lexical(data);
	}

}