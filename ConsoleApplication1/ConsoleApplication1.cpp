#define _CRT_SECURE_NO_WARNINGS
// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <map>

using namespace  std;

map<string, string> result;

//实现trim函数
string& trim(string &s)
{
	if(s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

void lexical(string s)
{
	if(s.find('int') == 0)
	{
		cout << s;
	}
	// else if(s.find('float') == 0)
	// {
	// 	
	// }

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
		cout << data.find_first_of('float');
	}

}