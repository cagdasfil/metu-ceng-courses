#include <iostream>

#include "tpbst.hpp"

int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.insert("ceng213", "sec2", 26);

    std::cout << "first insert successful." << '\n';

    tpbst.print();

    tpbst.insert("ceng223", "sec1", 38);

    std::cout << "second insert successful." << '\n';

    tpbst.print();

    tpbst.insert("ceng140", "sec3", 15);

    std::cout << "third insert successful." << '\n';

    tpbst.print();

    tpbst.remove("ceng213", "sec2");

    tpbst.print();

    tpbst.insert("ceng213", "sec2", 26);

    tpbst.print();

    tpbst.insert("ceng213", "sec1", 36);

    tpbst.print();


    return 0;
}
