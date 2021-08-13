#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H
#include<string>

struct Data{ //�ṹ�����ڴ洢������Ϣ
    int key;
    std::string info;
};

struct B_Plus_Tree_Node{
    int key_num; //��ǰ��ֵ�ĸ���
    bool is_leaf; //����Ƿ�ΪҶ�ӽڵ�
    int* key; //�洢��ֵ������
    B_Plus_Tree_Node** child; //���ӽڵ�ָ������
    B_Plus_Tree_Node* parent; //˫�׽ڵ�
    std::string info; //Ҷ�ӽڵ���Ϣ
    B_Plus_Tree_Node(){
        key_num = 0;
        is_leaf = false;
        key = nullptr;
        child = nullptr;
        parent = nullptr;
        info = "";
    }
};

class B_Plus_Tree{
private:
    int order; //B+���Ľ���
    int height; //B+���ĸ߶�
    int node_counter; //B+���ķ�Ҷ������
    int leaf_counter; //B+����Ҷ�Ӹ���(�ж��ٹؼ���)
    B_Plus_Tree_Node* root; //root�ڵ�ʼ��ָ��B+���ĸ��ڵ�
    B_Plus_Tree_Node* head; //Ҷ�ӽڵ�洢Ϊһ��˫�������ýڵ�Ϊͷ��㣬��Я���κ�����
    B_Plus_Tree_Node* rear; //�ýڵ�Ϊ˫�������β��㣬Ҳ��Я���κ�����
public:
    B_Plus_Tree();
    B_Plus_Tree(int order);
    int search_aux(int key, B_Plus_Tree_Node*& final_innode); // �������Ҹ�������
    bool search(int key, Data& data); //�������Һ���
    void insert_1(B_Plus_Tree_Node*& node, Data data, int pos); //ֱ�Ӳ��룬���÷���
    void insert_2(B_Plus_Tree_Node*& node, Data data, int pos); //��Ҫ���ѵĲ���
    bool insertNode(Data data); //��������
    bool deleteNode(int key); //����ɾ��
    //bool search(int key1, int key2, Data*& data);//��������
    //bool insertNode(Data* data); //��������
    //bool deleteNode(int key1, int key2); //����ɾ��
    bool store(char* filename);//���л��������ļ�
    bool load(char* filename); //���ļ���ȡ��B+��
    void printInfo();
    void printNode();
};

#endif //B_PLUS_TREE_H