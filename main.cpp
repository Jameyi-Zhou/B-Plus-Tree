#include <iostream>
#include "B_Plus_Tree.h"
using namespace std;

int main() {
    B_Plus_Tree B(7);
    Data d;
    for (int i = 0; i < 7; ++i) {
        d.key = i;
        d.info = "abcd";
        B.insertNode(d);
    }
    B.printNode();
    for (int i = 1; i < 4; ++i)
        B.deleteNode(i);
    B.printNode();
    d.key = 3;
    d.info = "abcd";
    B.insertNode(d);
    B.printNode();
    //cout << B.search(10, d);
    //B.store("data.dat");
    //B.load("data.dat");
    //B.printNode();

    return 0;
}
