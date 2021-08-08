#include <iostream>
#include "B_Plus_Tree.h"
using namespace std;
int main() {

    B_Plus_Tree B(9);
    Data d;
    for(int i = 0; i < 19; ++i){
        d.key = i;
        d.info = "aaaa";
        B.insertNode(d);
    }

    B.printRoot();
    B.printLeaf();
    return 0;
}
