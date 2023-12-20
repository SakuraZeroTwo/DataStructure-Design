#include <iostream>
#include <algorithm>
#include "head.h"
#include "compress.h"
#include "decompress.h"
#include<string>
using namespace std;

void showMenu()
{
    cout << "-------------------------------------------------------" << endl;
    cout << "------------------0、退出程序--------------------------" << endl;
    cout << "------------------1、展示预定字符及频度的哈夫曼编码----" << endl;
    cout << "------------------2、手动输入字符及字符频度进行编码----" << endl;
    cout << "------------------3、压缩文本文件----------------------" << endl;
    cout << "------------------4、解压缩文本文件--------------------" << endl;
    cout << "-------------------------------------------------------" << endl;
}

int main()
{
    frequence fre[27]{ frequence(' ',186),frequence('a',64), frequence('b',13), frequence('c',22),
                       frequence('d',32), frequence('e',103), frequence('f',21), frequence('g',15), frequence('h',47),
                       frequence('i',57), frequence('j',1), frequence('k',5), frequence('l',32), frequence('m',20),
                       frequence('n',57), frequence('o',63), frequence('p',15), frequence('q',1), frequence('r',48),
                       frequence('s',51), frequence('t',80), frequence('u',23), frequence('v',8), frequence('w',18),
                       frequence('x',1), frequence('y',16), frequence('z',1) };
    HuffmanTree huftree(27, fre);
    int n = 0;
    while (1)
    {
        showMenu();
        cout << "请选择功能" << endl;
        cin >> n;
        switch (n)
        {
            case 0:
            {
                cout << "欢迎下次使用!!!" << endl;
                return 0;
            }
            case 1:
            {
                cout << "对预定字符及频度进行哈夫曼编码之后结果为:" << endl;
                cout << "---------------------------------------------------" << endl;
                huftree.printHuffcode();
                cout << "---------------------------------------------------" << endl;
                system("pause");
                break;
            }
            case 2:
            {
                int num = 0;
                cout << "请输入字符种数" << endl;
                cin >> num;
                char uchar;
                int frequ = 0;
                frequence* freq = new frequence[num];
                for (int i = 0; i < num; i++)
                {
                    cout << "请输入字符和频度" << endl;
                    cin >> uchar >> frequ;
                    freq[i] = frequence(uchar, frequ);

                }
                HuffmanTree tr(num, freq);
                cout << "编码结果为:" << endl; cout << "对预定字符及频度进行哈夫曼编码之后结果为:" << endl;
                cout << "---------------------------------------------------" << endl;
                tr.printHuffcode();
                cout << "---------------------------------------------------" << endl;
                system("pause");
                break;
            }
            case 3:
            {
                string inputFileName, outputFileName;
                cout << "请输入待压缩的文件的地址：";
                cin >> inputFileName;
                cout << "请输入压缩之后的文件地址:";
                cin >> outputFileName;
                compress(inputFileName, outputFileName);
                system("pause");
                break;
            }
            case 4:
            {
                string inputFileName, outputFileName;
                cout << "请输入待解压缩的文件的地址：";
                cin >> inputFileName;
                cout << "请输入解压缩之后的文件地址:";
                cin >> outputFileName;
                decompress(inputFileName, outputFileName);
                system("pause");
                break;
            }
            default:
            {
                cout << "输入有误!按任意键重新输入！" << endl;
                system("pause");
                break;
            }
        }//switch
        system("cls");
    }//while

    return 0;
}