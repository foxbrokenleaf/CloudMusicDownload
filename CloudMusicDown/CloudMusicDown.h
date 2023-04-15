#pragma once
#ifndef _CLOUDMUSICDOWN_H
#define _CLOUDMUSICDOWN_H
#include <string>
#include<curl.h>
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<Windows.h>
#include<direct.h>
using namespace std;
//数据结构
class song {
public:
	string Name;	//歌曲名
	string Id;	//歌曲ID
	song *next;
};
//单向链表(需求中无需删除节点）
class quees {	
private:	
	song *head;	//链头
	int num;	//计数器
public:
	quees();	//构造函数
	~quees();	//析构函数
	void add(string , string ); //添加节点（从尾部添加）
	void show(); //显示所有节点（用于DEBUG)
	string get(int ); //获取节点数据
};
CURL *curlinit();
void curl_fun(string ,string,int );
void curlwrite(CURL* ,FILE *);
void curlcleanup(CURL *);
inline size_t r_write_call(char *ptr, size_t size, size_t nmemb, FILE *fp) { return fwrite(ptr, size, nmemb, fp); }
void Gfind(string url,quees &);
string ansi(wstring ptr);
wstring utf8(const char *ptr);
#endif // _CLOUDMUSICDOWN_H
