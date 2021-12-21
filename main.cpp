#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "BPTree.h"
using namespace std;
int main(int argc, char const *argv[])
{
    BPTree<int, int> *tree = new BPTree<int, int>();
    int a = 0, b = 10000;
    srand((unsigned)time(NULL));
    int n;
    cin >> n;
    while(n--){
        int c = (rand() % (b - a)) + a;
        tree->insert(n, n);
        tree->display();
    }
    return 0;
}
