#include <iostream>
#include <vector>
#include <string>
#include "strukture.h"


using std::vector;
using std::string;

int main()
{
    auto t = Trie(); // namesto Trie t = Trie(), saj lahko tip ugotovimo

    vector<string> besede1{"aaa", "aa", "ba", "a"};

    for (string beseda : besede1){
        t.vstavi(beseda);
    }
    std::cout << "Drevo vsebuje naslednje besede:" << std::endl;
    for (string b : t.nastejBesedeUrejeno()){
        std::cout << b << std::endl;
    }
    std::cout << t.toString()   << std::endl;

    std::cout << "Vstavimo še del sskj-ja:" << std::endl;
    t.vstaviIzDatoteke("malo.txt");
    std::cout << "Drevo vsebuje sedaj:" << std::endl;
    for (string b : t.nastejBesedeUrejeno()){
        std::cout << b << std::endl;
    }
};