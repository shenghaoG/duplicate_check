#ifndef DUPCHECK_H_INCLUDED
#define DUPCHECK_H_INCLUDED
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <queue>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>

using namespace std;

class FRE
{
public:
    string word;    //被统计的词
    double fre;     //频率

    FRE (string w, double f);       //构造函数

    bool operator < (const FRE& other);     //运算符重载

};

class DupCheck
{
private:
    string fileName1;   //代码1文件名
    string fileName2;   //代码2文件名

    //读取文件内容，用于读取符号或者关键字集合
    unordered_set <string> read_keyword(string fileName);

    //文件预处理，将符号替换成空格，并做初步词频统计
    void text_preprocessing(string fileName, unordered_map <string, int>& frequency,
                            unordered_set <string> commonSymbol);

    //文件读入，逐词读入，并做二次词频统计
    void frequency_statistics (string fileName, unordered_set <string> keyword,
                                unordered_map <string, int>& frequency);

    //对于以上步骤得到的单词出现频数，转化成频率后，从大到小排序
    vector <FRE> key_vector_extraction (unordered_map <string, int> freq);

    //利用上述函数得到两份代码的词频特征向量，并求得两向量相似度
    double calculate_similarity (vector <FRE> code1, vector <FRE> code2);

public:
    DupCheck (string f1, string f2);     //构造函数

    string get_fileName1 ();    //获取私有变量的值fileName1
    string get_fileName2 ();    //获取私有变量的值fileName2

    //外部函数接口，获取查重率和词频统计结果
    double duplicate_checking (vector <FRE> &keyVector1, vector <FRE> &keyVector2);

};

#endif // DUPCHECK_H_INCLUDED
