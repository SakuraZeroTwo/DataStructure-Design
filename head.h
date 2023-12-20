#pragma once
#include<iostream>
#include<algorithm>
#include<string.h>
using namespace std;

struct  frequence{//  ���������ַ�Ƶ��
    unsigned char uchar;			// ��8bitsΪ��Ԫ���޷����ַ�(0~255)
    unsigned long frequency;	   // ÿ�ࣨ�Զ����Ʊ������֣��ַ�����Ƶ��(Ƶ�ȿ��ܽϴ� ������char����)
    frequence(unsigned char x = 0, unsigned long y = 0) :uchar(x), frequency(y) {};
};
struct node {//�����������ṹ��
    unsigned char uchar;//��λ�޷����ַ�
    unsigned long  weight;//Ȩֵ(���ֵ�Ƶ�ȣ�
    char* hufcode;//����������
    int Lchild;//����
    int Rchild;//�Һ���
    int parent;//˫��
    node(int x = -1, int y = -1, int z = -1, int w = -1) :weight(x), Lchild(y), Rchild(z), parent(w) {};
};

void select(node* tree, int k, int& p1, int& p2) {
    // ��ʼ��ָ��
    p1 = -1, p2 = -1;
    // �ڻ�parent=-1�Ľ���в���Ȩֵ��С�ʹ�С���������
    for (int i = 0; i < k; ++i) {
        if (tree[i].parent != -1) continue;  // ��Ҷ�ӽ������
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
    HuffmanTree(int n, frequence* a);//���캯������������� n��Ҷ�ӽ��ĸ���  a��Ҷ�ӽ��Ȩֵ������
    ~HuffmanTree();
    void print();
    void printHuffcode();
    void huffcodeing();//����������
public:
    int n;//Ҷ�ӽ�����
    int m;//����ܸ���
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
    for (int i = n; i < m; i++)//�����������
    {
        //����ѡ��parent=-1 �Ľ����Ȩֵ��С���������
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
        cout << "���ڵ�:"<<tree[i].parent<<" "<<"���Һ���:"<<tree[i].Lchild<<","<<tree[i].Rchild<<" ��������:"<<tree[i].hufcode<<" Ȩ:"<<tree[i].weight<<" �ֽ���" <<tree[i].uchar<< endl;
}

inline void HuffmanTree::printHuffcode()
{
    for (int i = 0; i < n; i++)
        cout << " �ַ�:" << tree[i].uchar << "    ��������:" << tree[i].hufcode   << endl;
}



inline void HuffmanTree::huffcodeing()
{
    int start = -1;
    char* temp = new char[256];//���256���ַ� �������255��Ҷ�ӽ�� ���Ա��볤�����255
    temp[255] = '\0';
    for (int i = 0; i < n; i++)
    {
        start = 255;
        int pos = i;//��ǰ����λ��
        int parent = tree[i].parent;//���ڵ�λ��
        while (parent != -1)
        {
            if (tree[parent].Lchild == pos)//��ǰ���������
                temp[--start] = '0';
            else//�Һ���
                temp[--start] = '1';
            pos = parent;//����ǰλ���ƶ������ڵ�
            parent = tree[parent].parent;//���¸��ڵ�λ��
        }
        tree[i].hufcode = new char[256 - start];//Ϊ��ǰҶ�ӽ��Ĺ����붯̬���ٿռ�
        strcpy_s(tree[i].hufcode, 256 - start, &temp[start]);//����ʱ�����ֵĹ����뿽������ǰ����hufcode��
    }
    delete[]temp;
}