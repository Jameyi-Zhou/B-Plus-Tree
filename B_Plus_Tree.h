#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H
#include<string>

using namespace std;
struct B_Plus_Tree_Node{
    int key_num; //当前键值的个数
    bool is_leaf;
    int* key; //存储键值的数组
    B_Plus_Tree_Node** child; //孩子节点
    B_Plus_Tree_Node* parent; //双亲节点
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

struct Data{ //结构体用于存储数据信息
    int key;
    string info;
};

class B_Plus_Tree{
private:
    int  height;
    int order; //B+树的阶数
    B_Plus_Tree_Node* root; //root节点始终指向B+树的根节点
    B_Plus_Tree_Node* first; //叶子节点存储为一个双向链表，该节点为头结点
    B_Plus_Tree_Node* last_leaf; //该节点为双向链表的尾结点
public:

    B_Plus_Tree(int order){
        this->order = order;
        this->height = 1; //高度为1，表示B+树没有中间节点
        root = new B_Plus_Tree_Node; //创建一个根节点
        root->key = new int[order]; //根据阶数确定键值的最大数量
        root->child = new B_Plus_Tree_Node*[order+1]; //根据阶数确定子节点的最大数量
        for(int i = 0; i < order+1; i++)
            root->child[i] = nullptr;
        for(int i = 0; i < order; i++)
            root->key[i] = -1;
        this->first = new B_Plus_Tree_Node; //建立叶节点的双向链表头结点，不携带任何数据
        this->first->child = new B_Plus_Tree_Node*[2];
        this->first->child[0] = this->first->child[1] = nullptr; //child[1]指向下一个叶子节点
        this->last_leaf = this->first;
    }
    int search_aux(int key, B_Plus_Tree_Node*& final_innode); // 单个查找辅助函数
    bool search(int key, Data& data); //单个查找
    void insert_1(B_Plus_Tree_Node*& node, Data data, int pos); //直接插入，不用分裂
    void insert_2(B_Plus_Tree_Node*& node, Data data, int pos); //需要分裂的插入
    bool insertNode(Data data); //单个插入
    bool deleteNode(int key); //单个删除
    bool search(int key1, int key2, Data*& data);//批量查找
    bool insertNode(Data* data); //批量插入
    bool deleteNode(int key1, int key2); //批量删除
    bool store(char* filename);//序列化并存入文件
    bool load(char* filename); //从文件中取出B+树
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