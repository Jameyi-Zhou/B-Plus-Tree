#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H
#include<string>

struct Data{ //结构体用于存储数据信息
    int key;
    std::string info;
};

struct B_Plus_Tree_Node{
    int key_num; //当前键值的个数
    bool is_leaf; //标记是否为叶子节点
    int* key; //存储键值的数组
    B_Plus_Tree_Node** child; //孩子节点指针数组
    B_Plus_Tree_Node* parent; //双亲节点
    std::string info; //叶子节点信息
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
    int order; //B+树的阶数
    int height; //B+树的高度
    int node_counter; //B+树的非叶结点个数
    int leaf_counter; //B+树的叶子个数(有多少关键字)
    B_Plus_Tree_Node* root; //root节点始终指向B+树的根节点
    B_Plus_Tree_Node* head; //叶子节点存储为一个双向链表，该节点为头结点，不携带任何数据
    B_Plus_Tree_Node* rear; //该节点为双向链表的尾结点，也不携带任何数据
public:
    B_Plus_Tree(){
        leaf_counter = 0; //初始化时没有插入任何关键字信息，所以叶子节点数量为0
        node_counter = 1; //只有一个根节点
        this->order = 0; //未指定order，需要从文件中读取
        this->height = 1; //高度为1，表示B+树没有中间节点
        this->root = new B_Plus_Tree_Node; //创建一个根节点
        this->root->key = new int[order]; //根据阶数确定键值的最大数量
        this->root->child = new B_Plus_Tree_Node*[order+1]; //根据阶数确定子节点的最大数量
        for(int i = 0; i < order + 1; i++) //初始化根节点的各个孩子节点为nullptr
            this->root->child[i] = nullptr;
        for(int i = 0; i < order; i++) //初始化根节点的key数组，-1表示该位置没有key
            this->root->key[i] = -1;
        this->head = new B_Plus_Tree_Node; //建立叶节点双向链表头结点，不携带任何数据
        this->head->child = new B_Plus_Tree_Node*[2]; //child[0]指向前一个叶子节点, child[1]指向下一个叶子节点
        this->rear = new B_Plus_Tree_Node; //建立叶节点双向链表尾结点，不携带任何数据
        this->rear->child = new B_Plus_Tree_Node*[2];
        this->head->child[0] = nullptr;
        this->rear->child[1] = nullptr;
        this->head->child[1] = this->rear;
        this->rear->child[0] = this->head;
    }
    B_Plus_Tree(int order){
        leaf_counter = 0;
        node_counter = 1;
        this->order = order; //B+树初始化时带了参数order，所以与上面的构造函数这一点不同，其他地方一样
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
    int search_aux(int key, B_Plus_Tree_Node*& final_innode); // 单个查找辅助函数
    bool search(int key, Data& data); //单个查找函数
    void insert_1(B_Plus_Tree_Node*& node, Data data, int pos); //直接插入，不用分裂
    void insert_2(B_Plus_Tree_Node*& node, Data data, int pos); //需要分裂的插入
    bool insertNode(Data data); //单个插入
    bool deleteNode(int key); //单个删除
    //bool search(int key1, int key2, Data*& data);//批量查找
    //bool insertNode(Data* data); //批量插入
    //bool deleteNode(int key1, int key2); //批量删除
    bool store(char* filename);//序列化并存入文件
    bool load(char* filename); //从文件中取出B+树
    void printInfo();
    void printNode();
};

#endif //B_PLUS_TREE_H