#pragma once
#include<string>
#include<fstream>//读写文件的头文件
#include"head.h"
//#define _CRT_SECURE_NO_WARNINGS // 如果没有定义这句话会可能会有编译错误
//压缩文件
//压缩的思路
//1、首先一个一个字符读取要压缩的文件 统计字符的种数以及出现的频度 
//2、用上面统计的数据构造哈夫曼树 进行哈夫曼编码
//3、然后再一个一个字符地遍历要压缩的文件找到哈夫曼树中对应的叶子结点的哈夫曼码 然后用一个中间空字符 然后通过位操作将哈夫曼码转为二进制数存入中间字符 如果满了八位就写入文件 没有就继续读取下一个字符
//4、由于哈夫曼中的编码是权值越大的哈夫曼码越短 所以就实现了数据的压缩 
void compress(std::string inputFilename, std::string outputFilename)
{
    int i, j;
    unsigned int char_kind = 0;
    //暂存8bits字符
    unsigned char temporary_char;
    unsigned long file_length = 0;//文件的总字符数
    FILE* input_file, * output_file;
    frequence temporary_node;
    unsigned int number_node;
    //待存编码缓冲区
    char code_the_buffer[256] = "\0";
    unsigned int bit_count = 0;//用于记录写入是不足八位 的哈夫曼码的长度
    /*
    ** 动态分配256个结点，暂存字符频度，
    ** 统计并拷贝到树结点后立即释放
    */
    frequence* temporary_char_frequency = (frequence*)malloc(256 * sizeof(frequence));//用于统计的中间静态数组

    //初始化暂存结点
    for (i = 0; i < 256; ++i)
    {
        temporary_char_frequency[i].frequency = 0;
        //数组的256个下标与256种字符对应
        temporary_char_frequency[i].uchar = (unsigned char)i;
    }
    //首先遍历待压缩文件 统计字符的种数和出现的频度
    //以二进制只读的方式打开待压缩文件
    fopen_s(&input_file,inputFilename.c_str(), "rb");
    if (input_file == nullptr)
    {
        cout << "未找到待压缩文件" << endl;
        return;//直接退出函数
    }

    //读入一个字符
    fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);
    while (!feof(input_file))
    {
        //统计下标对应字符的权重，利用数组的随机访问快速统计字符频度
        ++temporary_char_frequency[temporary_char].frequency;
        ++file_length;
        //读入一个字符
        fread((char*)&temporary_char, sizeof(unsigned char), 1, input_file);
    }
    std::fclose(input_file);


    //对频度统计数组 temporary_char_frequency按照频度做降序排序
    std::sort(temporary_char_frequency, temporary_char_frequency + 256, [&](frequence a, frequence b) {
        return a.frequency > b.frequency;
    });
    // 统计实际的字符种类（出现次数不为0）
    for (i = 0; i < 256; ++i)
    {
        if (temporary_char_frequency[i].frequency == 0)
        {
            break;
        }
    }
    char_kind = i;
    //如果只有一种字符那么就要特殊处理 因为不能进行哈夫曼编码 那么就只要写入字符的种数 这个字符 然后写入这个字符的个数就行了
    if (char_kind == 1)
    {
        //打开压缩后将生成的文件
        fopen_s(&output_file,outputFilename.c_str(), "wb");//以二进制只写的方式打开压缩文件
        //写入字符种数(因为一种字符和多种字符的解码方式不同 所以要写入字符的种数 方边解码的时候选择解码的方式）
        fwrite((char*)&char_kind, sizeof(unsigned int), 1, output_file);
        //写入唯一的字符
        fwrite((char*)&temporary_char_frequency[0].uchar, sizeof(unsigned char), 1, output_file);
        //写入字符频度，也就是文件长度
        fwrite((char*)&temporary_char_frequency[0].frequency, sizeof(unsigned long), 1, output_file);
        free(temporary_char_frequency);
        std::fclose(output_file);
    }
    else
    {
        HuffmanTree huffman_tree(char_kind, temporary_char_frequency);//构造哈夫曼树 进行哈夫曼编码(向左一步位0 有一步为1
        //释放字符频度统计的暂存区
        free(temporary_char_frequency);
        //写入字符和相应权重，供解压时重建哈夫曼树
        //打开压缩后将生成的文件
        fopen_s(&output_file,outputFilename.c_str(), "wb");//以二进制只写的方式打开压缩文件
        //写入字符种类
        fwrite((char*)&char_kind, sizeof(unsigned int), 1, output_file);
        for (i = 0; i < char_kind; ++i)
        {
            //写入字符（已排序，读出后顺序不变）
            fwrite((char*)&huffman_tree.tree[i].uchar, sizeof(unsigned char), 1, output_file);
            //写入字符对应权重
            fwrite((char*)&huffman_tree.tree[i].weight, sizeof(unsigned long), 1, output_file);
        }
        //紧接着字符和权重信息后面写入文件长度和字符编码
        //写入文件长度
        fwrite((char*)&file_length, sizeof(unsigned long), 1, output_file);
        //以二进制只读的方式打开待压缩文件 遍历每一个字节
        fopen_s(&input_file,inputFilename.c_str(), "rb");
        char _char = '\0';
        while (fread(&temporary_char, sizeof(unsigned char), 1, input_file) == 1)
        {
            //匹配哈夫曼码
            string code;
            for (int o = 0; o < char_kind; o++)
            {
                if (huffman_tree.tree[o].uchar == temporary_char)
                {
                    code = huffman_tree.tree[o].hufcode;
                    break;
                }
            }
            //以八位（一个字节）为单位处理哈夫曼码
            for (int p = 0; p < code.length(); p++)//当哈夫曼码大于8个字符的时候
            {
                _char <<= 1;//左移一位腾出一个bit
                if (code[p] == '1')
                    _char |= 1;

                bit_count++;
                if (bit_count == 8)//满八位
                {
                    fwrite((char*)&_char, sizeof(unsigned char), 1, output_file);//写入压缩文件
                    bit_count = 0;
                    _char = '\0';
                }
            }
        }//读文件的循环

        //最后如果还剩下一个哈夫曼码没有写入 那就补齐八位写入（将已经写进的哈夫曼码移到高位）
        if (bit_count > 0 && bit_count < 8)
        {
            _char <<= (8 - bit_count);
            fwrite((char*)&_char, sizeof(unsigned char), 1, output_file);//写入压缩文件
        }
        std::fclose(input_file);//关闭文件
        std::fclose(output_file);
    }
    cout<<"压缩成功"<<endl;
}