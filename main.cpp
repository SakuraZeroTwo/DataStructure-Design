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
    cout << "------------------0���˳�����--------------------------" << endl;
    cout << "------------------1��չʾԤ���ַ���Ƶ�ȵĹ���������----" << endl;
    cout << "------------------2���ֶ������ַ����ַ�Ƶ�Ƚ��б���----" << endl;
    cout << "------------------3��ѹ���ı��ļ�----------------------" << endl;
    cout << "------------------4����ѹ���ı��ļ�--------------------" << endl;
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
        cout << "��ѡ����" << endl;
        cin >> n;
        switch (n)
        {
            case 0:
            {
                cout << "��ӭ�´�ʹ��!!!" << endl;
                return 0;
            }
            case 1:
            {
                cout << "��Ԥ���ַ���Ƶ�Ƚ��й���������֮����Ϊ:" << endl;
                cout << "---------------------------------------------------" << endl;
                huftree.printHuffcode();
                cout << "---------------------------------------------------" << endl;
                system("pause");
                break;
            }
            case 2:
            {
                int num = 0;
                cout << "�������ַ�����" << endl;
                cin >> num;
                char uchar;
                int frequ = 0;
                frequence* freq = new frequence[num];
                for (int i = 0; i < num; i++)
                {
                    cout << "�������ַ���Ƶ��" << endl;
                    cin >> uchar >> frequ;
                    freq[i] = frequence(uchar, frequ);

                }
                HuffmanTree tr(num, freq);
                cout << "������Ϊ:" << endl; cout << "��Ԥ���ַ���Ƶ�Ƚ��й���������֮����Ϊ:" << endl;
                cout << "---------------------------------------------------" << endl;
                tr.printHuffcode();
                cout << "---------------------------------------------------" << endl;
                system("pause");
                break;
            }
            case 3:
            {
                string inputFileName, outputFileName;
                cout << "�������ѹ�����ļ��ĵ�ַ��";
                cin >> inputFileName;
                cout << "������ѹ��֮����ļ���ַ:";
                cin >> outputFileName;
                compress(inputFileName, outputFileName);
                system("pause");
                break;
            }
            case 4:
            {
                string inputFileName, outputFileName;
                cout << "���������ѹ�����ļ��ĵ�ַ��";
                cin >> inputFileName;
                cout << "�������ѹ��֮����ļ���ַ:";
                cin >> outputFileName;
                decompress(inputFileName, outputFileName);
                system("pause");
                break;
            }
            default:
            {
                cout << "��������!��������������룡" << endl;
                system("pause");
                break;
            }
        }//switch
        system("cls");
    }//while

    return 0;
}