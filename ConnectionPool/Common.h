#pragma once
#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<mysql.h>
#include<time.h>
#include<chrono>
using namespace std;


#define INFO 1
#define WARNING 2
#define ERROR_ 3
#define FALTA 4

#define LOG(level,message)  log(#level,message,__FILE__,__LINE__)
static void log(string level, string message, string file_name, int line) {
		//显示当前事件
	   char now[1024] = { 0 };
	   time_t tt = time(nullptr);
	   struct tm* ttime;
	   ttime = localtime(&tt);
	   strftime(now, 1024, "%Y-%m-%d %H:%M:%S", ttime);
	   cout << "[" << now << "" << "][" << level << "]" << "[" << message << "]" << "[" << file_name << "]" << "[" << line << "]" << endl;

}

static bool CutString(string src, string& key, string& val)
{
	int index = src.find("=");
	if (index== string::npos)
	{
		return false;
	}

	key = src.substr(0, index);
	val = src.substr(index + 1);


	return true;
}

