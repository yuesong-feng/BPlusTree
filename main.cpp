#include <iostream>
#include "BPTree.h"
#include <stdlib.h>
#include <time.h>

using namespace std;
int main(int argc, char const *argv[])
{
    BPTree<int, int> *tree = new BPTree<int, int>();
    srand((unsigned)time(NULL)); 
    int n = 10000;
    int a = 1, b = 10000;
    while(n--){
        int c = (rand() % (b - a)) + a; 
        tree->insert(c, c);
    }
    tree->display();
    return 0;
}
