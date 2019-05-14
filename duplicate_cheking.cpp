
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

/*�������ã��ļ�Ԥ�����������滻�ɿո񣬲���������Ƶͳ�ơ�
��Ҫ������fileName�� ��ҪԤ������ļ����ļ�����
        frequency�� ��������ͳ�Ƶķ��ŵĳ���Ƶ����
        commonSymbol: ��Ҫ����ͳ�Ƶķ��ż��ϣ��ɺ���read_keyword���롣
����ֵ����
*/
void DupCheck::text_preprocessing(string fileName, unordered_map <string, int>& frequency,
                        unordered_set <string> commonSymbol)
{
    fstream file;   //�����ļ����ͱ���
    char buffer[2];     //�����ݴ浱ǰ������ַ������洢Ϊ�ַ�����
    buffer[1] = '\0';   //����ַ���������־
    file.open(fileName, ios::in | ios::out | ios::binary);  //���ļ���������Ϊ�ɶ���д�����������
    if (! file.is_open())     //�ļ���ʧ�����˳�������
    {
        cout << "Error opening file " << fileName << endl;
        exit (1);
    }

    //�����ļ�
    while(file.peek() != EOF)   //�����ļ�ĩδ����
    {
        file.get(buffer[0]);    //��ȡһ���ַ�
        if (! (buffer[0] >= 'A' && buffer[0] <= 'Z') && ! (buffer[0] >= '0' && buffer[0] <= '9')
            && ! (buffer[0] >= 'a' && buffer[0] <= 'z'))    //�ж��Ƿ�Ϊ����
        {
            file.seekg(-1, ios::cur);   //�ļ���ʶ��ǰ��һλ�������ַ��޸�Ϊ�ո�
            file<<' ';
        }
        string preSymbol(buffer);   //תΪstring���ͱ���
        if (commonSymbol.count(preSymbol))  //�������Ҫͳ�Ƶķ���
        {
            //���д�Ƶͳ��
            if (frequency.count(preSymbol))
                frequency[preSymbol]++;
            else
                frequency.insert(pair <string, int> (preSymbol, 1));
        }
    }

    file.close();   //�ر��ļ�
}

/*
�������ã��ļ����룬��ʶ��룬�������δ�Ƶͳ�ơ�
��Ҫ������fileName�� ��Ҫ�����δ�Ƶͳ�Ƶ��ļ����ļ�����
        keyword�� ��Ҫ���Եĳ��ùؼ��ּ��ϣ��ɺ���read_keyword���룻
        frequency�� ��������ͳ�Ƶķ��ŵĳ���Ƶ����
����ֵ����
*/
void DupCheck::frequency_statistics (string fileName, unordered_set <string> keyword,
                                                  unordered_map <string, int>& frequency)
{
    string buffer;  //�ݴ�����һ���ַ���
    ifstream in(fileName);    //���ļ�
    if (! in.is_open())     //�ļ���ʧ�����˳�������
    {
        cout << "Error opening file " << fileName << endl;
        exit (1);
    }

    //���������ļ����Զ����ÿ��������Ƶͳ�Ʋ�ɸ��C++���ùؼ���
    while (in.peek() != EOF)
    {
        in >> buffer;
        //ɸ��C++���ùؼ���
        if (keyword.count(buffer))
            continue;

        //��Ƶͳ��
        if (frequency.count(buffer))
            frequency[buffer]++;
        else
            frequency.insert(pair <string, int> (buffer, 1));

    }

    in.close(); //�ر��ļ�
}

/*
�������ã��������ϲ���õ��ĵ��ʳ���Ƶ����ת����Ƶ�ʺ󣬴Ӵ�С����
��Ҫ������freq�� ͳ�ƺõĵ��ʳ���Ƶ����
����ֵ��Ƶ��������
*/
vector <FRE> DupCheck::key_vector_extraction (unordered_map <string, int> freq)
{
    unordered_map <string, int>::iterator it;   //����fre�ĵ�����
    vector <FRE> keyVector;     //��ȡ���Ĵ�Ƶ��������

    //����ͳ�Ƶ��ܴ���
    int sum = 0;
    for (it = freq.begin(); it != freq.end(); it++)
    {
        sum += it->second;
    }

    //����Ƶ��
    for (it = freq.begin(); it != freq.end(); it++)
    {
        keyVector.push_back(FRE (it->first, it->second * 1.0 / sum));
    }

    //����
    sort(keyVector.begin(), keyVector.end());

    return keyVector;   //����
}

/*
�������ã��������������õ����ݴ���Ĵ�Ƶ������������������������ƶȡ�
��Ҫ������
code1�� ��һ�ݴ���Ĵ�Ƶ��������
code2�� �ڶ��ݴ���Ĵ�Ƶ��������
����ֵ�� �������������ƶ�
 */
double DupCheck::calculate_similarity (vector <FRE> code1, vector <FRE> code2)
{
    vector <FRE>::iterator it1, it2;    //���������ĵ�����
    double product = 0.0, quadraticSum1 = 0.0, quadraticSum2 = 0.0;     //���ڴ�ȡ�ڻ���������ģ

    //�����ڻ���������ģ
    for (it1 = code1.begin(), it2 = code2.begin(); it1 != code1.end()
            && it2 != code2.end(); it1++, it2++)
    {
        product += (it1->fre) * (it2->fre);
        quadraticSum1 += (it1->fre) * (it1->fre);
        quadraticSum2 += (it2->fre) * (it2->fre);
    }

    //�������ƶ�
    return (product / (sqrt(quadraticSum1) * sqrt(quadraticSum2)) - 0.95) / 0.05;
}

/*
�������ã����ùؼ��ֺ���ͳ�Ʒ��Ŵ洢���ļ����У���Ҫʱͨ���˺�����ȡ��
����˵����filename���ļ�����
����ֵ���ؼ��ֻ���ż��ϡ�
*/
unordered_set <string> DupCheck::read_keyword(string fileName)
{
    unordered_set <string> keyword;
    string buffer;  //�ݴ�����һ���ַ���
    ifstream in(fileName);  //���ļ�
    if (! in.is_open())     //�ļ���ʧ�����˳�������
    {
        cout << "Error opening file " << fileName << endl;
        exit (1);
    }

    //���������ļ����Զ����ÿ��������Ƶͳ�Ʋ�ɸ��C++���ùؼ���
    while (! in.eof())
    {
        in >> buffer;
        keyword.insert(buffer);
    }

    return keyword;     //���ؼ���
}

/*
�������ã����ڵ����������к��������ĵ����ݴ�������ƶȡ�
��Ҫ������keyVector1�����ڴ�������1�Ĵ�Ƶͳ�ƽ���������
        keyVector1�����ڴ�������2�Ĵ�Ƶͳ�ƽ���������
����ֵ�����ؽ�����������ƶȡ�
*/
double DupCheck::duplicate_checking (vector <FRE> &keyVector1, vector <FRE> &keyVector2)
{
    unordered_set <string> keyword = read_keyword("keyword.txt");   //��ȡ���볣�ùؼ��ּ���

    unordered_set <string> commonSymbol = read_keyword("commonSymbol.txt");     //��ȡ��Ҫͳ�Ƶķ��ż���

    //��Ƶͳ��
    unordered_map <string, int> frequency1, frequency2;

    //����1
    text_preprocessing(get_fileName1(), frequency1, commonSymbol);
    frequency_statistics(get_fileName1(), keyword, frequency1);
    keyVector1 = key_vector_extraction(frequency1);

    //����2
    text_preprocessing(get_fileName2(), frequency2, commonSymbol);
    frequency_statistics(get_fileName2(), keyword, frequency2);
    keyVector2 = key_vector_extraction(frequency2);

    return calculate_similarity(keyVector1, keyVector2);    //�����غ���
}




