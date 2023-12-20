#pragma once
#include"head.h"
#include<string>
#include<fstream>
//�����������ѹ���ļ�
void decompress(string inputFilename, string outputFilename)
{
    unsigned int i;
    unsigned long file_length;
    //�����ļ�д�볤��
    unsigned long file_write_length = 0;
    FILE* input_file, * output_file;
    //�洢�ַ�����
    unsigned int char_kind;
    unsigned int number_node;
    //�ݴ�8bits����
    unsigned char temporary_char;
    //������ڵ���������ƥ�����ʹ��
    unsigned int root;

    //�Զ����Ʒ�ʽ��ѹ���ļ�
    fopen_s(&input_file,inputFilename.c_str(), "rb");

    if (input_file == nullptr)
    {
        cout<<"δ�ҵ���Ӧ�ļ����ļ���ʽ����"<<endl;
        return;
    }

    //��ȡѹ���ļ�ǰ�˵��ַ�����Ӧ���룬�����ؽ���������
    //��ȡ�ַ�������
    fread((char*)&char_kind, sizeof(unsigned int), 1, input_file);
    number_node = 2 * char_kind - 1;
    if (char_kind == 1)
    {
        //��ȡΨһ���ַ�
        fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);
        //��ȡ�ļ�����
        fread((char*)&file_length, sizeof(unsigned long), 1, input_file);
        //��ѹ�������ɵ��ļ�
        fopen_s(&output_file,outputFilename.c_str(), "wb");
        while (file_length--)
        {
            fwrite((char*)&temporary_char, sizeof(unsigned char), 1, output_file);
        }
        std::fclose(input_file);
        std::fclose(output_file);
    }
    else
    {
        frequence* frequencearry = new frequence[char_kind];
        for (i = 0; i < char_kind; ++i)
        {
            //�����ַ�
            fread((char*)&frequencearry[i].uchar, sizeof(unsigned char), 1, input_file);
            //�����ַ���ӦȨ��
            fread((char*)&frequencearry[i].frequency, sizeof(unsigned long), 1, input_file);
        }
        //�ع���������
        HuffmanTree huffman_tree(char_kind, frequencearry);
        cout << "�ؽ���Ĺ�������:" << endl;
        huffman_tree.print();
        cout << "-----------------------------------------------------" << endl;
        //�����ַ���Ȩ����Ϣ�������Ŷ�ȡ�ļ����Ⱥͱ��룬���н���
        //�����ļ�����
        fread((char*)&file_length, sizeof(unsigned long), 1, input_file);
        //��ѹ�������ɵ��ļ�
        fopen_s(&output_file,outputFilename.c_str(), "wb");
        root = number_node - 1;
        while (1)
        {
            //��ȡһ���ַ����ȵı��루8λ��
            fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);

            //�����ȡ��һ���ַ����ȵı���
            for (i = 0; i < 8; ++i)
            {
                //�ɸ�����ֱ��Ҷ�ڵ�����ƥ������Ӧ�ַ�
                if (temporary_char & 128)//0 ������
                {
                    root = huffman_tree.tree[root].Rchild;
                }
                else//1 ������
                {
                    root = huffman_tree.tree[root].Lchild;
                }

                if (root < char_kind)//�ߵ���Ҷ�ӽ��
                {
                    fwrite((char*)&huffman_tree.tree[root].uchar, sizeof(unsigned char), 1, output_file);
                    //cout << "д��ĵ�" << file_write_length +1<< "���ַ�Ϊ:" << huffman_tree.tree[root].uchar << endl;
                    ++file_write_length;
                    //�����ļ����ȣ������ڲ�ѭ��
                    if (file_write_length == file_length)
                    {
                        break;
                    }
                    //��λΪ��������ƥ����һ���ַ�
                    root = number_node - 1;
                }
                //�����뻺�����һλ�Ƶ����λ����ƥ��
                temporary_char <<= 1;
            }
            //�����ļ����ȣ��������ѭ��
            if (file_write_length >=file_length)
            {
                break;
            }
        }

        //�ر��ļ�
        std::fclose(input_file);
        std::fclose(output_file);
    }
    cout << "��ѹ���ɹ�" << endl;
}