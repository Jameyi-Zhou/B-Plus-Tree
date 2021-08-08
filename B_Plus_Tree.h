#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H
#include<string>

using namespace std;
struct B_Plus_Tree_Node{
    int key_num; //��ǰ��ֵ�ĸ���
    bool is_leaf;
    int* key; //�洢��ֵ������
    B_Plus_Tree_Node** child; //���ӽڵ�
    B_Plus_Tree_Node* parent; //˫�׽ڵ�
    string info;
    B_Plus_Tree_Node(){
        key_num = 0;
        is_leaf = false;
        key = nullptr;
        child = nullptr;
        parent = nullptr;
        info = "";
    }
};

struct Data{ //�ṹ�����ڴ洢������Ϣ
    int key;
    string info;
};

class B_Plus_Tree{
private:
    int  height;
    int order; //B+���Ľ���
    B_Plus_Tree_Node* root; //root�ڵ�ʼ��ָ��B+���ĸ��ڵ�
    B_Plus_Tree_Node* first; //Ҷ�ӽڵ�洢Ϊһ��˫�������ýڵ�Ϊͷ���
    B_Plus_Tree_Node* last_leaf; //�ýڵ�Ϊ˫�������β���
public:

    B_Plus_Tree(int order){
        this->order = order;
        this->height = 1; //�߶�Ϊ1����ʾB+��û���м�ڵ�
        root = new B_Plus_Tree_Node; //����һ�����ڵ�
        root->key = new int[order]; //���ݽ���ȷ����ֵ���������
        root->child = new B_Plus_Tree_Node*[order+1]; //���ݽ���ȷ���ӽڵ���������
        for(int i = 0; i < order+1; i++)
            root->child[i] = nullptr;
        for(int i = 0; i < order; i++)
            root->key[i] = -1;
        this->first = new B_Plus_Tree_Node; //����Ҷ�ڵ��˫������ͷ��㣬��Я���κ�����
        this->first->child = new B_Plus_Tree_Node*[2];
        this->first->child[0] = this->first->child[1] = nullptr; //child[1]ָ����һ��Ҷ�ӽڵ�
        this->last_leaf = this->first;
    }
    int search_aux(int key, B_Plus_Tree_Node*& final_innode); // �������Ҹ�������
    bool search(int key, Data& data); //��������
    void insert_1(B_Plus_Tree_Node*& node, Data data, int pos); //ֱ�Ӳ��룬���÷���
    void insert_2(B_Plus_Tree_Node*& node, Data data, int pos); //��Ҫ���ѵĲ���
    bool insertNode(Data data); //��������
    bool deleteNode(int key); //����ɾ��
    bool search(int key1, int key2, Data*& data);//��������
    bool insertNode(Data* data); //��������
    bool deleteNode(int key1, int key2); //����ɾ��
    bool store(char* filename);//���л��������ļ�
    bool load(char* filename); //���ļ���ȡ��B+��
    void printLeaf() {
        for(B_Plus_Tree_Node *p = this->first->child[1]; p; p = p->child[1])
            cout << *p->key << " " << p->info << endl;
    }
    void printRoot(){
        for(int i = 0; i < this->root->key_num; ++i)
            cout << this->root->key[i] << " ";
        cout << endl;
        for(int i = 0; i <= this->root->key_num; ++i) {
            if(this->root->child[i])
                cout << *(this->root->child[i]->key) << " ";
        }
        cout << endl;
    }
};

#endif //B_PLUS_TREE_H