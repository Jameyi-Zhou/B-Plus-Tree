#include <iostream>
#include<string>
#include "B_Plus_Tree.h"
using namespace std;

int B_Plus_Tree::search_aux(int key, B_Plus_Tree_Node*& final_innode) { //final_innode->child[pos]可能指向空指针
    B_Plus_Tree_Node* p = final_innode = this->root;
    int pos = 0;
    while(p && !p->is_leaf){
        final_innode = p;
        if(p->key[0] > key) {
            p = p->child[pos];
        }
        else if(p->key[p->key_num-1] <= key) {
            pos = p->key_num;
            p = p->child[pos];
        }
        else{
            for(pos = 1; pos < p->key_num - 1; ++pos) {
                if (p->key[pos - 1] <= key && p->key[pos] > key)
                    break;
            }
            p = p->child[pos];
        }
    }
    return pos; //返回最后一个中间节点所指的叶节点位置，为检索和插入节点提供信息
}

bool B_Plus_Tree::search(int key, Data &data) {
    B_Plus_Tree_Node* final_innode = nullptr;
    int pos = this->search_aux(key, final_innode);
    if(final_innode->child[pos] && *(final_innode->child[pos]->key) == key){
        data.key = key;
        data.info = final_innode->child[pos]->info;
        return true;
    }
    else
        return false;
}

void B_Plus_Tree::insert_1(B_Plus_Tree_Node*& node, Data data, int pos){ //直接插入，不用分裂
    B_Plus_Tree_Node* s = new B_Plus_Tree_Node;
    s->is_leaf = true;
    s->key = new int;
    s->parent = node;
    *(s->key) = data.key;
    s->info = data.info;

    if(pos == 0 && !node->child[0]){
        node->child[pos] = s;
        node->child[pos]->child = new B_Plus_Tree_Node *[2];
        node->child[pos]->child[0] = node->child[pos+1]->child[0];
        node->child[pos]->child[1] = node->child[pos+1];
        node->child[pos+1]->child[0]->child[1] = node->child[pos];
        node->child[pos+1]->child[0] = node->child[pos];
        return;
    }
    else if(pos == 1 && node->child[1] && !node->child[0]){
        node->key[pos-1] = data.key;
        node->child[pos-1] = node->child[pos];
        node->child[pos] = s;
        node->child[pos]->child = new B_Plus_Tree_Node *[2];
        node->child[pos]->child[0] = node->child[pos-1];
        node->child[pos]->child[1] = nullptr;
        node->child[pos-1]->child[1] = node->child[pos];
        return;
    }
    else if(!node->child[pos])
        node->key[pos-1] = data.key;
    else if(*(node->child[pos]->key) > data.key){
        for(int i = node->key_num + 1; i > pos; --i)
            node->key[i] = node->key[i-1];
        node->key[pos] = *(node->child[pos]->key);
    }
    else {
        for(int i = node->key_num + 1; i > pos - 1; --i)
            node->key[i] = node->key[i-1];
        node->key[pos-1] = data.key;
    }
    for(int i = node->key_num + 1; i > pos; --i)
        node->child[i] = node->child[i-1];
    node->child[pos] = s;
    ++node->key_num;
    if(node->key_num == 1) //此时为根节点的初始化
        return;

    //对叶节点链表进行修改
    node->child[pos]->child = new B_Plus_Tree_Node *[2];
    if(pos < node->key_num){
        node->child[pos]->child[0] = node->child[pos+1]->child[0];
        node->child[pos]->child[1] = node->child[pos+1];
        node->child[pos+1]->child[0]->child[1] = node->child[pos];
        node->child[pos+1]->child[0] = node->child[pos];
    }
    else{
        node->child[pos]->child[0] = node->child[pos-1];
        node->child[pos]->child[1] = node->child[pos-1]->child[1];
        node->child[pos-1]->child[1] = node->child[pos];
        if (node->child[pos]->child[1] == nullptr)
            this->last_leaf = node->child[pos];
        else
            node->child[pos]->child[1]->child[0] = node->child[pos];
    }
}

void B_Plus_Tree::insert_2(B_Plus_Tree_Node*& node, Data data, int pos){ //需要分裂的插入
    insert_1(node, data, pos); //插入的结点已满，需要进行分裂
    B_Plus_Tree_Node* p = node;

    while(p){
        int p_key = p->key[this->order / 2];
        B_Plus_Tree_Node* s_next_sibling = new B_Plus_Tree_Node;
        s_next_sibling->key = new int[this->order];
        s_next_sibling->child = new B_Plus_Tree_Node*[this->order+1];
        for(int i = 0; i < order+1; i++)
            s_next_sibling->child[i] = nullptr;
        for(int i = 0; i < order; i++)
            s_next_sibling->key[i] = -1;
        for(int i = 0; i < this->order / 2; ++i){
            s_next_sibling->key[i] = p->key[this->order/2+i+1];
            s_next_sibling->child[i+1] = p->child[this->order/2+i+2];
            s_next_sibling->child[i+1]->parent = s_next_sibling;
            p->child[this->order/2+i+2] = nullptr;
        }
        s_next_sibling->child[0] = p->child[this->order/2+1];
        s_next_sibling->child[0]->parent = s_next_sibling;
        p->child[this->order/2+1] = nullptr;
        p->key_num = s_next_sibling->key_num = this->order / 2;
        B_Plus_Tree_Node* q = p;
        p = p->parent;
        if(p) {
            int p_pos = 0;
            for(; p_pos < p->key_num; ++p_pos){
                if(p->key[p_pos] > p_key)
                    break;
            }
            for(int i = p->key_num; i > p_pos; --i){
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
        else{ //根节点需要分裂
            B_Plus_Tree_Node* new_root = new B_Plus_Tree_Node;
            new_root->key = new int[this->order];
            new_root->child = new B_Plus_Tree_Node*[this->order+1];
            for(int i = 0; i < order+1; i++)
                new_root->child[i] = nullptr;
            for(int i = 0; i < order; i++)
                new_root->key[i] = -1;
            new_root->key_num = 1;
            new_root->key[0] = p_key;
            new_root->child[0] = q;
            new_root->child[1] = s_next_sibling;
            q->parent = new_root;
            s_next_sibling->parent = new_root;
            ++this->height;
            this->root = new_root;
            return;
        }
    }
}

bool B_Plus_Tree::insertNode(Data data) {
    B_Plus_Tree_Node* final_innode = nullptr;
    int child_pos = search_aux(data.key, final_innode);
    //如果是一个空树，则进行初始化操作

    if(this->root->key_num == 0) {
        child_pos = 1;
        insert_1(final_innode, data, child_pos);
        final_innode->child[child_pos]->child = new B_Plus_Tree_Node *[2];
        this->first->child[1] = final_innode->child[child_pos];
        final_innode->child[child_pos]->child[1] = nullptr;
        final_innode->child[child_pos]->child[0] = this->first;
        this->last_leaf = final_innode->child[child_pos];
        cout << "insert key " << data.key << " successfully!" << endl;
        return true;
    }

    //要插入的节点已经存在
    else if(final_innode->child[child_pos] && *(final_innode->child[child_pos]->key) == data.key) {
        cout << "insert fail! key " << data.key << " already exists!" << endl;
        return false;
    }

    //如果是根节点只有一个叶子
    else if(!this->root->child[0]){
        if(*(this->root->child[1]->key) > data.key)
            child_pos = 0;
        else
            child_pos = 1;
        insert_1(final_innode, data, child_pos);
        if (final_innode->child[child_pos]->child[1] == nullptr)
            this->last_leaf = final_innode->child[child_pos];
        cout << "insert key " << data.key << " successfully!" << endl;
        return true;
    }

    //如果不是一个空树且根节点有两个以上的孩子，则先找到应该插入的位置，再根据情况进行插入操作
    else {
        if (*(final_innode->child[child_pos]->key) < data.key)
            ++child_pos;
        if (final_innode->key_num < this->order - 1)  //该节点未插满叶子
            insert_1(final_innode, data, child_pos);
        else { //该节点叶子已插满
            insert_2(final_innode, data, child_pos);
        }
        cout << "insert key " << data.key << " successfully!" << endl;
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

    if(p->child[child_pos] != this->last_leaf)//如果删除的是最后一个叶子节点
        p->child[child_pos]->child[1]->child[0] = p->child[child_pos]->child[0];
    else
        this->last_leaf = p->child[child_pos]->child[0];
    p->child[child_pos]->child[0]->child[1] = p->child[child_pos]->child[1];
    delete(q);

    if(child_pos == p->key_num)
        p->key[p->key_num-1] = -1;
    for(int i = child_pos; i < p->key_num; ++i){
        p->key[i] = p->key[i+1];
        p->child[i] = p->child[i+1];
    }
    p->child[p->key_num] = nullptr;

    if(--p->key_num >= this->order / 2)
        return true;
    else{
        q = p;
        p = p->parent;
        while(p){
            int pos = 0;
            for(; pos <= p->key_num; ++pos)
                if(p->child[pos] == q) break;
            if(pos < p->key_num && p->child[pos+1]->key_num > this->order / 2){ //有右兄弟且右兄弟够借
                B_Plus_Tree_Node* r = p->child[pos+1];
                p->key[pos] = r->key[0];
                q->child[q->key_num] = r->child[0];
                q->child[q->key_num]->parent = q;
                q->key[q->key_num] = *(r->child[0]->key);
                ++q->key_num;
                --r->key_num;
                for(int i = 0; i < r->key_num; ++i){
                    r->key[i] = r->key[i+1];
                    r->child[i] = r->child[i+1];
                }
                r->child[r->key_num] = r->child[r->key_num+1];
            }
            else if(pos > 0 && p->child[pos-1]->key_num > this->order / 2){ //有左兄弟且左兄弟够借
                B_Plus_Tree_Node* l = p->child[pos-1];
                p->key[pos-1] = l->key[l->key_num-1];

                for(int i = q->key_num; i > 0; --i){
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
                l->key[l->key_num] = key;
                ++l->key_num;
                for(int i = 0; i < q->key_num; ++i){
                    l->key[l->key_num] = q->key[i];
                    l->child[l->key_num] = q->child[i];
                    l->child[l->key_num]->parent = l;
                    ++l->key_num;
                }
                l->child[l->key_num] = q->child[q->key_num];
                delete(q);
                for(int i = pos-1; i < p->key_num; ++i){
                    p->key[i] = p->key[i+1];
                    p->child[i+1] = p->child[i+2];
                }
                --p->key_num;
            }
            q = p;
            p = p->parent;
        }
    }
    cout << "delete key " << key << " successfully!" << endl;
    return true;
}

bool B_Plus_Tree::store(char *filename) {

}



