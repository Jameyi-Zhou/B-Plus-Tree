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
    B_Plus_Tree(){
        leaf_counter = 0; //��ʼ��ʱû�в����κιؼ�����Ϣ������Ҷ�ӽڵ�����Ϊ0
        node_counter = 1; //ֻ��һ�����ڵ�
        this->order = 0; //δָ��order����Ҫ���ļ��ж�ȡ
        this->height = 1; //�߶�Ϊ1����ʾB+��û���м�ڵ�
        this->root = new B_Plus_Tree_Node; //����һ�����ڵ�
        this->root->key = new int[order]; //���ݽ���ȷ����ֵ���������
        this->root->child = new B_Plus_Tree_Node*[order+1]; //���ݽ���ȷ���ӽڵ���������
        for(int i = 0; i < order + 1; i++) //��ʼ�����ڵ�ĸ������ӽڵ�Ϊnullptr
            this->root->child[i] = nullptr;
        for(int i = 0; i < order; i++) //��ʼ�����ڵ��key���飬-1��ʾ��λ��û��key
            this->root->key[i] = -1;
        this->head = new B_Plus_Tree_Node; //����Ҷ�ڵ�˫������ͷ��㣬��Я���κ�����
        this->head->child = new B_Plus_Tree_Node*[2]; //child[0]ָ��ǰһ��Ҷ�ӽڵ�, child[1]ָ����һ��Ҷ�ӽڵ�
        this->rear = new B_Plus_Tree_Node; //����Ҷ�ڵ�˫������β��㣬��Я���κ�����
        this->rear->child = new B_Plus_Tree_Node*[2];
        this->head->child[0] = nullptr;
        this->rear->child[1] = nullptr;
        this->head->child[1] = this->rear;
        this->rear->child[0] = this->head;
    }
    B_Plus_Tree(int order){
        leaf_counter = 0;
        node_counter = 1;
        this->order = order; //B+����ʼ��ʱ���˲���order������������Ĺ��캯����һ�㲻ͬ�������ط�һ��
        this->height = 1;
        this->root = new B_Plus_Tree_Node;
        this->root->key = new int[order];
        this->root->child = new B_Plus_Tree_Node*[order+1];
        for(int i = 0; i < order + 1; i++)
            this->root->child[i] = nullptr;
        for(int i = 0; i < order; i++)
            this->root->key[i] = -1;
        this->head = new B_Plus_Tree_Node;
        this->head->child = new B_Plus_Tree_Node*[2];
        this->rear = new B_Plus_Tree_Node;
        this->rear->child = new B_Plus_Tree_Node*[2];
        this->head->child[0] = nullptr;
        this->rear->child[1] = nullptr;
        this->head->child[1] = this->rear;
        this->rear->child[0] = this->head;
    }
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