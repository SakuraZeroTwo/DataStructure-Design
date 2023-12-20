#pragma once
#include"head.h"
#include<string>
#include<fstream>
//哈夫曼解码解压缩文件
void decompress(string inputFilename, string outputFilename)
{
    unsigned int i;
    unsigned long file_length;
    //控制文件写入长度
    unsigned long file_write_length = 0;
    FILE* input_file, * output_file;
    //存储字符种类
    unsigned int char_kind;
    unsigned int number_node;
    //暂存8bits编码
    unsigned char temporary_char;
    //保存根节点索引，供匹配编码使用
    unsigned int root;

    //以二进制方式打开压缩文件
    fopen_s(&input_file,inputFilename.c_str(), "rb");

    if (input_file == nullptr)
    {
        cout<<"未找到相应文件或文件格式有误"<<endl;
        return;
    }

    //读取压缩文件前端的字符及对应编码，用于重建哈夫曼树
    //读取字符种类数
    fread((char*)&char_kind, sizeof(unsigned int), 1, input_file);
    number_node = 2 * char_kind - 1;
    if (char_kind == 1)
    {
        //读取唯一的字符
        fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);
        //读取文件长度
        fread((char*)&file_length, sizeof(unsigned long), 1, input_file);
        //打开压缩后将生成的文件
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
            //读入字符
            fread((char*)&frequencearry[i].uchar, sizeof(unsigned char), 1, input_file);
            //读入字符对应权重
            fread((char*)&frequencearry[i].frequency, sizeof(unsigned long), 1, input_file);
        }
        //重构哈夫曼树
        HuffmanTree huffman_tree(char_kind, frequencearry);
        cout << "重建后的哈夫曼树:" << endl;
        huffman_tree.print();
        cout << "-----------------------------------------------------" << endl;
        //读完字符和权重信息，紧接着读取文件长度和编码，进行解码
        //读入文件长度
        fread((char*)&file_length, sizeof(unsigned long), 1, input_file);
        //打开压缩后将生成的文件
        fopen_s(&output_file,outputFilename.c_str(), "wb");
        root = number_node - 1;
        while (1)
        {
            //读取一个字符长度的编码（8位）
            fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);

            //处理读取的一个字符长度的编码
            for (i = 0; i < 8; ++i)
            {
                //由根向下直至叶节点正向匹配编码对应字符
                if (temporary_char & 128)//0 向左走
                {
                    root = huffman_tree.tree[root].Rchild;
                }
                else//1 向右走
                {
                    root = huffman_tree.tree[root].Lchild;
                }

                if (root < char_kind)//走到了叶子结点
                {
                    fwrite((char*)&huffman_tree.tree[root].uchar, sizeof(unsigned char), 1, output_file);
                    //cout << "写入的第" << file_write_length +1<< "个字符为:" << huffman_tree.tree[root].uchar << endl;
                    ++file_write_length;
                    //控制文件长度，跳出内层循环
                    if (file_write_length == file_length)
                    {
                        break;
                    }
                    //复位为根索引，匹配下一个字符
                    root = number_node - 1;
                }
                //将编码缓存的下一位移到最高位，供匹配
                temporary_char <<= 1;
            }
            //控制文件长度，跳出外层循环
            if (file_write_length >=file_length)
            {
                break;
            }
        }

        //关闭文件
        std::fclose(input_file);
        std::fclose(output_file);
    }
    cout << "解压缩成功" << endl;
}