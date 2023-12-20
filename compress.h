#pragma once
#include<string>
#include<fstream>//��д�ļ���ͷ�ļ�
#include"head.h"
//#define _CRT_SECURE_NO_WARNINGS // ���û�ж�����仰����ܻ��б������
//ѹ���ļ�
//ѹ����˼·
//1������һ��һ���ַ���ȡҪѹ�����ļ� ͳ���ַ��������Լ����ֵ�Ƶ�� 
//2��������ͳ�Ƶ����ݹ���������� ���й���������
//3��Ȼ����һ��һ���ַ��ر���Ҫѹ�����ļ��ҵ����������ж�Ӧ��Ҷ�ӽ��Ĺ������� Ȼ����һ���м���ַ� Ȼ��ͨ��λ��������������תΪ�������������м��ַ� ������˰�λ��д���ļ� û�оͼ�����ȡ��һ���ַ�
//4�����ڹ������еı�����ȨֵԽ��Ĺ�������Խ�� ���Ծ�ʵ�������ݵ�ѹ�� 
void compress(std::string inputFilename, std::string outputFilename)
{
    int i, j;
    unsigned int char_kind = 0;
    //�ݴ�8bits�ַ�
    unsigned char temporary_char;
    unsigned long file_length = 0;//�ļ������ַ���
    FILE* input_file, * output_file;
    frequence temporary_node;
    unsigned int number_node;
    //������뻺����
    char code_the_buffer[256] = "\0";
    unsigned int bit_count = 0;//���ڼ�¼д���ǲ����λ �Ĺ�������ĳ���
    /*
    ** ��̬����256����㣬�ݴ��ַ�Ƶ�ȣ�
    ** ͳ�Ʋ������������������ͷ�
    */
    frequence* temporary_char_frequency = (frequence*)malloc(256 * sizeof(frequence));//����ͳ�Ƶ��м侲̬����

    //��ʼ���ݴ���
    for (i = 0; i < 256; ++i)
    {
        temporary_char_frequency[i].frequency = 0;
        //�����256���±���256���ַ���Ӧ
        temporary_char_frequency[i].uchar = (unsigned char)i;
    }
    //���ȱ�����ѹ���ļ� ͳ���ַ��������ͳ��ֵ�Ƶ��
    //�Զ�����ֻ���ķ�ʽ�򿪴�ѹ���ļ�
    fopen_s(&input_file,inputFilename.c_str(), "rb");
    if (input_file == nullptr)
    {
        cout << "δ�ҵ���ѹ���ļ�" << endl;
        return;//ֱ���˳�����
    }

    //����һ���ַ�
    fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);
    while (!feof(input_file))
    {
        //ͳ���±��Ӧ�ַ���Ȩ�أ����������������ʿ���ͳ���ַ�Ƶ��
        ++temporary_char_frequency[temporary_char].frequency;
        ++file_length;
        //����һ���ַ�
        fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);
    }
    std::fclose(input_file);


    //��Ƶ��ͳ������ temporary_char_frequency����Ƶ������������
    std::sort(temporary_char_frequency, temporary_char_frequency + 256, [&](frequence a, frequence b) {
        return a.frequency > b.frequency;
    });
    // ͳ��ʵ�ʵ��ַ����ࣨ���ִ�����Ϊ0��
    for (i = 0; i < 256; ++i)
    {
        if (temporary_char_frequency[i].frequency == 0)
        {
            break;
        }
    }
    char_kind = i;
    //���ֻ��һ���ַ���ô��Ҫ���⴦�� ��Ϊ���ܽ��й��������� ��ô��ֻҪд���ַ������� ����ַ� Ȼ��д������ַ��ĸ���������
    if (char_kind == 1)
    {
        //��ѹ�������ɵ��ļ�
        fopen_s(&output_file,outputFilename.c_str(), "wb");//�Զ�����ֻд�ķ�ʽ��ѹ���ļ�
        //д���ַ�����(��Ϊһ���ַ��Ͷ����ַ��Ľ��뷽ʽ��ͬ ����Ҫд���ַ������� ���߽����ʱ��ѡ�����ķ�ʽ��
        fwrite((char*)&char_kind, sizeof(unsigned int), 1, output_file);
        //д��Ψһ���ַ�
        fwrite((char*)&temporary_char_frequency[0].uchar, sizeof(unsigned char), 1, output_file);
        //д���ַ�Ƶ�ȣ�Ҳ�����ļ�����
        fwrite((char*)&temporary_char_frequency[0].frequency, sizeof(unsigned long), 1, output_file);
        free(temporary_char_frequency);
        std::fclose(output_file);
    }
    else
    {
        HuffmanTree huffman_tree(char_kind, temporary_char_frequency);//����������� ���й���������(����һ��λ0 ��һ��Ϊ1
        //�ͷ��ַ�Ƶ��ͳ�Ƶ��ݴ���
        free(temporary_char_frequency);
        //д���ַ�����ӦȨ�أ�����ѹʱ�ؽ���������
        //��ѹ�������ɵ��ļ�
        fopen_s(&output_file,outputFilename.c_str(), "wb");//�Զ�����ֻд�ķ�ʽ��ѹ���ļ�
        //д���ַ�����
        fwrite((char*)&char_kind, sizeof(unsigned int), 1, output_file);
        for (i = 0; i < char_kind; ++i)
        {
            //д���ַ��������򣬶�����˳�򲻱䣩
            fwrite((char*)&huffman_tree.tree[i].uchar, sizeof(unsigned char), 1, output_file);
            //д���ַ���ӦȨ��
            fwrite((char*)&huffman_tree.tree[i].weight, sizeof(unsigned long), 1, output_file);
        }
        //�������ַ���Ȩ����Ϣ����д���ļ����Ⱥ��ַ�����
        //д���ļ�����
        fwrite((char*)&file_length, sizeof(unsigned long), 1, output_file);
        //�Զ�����ֻ���ķ�ʽ�򿪴�ѹ���ļ� ����ÿһ���ֽ�
        fopen_s(&input_file,inputFilename.c_str(), "rb");
        char _char = '\0';
        while (fread(&temporary_char, sizeof(unsigned char), 1, input_file) == 1)
        {
            //ƥ���������
            string code;
            for (int o = 0; o < char_kind; o++)
            {
                if (huffman_tree.tree[o].uchar == temporary_char)
                {
                    code = huffman_tree.tree[o].hufcode;
                    break;
                }
            }
            //�԰�λ��һ���ֽڣ�Ϊ��λ�����������
            for (int p = 0; p < code.length(); p++)//�������������8���ַ���ʱ��
            {
                _char <<= 1;//����һλ�ڳ�һ��bit
                if (code[p] == '1')
                    _char |= 1;

                bit_count++;
                if (bit_count == 8)//����λ
                {
                    fwrite((char*)&_char, sizeof(unsigned char), 1, output_file);//д��ѹ���ļ�
                    bit_count = 0;
                    _char = '\0';
                }
            }
        }//���ļ���ѭ��

        //��������ʣ��һ����������û��д�� �ǾͲ����λд�루���Ѿ�д���Ĺ��������Ƶ���λ��
        if (bit_count > 0 && bit_count < 8)
        {
            _char <<= (8 - bit_count);
            fwrite((char*)&_char, sizeof(unsigned char), 1, output_file);//д��ѹ���ļ�
        }
        std::fclose(input_file);//�ر��ļ�
        std::fclose(output_file);
    }
    cout<<"ѹ���ɹ�"<<endl;
}