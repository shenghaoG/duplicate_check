
#include "DupCheck.h"

using namespace std;

FRE::FRE (string w, double f)
{
    word = w;
    fre = f;
}

bool FRE::operator < (const FRE& other)
{
    return fre > other.fre;
}

DupCheck::DupCheck (string f1, string f2)
{
    fileName1 = f1;
    fileName2 = f2;
}

string DupCheck::get_fileName1 ()
{
    return fileName1;
}
string DupCheck::get_fileName2 ()
{
    return fileName2;
}

/*函数作用：文件预处理，将符号替换成空格，并做初步词频统计。
主要参数：fileName： 需要预处理的文件的文件名；
        frequency： 本步骤中统计的符号的出现频数；
        commonSymbol: 需要进行统计的符号集合，由函数read_keyword读入。
返回值：无
*/
void DupCheck::text_preprocessing(string fileName, unordered_map <string, int>& frequency,
                        unordered_set <string> commonSymbol)
{
    fstream file;   //定义文件类型变量
    char buffer[2];     //用于暂存当前读入的字符，并存储为字符数组
    buffer[1] = '\0';   //添加字符串结束标志
    file.open(fileName, ios::in | ios::out | ios::binary);  //打开文件，并设置为可读可写，允许二进制
    if (! file.is_open())     //文件打开失败则退出并报错
    {
        cout << "Error opening file " << fileName << endl;
        exit (1);
    }

    //遍历文件
    while(file.peek() != EOF)   //读至文件末未结束
    {
        file.get(buffer[0]);    //读取一个字符
        if (! (buffer[0] >= 'A' && buffer[0] <= 'Z') && ! (buffer[0] >= '0' && buffer[0] <= '9')
            && ! (buffer[0] >= 'a' && buffer[0] <= 'z'))    //判断是否为符号
        {
            file.seekg(-1, ios::cur);   //文件标识符前移一位，将此字符修改为空格
            file<<' ';
        }
        string preSymbol(buffer);   //转为string类型变量
        if (commonSymbol.count(preSymbol))  //如果是需要统计的符号
        {
            //进行词频统计
            if (frequency.count(preSymbol))
                frequency[preSymbol]++;
            else
                frequency.insert(pair <string, int> (preSymbol, 1));
        }
    }

    file.close();   //关闭文件
}

/*
函数作用：文件读入，逐词读入，并做二次词频统计。
主要参数：fileName： 需要做二次词频统计的文件的文件名；
        keyword： 需要忽略的常用关键字集合，由函数read_keyword读入；
        frequency： 本步骤中统计的符号的出现频数。
返回值：无
*/
void DupCheck::frequency_statistics (string fileName, unordered_set <string> keyword,
                                                  unordered_map <string, int>& frequency)
{
    string buffer;  //暂存读入的一个字符串
    ifstream in(fileName);    //打开文件
    if (! in.is_open())     //文件打开失败则退出并报错
    {
        cout << "Error opening file " << fileName << endl;
        exit (1);
    }

    //遍历整个文件，对读入的每个词做词频统计并筛除C++常用关键词
    while (in.peek() != EOF)
    {
        in >> buffer;
        //筛除C++常用关键词
        if (keyword.count(buffer))
            continue;

        //词频统计
        if (frequency.count(buffer))
            frequency[buffer]++;
        else
            frequency.insert(pair <string, int> (buffer, 1));

    }

    in.close(); //关闭文件
}

/*
函数作用：对于以上步骤得到的单词出现频数，转化成频率后，从大到小排序。
主要参数：freq： 统计好的单词出现频数。
返回值：频率向量。
*/
vector <FRE> DupCheck::key_vector_extraction (unordered_map <string, int> freq)
{
    unordered_map <string, int>::iterator it;   //遍历fre的迭代器
    vector <FRE> keyVector;     //提取出的词频特征向量

    //计算统计的总次数
    int sum = 0;
    for (it = freq.begin(); it != freq.end(); it++)
    {
        sum += it->second;
    }

    //计算频率
    for (it = freq.begin(); it != freq.end(); it++)
    {
        keyVector.push_back(FRE (it->first, it->second * 1.0 / sum));
    }

    //排序
    sort(keyVector.begin(), keyVector.end());

    return keyVector;   //返回
}

/*
函数作用：利用上述函数得到两份代码的词频特征向量，并求得两向量相似度。
主要参数：
code1： 第一份代码的词频特征向量
code2： 第二份代码的词频特征向量
返回值： 向量的余弦相似度
 */
double DupCheck::calculate_similarity (vector <FRE> code1, vector <FRE> code2)
{
    vector <FRE>::iterator it1, it2;    //遍历向量的迭代器
    double product = 0.0, quadraticSum1 = 0.0, quadraticSum2 = 0.0;     //用于存取内积和向量的模

    //计算内积和向量的模
    for (it1 = code1.begin(), it2 = code2.begin(); it1 != code1.end()
            && it2 != code2.end(); it1++, it2++)
    {
        product += (it1->fre) * (it2->fre);
        quadraticSum1 += (it1->fre) * (it1->fre);
        quadraticSum2 += (it2->fre) * (it2->fre);
    }

    //计算相似度
    return (product / (sqrt(quadraticSum1) * sqrt(quadraticSum2)) - 0.95) / 0.05;
}

/*
函数作用：常用关键字和需统计符号存储于文件当中，需要时通过此函数读取。
参数说明：filename：文件名。
返回值：关键字或符号集合。
*/
unordered_set <string> DupCheck::read_keyword(string fileName)
{
    unordered_set <string> keyword;
    string buffer;  //暂存读入的一个字符串
    ifstream in(fileName);  //打开文件
    if (! in.is_open())     //文件打开失败则退出并报错
    {
        cout << "Error opening file " << fileName << endl;
        exit (1);
    }

    //遍历整个文件，对读入的每个词做词频统计并筛除C++常用关键词
    while (! in.eof())
    {
        in >> buffer;
        keyword.insert(buffer);
    }

    return keyword;     //返回集合
}

/*
函数作用：用于调用上述所有函数，并的到两份代码的相似度。
主要参数：keyVector1：用于传出代码1的词频统计结果并输出；
        keyVector1：用于传出代码2的词频统计结果并输出。
返回值：查重结果即代码相似度。
*/
double DupCheck::duplicate_checking (vector <FRE> &keyVector1, vector <FRE> &keyVector2)
{
    unordered_set <string> keyword = read_keyword("keyword.txt");   //读取代码常用关键字集合

    unordered_set <string> commonSymbol = read_keyword("commonSymbol.txt");     //读取需要统计的符号集合

    //词频统计
    unordered_map <string, int> frequency1, frequency2;

    //代码1
    text_preprocessing(get_fileName1(), frequency1, commonSymbol);
    frequency_statistics(get_fileName1(), keyword, frequency1);
    keyVector1 = key_vector_extraction(frequency1);

    //代码2
    text_preprocessing(get_fileName2(), frequency2, commonSymbol);
    frequency_statistics(get_fileName2(), keyword, frequency2);
    keyVector2 = key_vector_extraction(frequency2);

    return calculate_similarity(keyVector1, keyVector2);    //返回重合率
}




