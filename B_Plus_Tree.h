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
    B_Plus_Tree();
    B_Plus_Tree(int order);
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