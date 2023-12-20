#pragma once
#include<iostream>
#include<algorithm>
#include<string.h>
using namespace std;

struct  frequence{//  用来储存字符频度
    unsigned char uchar;			// 以8bits为单元的无符号字符(0~255)
    unsigned long frequency;	   // 每类（以二进制编码区分）字符出现频度(频度可能较大 所以用char不够)
    frequence(unsigned char x = 0, unsigned long y = 0) :uchar(x), frequency(y) {};
};
struct node {//哈夫曼树结点结构体
    unsigned char uchar;//八位无符号字符
    unsigned long  weight;//权值(出现的频度）
    char* hufcode;//哈夫曼编码
    int Lchild;//左孩子
    int Rchild;//右孩子
    int parent;//双亲
    node(int x = -1, int y = -1, int z = -1, int w = -1) :weight(x), Lchild(y), Rchild(z), parent(w) {};
};

void select(node* tree, int k, int& p1, int& p2) {
    // 初始化指针
    p1 = -1, p2 = -1;
    // 在还parent=-1的结点中查找权值最小和次小的两个结点
    for (int i = 0; i < k; ++i) {
        if (tree[i].parent != -1) continue;  // 非叶子结点跳过
        if (p1 == -1 || tree[i].weight < tree[p1].weight && tree[i].weight>0) {
            p2 = p1;
            p1 = i;
        }
        else if (p2 == -1 || tree[i].weight < tree[p2].weight && tree[i].weight>0) {
            p2 = i;
        }
    }
}

class HuffmanTree {
public:
    HuffmanTree(int n, frequence* a);//构造函数构造哈夫曼树 n是叶子结点的个数  a是叶子结点权值的数组
    ~HuffmanTree();
    void print();
    void printHuffcode();
    void huffcodeing();//哈夫曼编码
public:
    int n;//叶子结点个数
    int m;//结点总个数
    node* tree;
    node* root;
    //char** Huffcode;
};
inline HuffmanTree::HuffmanTree(int n, frequence* a)
{
    this->n = n;
    this->m = 2 * n - 1;
    tree = new node[m];
    root = &tree[m - 1];
    for (int i = 0; i < n; i++)
    {
        tree[i].weight = a[i].frequency;
        tree[i].uchar = a[i].uchar;
    }
    for (int i = n; i < m; i++)//构造哈夫曼树
    {
        //首先选出parent=-1 的结点中权值最小的两个结点
        int p = -1, q = -1;
        select(tree, m, p, q);
        tree[i] = node(tree[p].weight + tree[q].weight, p, q);
        tree[p].parent = i;
        tree[q].parent = i;
    }
    huffcodeing();
}
inline HuffmanTree::~HuffmanTree()
{
    if (tree)
        delete[]tree;
}

inline void HuffmanTree::print()
{
    for (int i = 0; i < n; i++)
        cout << "父节点:"<<tree[i].parent<<" "<<"左右孩子:"<<tree[i].Lchild<<","<<tree[i].Rchild<<" 哈夫曼码:"<<tree[i].hufcode<<" 权:"<<tree[i].weight<<" 字节流" <<tree[i].uchar<< endl;
}

inline void HuffmanTree::printHuffcode()
{
    for (int i = 0; i < n; i++)
        cout << " 字符:" << tree[i].uchar << "    哈夫曼码:" << tree[i].hufcode   << endl;
}



inline void HuffmanTree::huffcodeing()
{
    int start = -1;
    char* temp = new char[256];//最多256种字符 所以最多255个叶子结点 所以编码长度最多255
    temp[255] = '\0';
    for (int i = 0; i < n; i++)
    {
        start = 255;
        int pos = i;//当前处理位置
        int parent = tree[i].parent;//父节点位置
        while (parent != -1)
        {
            if (tree[parent].Lchild == pos)//当前结点是左孩子
                temp[--start] = '0';
            else//右孩子
                temp[--start] = '1';
            pos = parent;//将当前位置移动到父节点
            parent = tree[parent].parent;//更新父节点位置
        }
        tree[i].hufcode = new char[256 - start];//为当前叶子结点的哈夫码动态开辟空间
        strcpy_s(tree[i].hufcode, 256 - start, &temp[start]);//将临时数组种的哈夫码拷贝到当前结点的hufcode中
    }
    delete[]temp;
}