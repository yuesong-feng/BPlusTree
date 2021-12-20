#include <iostream>
#include "BPTree.h"
using namespace std;
int main(int argc, char const *argv[])
{
    BPTree<int, int> *tree = new BPTree<int, int>();
    int n;
    while (std::cin >> n)
    {
        tree->insert(n, n);
        tree->display();
    }
    tree->display();
    return 0;
}
