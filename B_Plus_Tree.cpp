#include <iostream>
#include<fstream>
#include<string>
#include<queue>
#include "B_Plus_Tree.h"
using namespace std;

B_Plus_Tree::B_Plus_Tree() {
    this->leaf_counter = 0; //初始化时没有插入任何关键字信息，所以叶子节点数量为0
    this->node_counter = 1; //只有一个根节点
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

B_Plus_Tree::B_Plus_Tree(int order) {
    this->leaf_counter = 0;
    this->node_counter = 1;
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

int B_Plus_Tree::search_aux(int key, B_Plus_Tree_Node*& final_innode) { //final_innode->child[pos]可能指向空指针
    B_Plus_Tree_Node* p = final_innode = this->root;
    int pos = 0;
    while(p && !p->is_leaf) { //当p不是叶子层的指针时，根据键值大小往下寻找
        final_innode = p;
        if(p->key[0] > key) {
            pos = 0;
            p = p->child[pos];
        }
        else if(p->key[p->key_num-1] <= key) {
            pos = p->key_num;
            p = p->child[pos];
        }
        else {
            for(pos = 1; pos < p->key_num - 1; ++pos)
                if (p->key[pos-1] <= key && p->key[pos] > key)
                    break;
            p = p->child[pos];
        }
    }
    return pos; //返回最后一个非叶子节点所指的叶节点位置，为检索和插入节点提供信息
}

bool B_Plus_Tree::search(int key, Data &data) {
    B_Plus_Tree_Node* final_innode = nullptr;
    int pos = this->search_aux(key, final_innode);
    if(final_innode->child[pos] && *(final_innode->child[pos]->key) == key) {
        data.key = key;
        data.info = final_innode->child[pos]->info;
        return true;
    }
    return false;
}

void B_Plus_Tree::insert_1(B_Plus_Tree_Node*& node, Data data, int pos) { //直接插入，不用分裂
    B_Plus_Tree_Node* s = new B_Plus_Tree_Node; //构造一个叶子节点，准备插入到B+树当中
    s->is_leaf = true;
    s->key = new int;
    s->parent = node;
    *(s->key) = data.key;
    s->info = data.info;

    if(pos == 0 && !node->child[0]) { //如果插入时B+树只有一个叶子,且插入的key比根节点的key小
        node->child[pos] = s;
        node->child[pos]->child = new B_Plus_Tree_Node*[2]; //叶子链表，child[0]指向上一个叶子，child[1]指向下一个叶子
        node->child[pos]->child[0] = node->child[pos+1]->child[0];
        node->child[pos]->child[1] = node->child[pos+1];
        node->child[pos]->child[0]->child[1] = node->child[pos];
        node->child[pos+1]->child[0] = node->child[pos];
        ++this->leaf_counter;
        return;
    }
    else if(pos == 1 && node->child[1] && !node->child[0]) { //插入时B+树只有一个叶子，且插入的key比根节点的key大
        node->key[pos-1] = data.key;
        node->child[pos-1] = node->child[pos];
        node->child[pos] = s;
        node->child[pos]->child = new B_Plus_Tree_Node *[2];
        node->child[pos]->child[0] = node->child[pos-1];
        node->child[pos]->child[1] = node->child[pos-1]->child[1];
        node->child[pos-1]->child[1] = node->child[pos];
        node->child[pos]->child[1]->child[0] = node->child[pos];
        ++this->leaf_counter;
        return;
    }
    else { //当B+树的叶子节点大于等于2时
        for(int i = node->key_num + 1; i > pos; --i)
            node->child[i] = node->child[i-1];
        node->child[pos] = s;
        if (pos > node->key_num) //要插入的位置在该节点的最后
            node->key[pos - 1] = data.key;
        else  //要插入的地方有数据，需要将key重新赋值
            for(int i = pos - 1; i <= node->key_num; ++i)
                node->key[i] = *(node->child[i+1]->key);
        ++node->key_num;
        ++this->leaf_counter;
        if(node->key_num == 1) //此时为根节点的初始化
            return;
        //对叶节点链表进行修改
        node->child[pos]->child = new B_Plus_Tree_Node *[2];
        if(pos < node->key_num) {
            node->child[pos]->child[0] = node->child[pos+1]->child[0];
            node->child[pos]->child[1] = node->child[pos+1];
            node->child[pos+1]->child[0]->child[1] = node->child[pos];
            node->child[pos+1]->child[0] = node->child[pos];
        }
        else {
            node->child[pos]->child[0] = node->child[pos-1];
            node->child[pos]->child[1] = node->child[pos-1]->child[1];
            node->child[pos-1]->child[1] = node->child[pos];
            node->child[pos]->child[1]->child[0] = node->child[pos];
        }
    }

}

void B_Plus_Tree::insert_2(B_Plus_Tree_Node*& node, Data data, int pos) { //需要分裂的插入
    insert_1(node, data, pos); //插入的结点已满，需要进行分裂
    B_Plus_Tree_Node* p = node;
    while(p) {
        int p_key = p->key[this->order/2];
        B_Plus_Tree_Node* s_next_sibling = new B_Plus_Tree_Node;
        s_next_sibling->key = new int[this->order];
        s_next_sibling->child = new B_Plus_Tree_Node*[this->order+1];
        for(int i = 0; i < this->order + 1; i++)
            s_next_sibling->child[i] = nullptr;
        for(int i = 0; i < this->order; i++)
            s_next_sibling->key[i] = -1;
        for(int i = 0; i < this->order / 2; ++i) {
            s_next_sibling->key[i] = p->key[this->order/2+i+1];
            s_next_sibling->child[i+1] = p->child[this->order/2+i+2];
            s_next_sibling->child[i+1]->parent = s_next_sibling;
            p->key[this->order/2+i+1] = -1;
            p->child[this->order/2+i+2] = nullptr;
        }
        s_next_sibling->child[0] = p->child[this->order/2+1];
        s_next_sibling->child[0]->parent = s_next_sibling;
        p->key[this->order/2] = -1;
        p->child[this->order/2+1] = nullptr;

        p->key_num = s_next_sibling->key_num = this->order / 2;
        ++this->node_counter;
        B_Plus_Tree_Node* q = p;
        p = p->parent;
        if(p) { //
            int p_pos = 0;
            for(; p_pos < p->key_num; ++p_pos)
                if(p->key[p_pos] > p_key)
                    break;
            for(int i = p->key_num; i > p_pos; --i) {
                p->key[i] = p->key[i-1];
                p->child[i+1] = p->child[i];
            }
            p->key[p_pos] = p_key;
            p->child[p_pos+1] = s_next_sibling;
            s_next_sibling->parent = p;
            ++p->key_num;
            if(p->key_num < this->order)
                return;
        }
        else { //根节点需要分裂，需要再new一个根节点
            B_Plus_Tree_Node* new_root = new B_Plus_Tree_Node;
            new_root->key = new int[this->order];
            new_root->child = new B_Plus_Tree_Node*[this->order+1];
            for(int i = 0; i < this->order + 1; i++)
                new_root->child[i] = nullptr;
            for(int i = 0; i < this->order; i++)
                new_root->key[i] = -1;
            new_root->key_num = 1;
            new_root->key[0] = p_key;
            new_root->child[0] = q;
            new_root->child[1] = s_next_sibling;
            q->parent = new_root;
            s_next_sibling->parent = new_root;
            ++this->height;
            ++this->node_counter;
            this->root = new_root;
            return;
        }
    }
}

bool B_Plus_Tree::insertNode(Data data) {
    B_Plus_Tree_Node* final_innode = nullptr;
    int child_pos = search_aux(data.key, final_innode);

    if(this->root->key_num == 0) { //如果是一棵空树，则进行初始化操作
        child_pos = 1;
        insert_1(final_innode, data, child_pos);
        final_innode->child[child_pos]->child = new B_Plus_Tree_Node *[2];
        this->head->child[1] = final_innode->child[child_pos];
        final_innode->child[child_pos]->child[0] = this->head;
        final_innode->child[child_pos]->child[1] = this->rear;
        this->rear->child[0] = final_innode->child[child_pos];
        return true;
    }
    else if(final_innode->child[child_pos] && *(final_innode->child[child_pos]->key) == data.key) //要插入的节点已经存在
        return false;
    else if(!this->root->child[0]) { //如果是根节点只有一个叶子
        if(*(this->root->child[1]->key) > data.key) child_pos = 0;
        else child_pos = 1;
        insert_1(final_innode, data, child_pos);
        return true;
    }
    else { //如果不是一个空树且根节点有两个以上的孩子，则先找到应该插入的位置，再根据情况进行插入操作
        if (final_innode->child[child_pos] && *(final_innode->child[child_pos]->key) < data.key) //确定key应该在哪一个位置
            ++child_pos;
        if (final_innode->key_num < this->order - 1)  //该节点未插满叶子
            insert_1(final_innode, data, child_pos);
        else //该节点叶子已插满
            insert_2(final_innode, data, child_pos);
        return true;
    }
}

bool B_Plus_Tree::deleteNode(int key) {
    B_Plus_Tree_Node* node = nullptr, *p;
    int child_pos = search_aux(key, node);
    if(!node->child[child_pos] || *(node->child[child_pos]->key) != key) //不存在要删除的节点
        return false;
    p = node;
    B_Plus_Tree_Node* q = p->child[child_pos];
    q->child[1]->child[0] = q->child[0]; //q节点的后驱节点的前驱指针指向q的前驱节点
    q->child[0]->child[1] = q->child[1]; //q节点的前驱节点的后驱指针指向q的后驱节点
    delete q;

    if(child_pos == p->key_num)
        p->key[p->key_num-1] = -1;
    for(int i = child_pos; i < p->key_num; ++i) {
        p->key[i] = p->key[i+1];
        p->child[i] = p->child[i+1];
    }
    p->child[p->key_num] = nullptr;
    --this->leaf_counter;
    if(--p->key_num >= this->order / 2)
        return true;

    else {
        q = p;
        p = p->parent;
        while(p) {
            int pos = 0;
            for(; pos <= p->key_num; ++pos)
                if(p->child[pos] == q) break;

            if(pos < p->key_num && p->child[pos+1]->key_num > this->order / 2) { //有右兄弟且右兄弟够借
                B_Plus_Tree_Node* r = p->child[pos+1];
                p->key[pos] = r->key[0];
                q->child[q->key_num+1] = r->child[0];
                q->child[q->key_num+1]->parent = q;
                B_Plus_Tree_Node* temp = r;
                for(; !temp->is_leaf; temp = temp->child[0]);
                q->key[q->key_num] = *(temp->key);
                ++q->key_num;
                --r->key_num;
                for(int i = 0; i < r->key_num; ++i) {
                    r->key[i] = r->key[i+1];
                    r->child[i] = r->child[i+1];
                }
                r->child[r->key_num] = r->child[r->key_num+1];

            }
            else if(pos > 0 && p->child[pos-1]->key_num > this->order / 2) { //有左兄弟且左兄弟够借
                B_Plus_Tree_Node* l = p->child[pos-1];
                p->key[pos-1] = l->key[l->key_num-1];

                for(int i = q->key_num; i > 0; --i) {
                    q->key[i] = q->key[i-1];
                    q->child[i] = q->child[i-1];
                }
                q->child[0] = l->child[l->key_num];
                q->child[0]->parent = q;
                q->key[0] = *(q->child[1]->key);
                l->key[l->key_num-1] = -1;
                l->child[l->key_num] = nullptr;
                ++q->key_num;
                --l->key_num;
            }
            else { //和兄弟节点合并
                if(pos == 0) {
                    ++pos;
                    q = p->child[pos];
                }
                B_Plus_Tree_Node *l = p->child[pos-1];
                l->key[l->key_num] = p->key[pos-1];
                ++l->key_num;
                for(int i = 0; i < q->key_num; ++i) {
                    l->key[l->key_num] = q->key[i];
                    l->child[l->key_num] = q->child[i];
                    q->child[i] = nullptr;
                    l->child[l->key_num]->parent = l;
                    ++l->key_num;
                }
                l->child[l->key_num] = q->child[q->key_num];
                l->child[l->key_num]->parent = l;
                q->child[q->key_num] = nullptr;
                delete q;
                for(int i = pos - 1; i < p->key_num; ++i) {
                    p->key[i] = p->key[i+1];
                    p->child[i+1] = p->child[i+2];
                }
                --p->key_num;
                --this->node_counter;
            }
            if(p->key_num >= this->order / 2 || (p == this->root && p->key_num > 0))
                return true;
            if(!p->key_num) { //删除根节点
                this->root = p->child[0];
                this->root->parent = nullptr;
                p->child[0] = nullptr;
                --height;
                delete p;
            }
            q = p;
            p = p->parent;
        }
    }

    return true;
}

bool B_Plus_Tree::store(char *filename) {
    ofstream fout(filename, ios::binary);
    int order = this->order;
    fout.write((char*)&order, 4); //存入B+树的阶数，从文件读入时根据order获取每个节点的大小
    fout.write((char*)&this->height, 4); //存入B+树的树高
    fout.write((char*)&this->node_counter, 4); //B+树中间节点数量
    fout.write((char*)&this->leaf_counter, 4); //B+树叶子结点数量

    B_Plus_Tree_Node *r = this->root;
    B_Plus_Tree_Node *p = nullptr;
    queue<B_Plus_Tree_Node*> q;
    q.push(r);
    while(q.front()->key_num != 0) {
        p = q.front();
        fout.write((char*)&p->key_num, 4);
        fout.write((char*)p->key, this->order * 4);
        for(int i = 0; i <= p->key_num; ++i)
            q.push(p->child[i]);
        q.pop();
    }
    while(!q.empty()) {
        p = q.front();
        fout.write((char*)p->key, 4);
        int len = p->info.length();
        ++len;
        fout.write((char*)&len, 4);
        fout.write(p->info.c_str(), len);
        q.pop();
    }
    fout.close();
    return true;
}

bool B_Plus_Tree::load(char *filename) {
    ifstream fin(filename, ios::binary);
    int order, height, node_counter, leaf_counter;
    fin.read((char*)&order, 4);
    fin.read((char*)&height, 4);
    fin.read((char*)&node_counter, 4);
    fin.read((char*)&leaf_counter, 4);
    this->order = order;
    this->height = height;
    this->node_counter = node_counter;
    this->leaf_counter = leaf_counter;
    this->head = new B_Plus_Tree_Node;
    this->head->child = new B_Plus_Tree_Node*[2];
    this->rear = new B_Plus_Tree_Node;
    this->rear->child = new B_Plus_Tree_Node*[2];
    this->head->child[0] = nullptr;
    this->rear->child[1] = nullptr;
    this->head->child[1] = this->rear;
    this->rear->child[0] = this->head;
    fin.read((char*)&this->root->key_num, 4);
    fin.read((char*)this->root->key, this->order * 4);
    queue<B_Plus_Tree_Node*> q;
    q.push(this->root);
    --node_counter;
    while(node_counter) { //插入非叶子节点
        B_Plus_Tree_Node *p = q.front();
        for (int j = 0; j <= p->key_num; ++j) {
            B_Plus_Tree_Node *ch = new B_Plus_Tree_Node;
            ch->key = new int[this->order];
            ch->child = new B_Plus_Tree_Node *[this->order + 1];
            for (int i = 0; i < order + 1; ++i)
                ch->child[i] = nullptr;
            fin.read((char *) &ch->key_num, 4);
            fin.read((char *) ch->key, this->order * 4);
            p->child[j] = ch;
            ch->parent = p;
            q.push(ch);
            --node_counter;
        }
        q.pop();
    }
    B_Plus_Tree_Node* leaf_p = this->head;
    while(leaf_counter) { //插入叶子节点
        B_Plus_Tree_Node *p = q.front();
        for(int i = 0; i <= p->key_num; ++i) {
            --leaf_counter;
            B_Plus_Tree_Node *ch = new B_Plus_Tree_Node;
            ch->key = new int;
            ch->is_leaf = 1;
            fin.read((char *) ch->key, 4);
            int len;
            fin.read((char *) &len, 4);
            char *temp = new char[len];
            fin.read(temp, len);
            ch->info = temp;
            p->child[i] = ch;
            ch->parent = p;
            ch->child = new B_Plus_Tree_Node*[2];
            ch->child[1] = leaf_p->child[1];
            ch->child[0] = leaf_p;
            leaf_p->child[1] = ch;
            this->rear->child[0] = ch;
            leaf_p = ch;
        }
        q.pop();
    }
    fin.close();
    return true;
}

void B_Plus_Tree::printInfo() {
    B_Plus_Tree_Node* p = this->head->child[1];
    while(p != this->rear){
        cout << *(p->key) << " " << p->info << endl;
        p = p->child[1];
    }
}

void B_Plus_Tree::printNode() {
    B_Plus_Tree_Node* p = nullptr, *layer_last_node = this->root;
    queue<B_Plus_Tree_Node*> q;
    q.push(this->root);
    while(!q.empty()){
        p = q.front();
        cout << "|" << p->key[0];
        for(int i = 1; i < p->key_num; ++i)
            cout << "|" << p->key[i];
        cout << "|  ";
        if(p->key_num!=0)
            for(int i = 0; i <= p->key_num; ++i)
                q.push(p->child[i]);
        if(p == layer_last_node) {
            cout << endl;
            layer_last_node = q.back();
        }
        q.pop();
    }
}


