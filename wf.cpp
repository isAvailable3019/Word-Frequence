#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <io.h>

using namespace std;

map<string, int> wdmap;

bool cmp(pair<string, int> a, pair<string, int> b)
{
    return a.second > b.second;
}

void funC()
{
    ifstream myfile("E:\\other\\wf\\wf.txt");
    ofstream outfile("E:\\other\\wf\\wf.txt", ios::app);
    if (!myfile.is_open())
    {
        cout << "未成功打开文件" << endl;
        return;
    }
    string temp;
    string str = "";
    while (myfile >> temp)
    {
        str += temp;
    }
    myfile.close();
    char *word = (char *)str.data();
    int num = 0;
    int a[26][2];
    for (int i = 0; i < 26; i++)
    {
        a[i][0] = 97 + i;
        a[i][1] = 0;
    }
    for (int i = 0; i < str.length(); i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
            word[i] += 32;
        if (word[i] >= 'a' && word[i] <= 'z')
        {
            num++;
            a[(int)word[i] - 97][1]++;
        }
    }
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 25 - i; j++)
        {
            if (a[j][1] < a[j + 1][1])
            {
                int tmp1, tmp2;
                tmp1 = a[j + 1][1];
                tmp2 = a[j + 1][0];
                a[j + 1][0] = a[j][0];
                a[j + 1][1] = a[j][1];
                a[j][1] = tmp1;
                a[j][0] = tmp2;
            }
        }
    }
    for (int i = 0; i < 26; i++)
    {
        cout << (char)a[i][0] << "出现频率：" << setiosflags(ios::fixed) << setprecision(2) << ((float)a[i][1] / num) * 100 << "%" << endl;
    }
}

void funF(string file)
{
    ifstream myfile(file);
    ofstream outfile(file, ios::app);
    if (!myfile.is_open())
    {
        cout << "未成功打开文件" + file << endl;
        return;
    }
    string temp, temp2;
    while (myfile >> temp)
    {
        temp2 = "";
        string::iterator iter;
        for (iter = temp.begin(); iter != temp.end(); iter++)
        {
            if ((*iter >= 'A' && *iter <= 'Z') || (*iter >= 'a' & *iter <= 'z'))
            {
                temp2 += *iter < 'a' ? *iter + 32 : *iter;
            }
        }
        if (temp2 != "")
        {
            if (wdmap.count(temp2) == 0)
                wdmap.insert(pair<string, int>(temp2, 1));
            else
                wdmap[temp2]++;
        }
    }
}

void funD(string arg)
{
    const char *dir = const_cast<char *>((arg + "\\*.txt").c_str());
    _finddata64i32_t fileInfo;
    intptr_t hFile = _findfirst("E:\\other\\wf\\*.txt", &fileInfo);
    if (hFile != -1)
    {
        do
        {
            funF((string)arg + "\\" + (string)fileInfo.name);
        } while (_findnext(hFile, &fileInfo) == 0);
    }
    else
    {
        cout << arg << "文件夹不存在文本文件";
    }
}

void funS(string path, int layer)
{
    _finddata_t file_info;
    string current_path = path + "/*.*";
    int handle = _findfirst(current_path.c_str(), &file_info);
    //返回值为-1则查找失败
    if (-1 == handle)
    {
        cout << "未成功打开文件夹" << endl;
        return;
    }
    do
    {
        if (file_info.attrib == _A_SUBDIR)
        {
            int layer_tmp = layer;
            if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0){
                funD(path + "\\" + file_info.name);
                funS(path + '/' + file_info.name, layer_tmp + 1);
            }
        }
    } while (!_findnext(handle, &file_info)); //返回0则遍历完
    //关闭文件句柄
    _findclose(handle);
}

void showMap(int num)
{
    int i = 0;
    vector<pair<string, int>> vec;
    for (map<string, int>::iterator it = wdmap.begin(); it != wdmap.end(); it++)
    {
        vec.push_back(pair<string, int>(it->first, it->second));
    }
    sort(vec.begin(), vec.end(), cmp);
    for (vector<pair<string, int>>::iterator iter = vec.begin(); iter != vec.end(); iter++)
    {
        cout << iter->first << "出现次数" << iter->second << endl;
        i++;
        if (num != 0 && i >= num)
            break;
    }
}

int main(int arg, char *args[])
{
    int num = 0;
    for (int i = 0; i < arg; i++)
    {
        string cmd = args[i];
        if (cmd == "-c")
        {
            funC();
        }
        if (cmd == "-f")
        {
            funF("E:\\other\\wf\\wf.txt");
        }
        if (cmd == "-d")
        {
            if (i + 1 < arg)
            {
                funD(args[i + 1]);
                if (i + 2 < arg && (string)args[i + 2] == "-s")
                {
                    funS(args[i + 1], 0);
                    i += 1;
                }
                i += 1;
            }            
            else
                cout << "-d无参数";
        }
        if (cmd == "-n")
        {
            try
            {
                num = stoi(args[i + 1]);
                if (num < 0)
                {
                    cout << "-n有错误参数";
                    num = 0;
                }
            }
            catch (invalid_argument &)
            {
                cout << "-n有错误参数";
            }
        }
    }
    showMap(num);
    return 0;
}