#include <iostream>
#include <vector>
#include <string>
#include "strukture.h"


using std::vector;
using std::string;

 // ${workspaceFolder}

int main()
{
    // tudi to deluje: Trie t;
    auto t = Trie(); // namesto Trie t = Trie(), saj lahko tip ugotovimo

    vector<string> besede1{"aaa", "aa", "ba", "a"};

    for (auto const &beseda : besede1){
        t.vstavi(beseda);
    }
    std::cout << "Drevo vsebuje naslednje besede:" << std::endl;
    for (auto const &b : t.nastejBesedeUrejeno()){
        std::cout << b << std::endl;
    }
    std::cout << t.toString() << std::endl;

    std::cout << "Vstavimo še del sskj-ja:" << std::endl;
    t.vstaviIzDatoteke("malo.txt");
    std::cout << "Drevo vsebuje sedaj:" << std::endl;
    for (string b : t.nastejBesedeUrejeno()){
        std::cout << b << std::endl;
    }
    std::cout << t.toString() << std::endl;
    int vozlisca = t.steviloVozlisc();
    int crke = t.getSteviloCrk();
    std::cout << "Št. vozlišč v drevesu:             " << vozlisca << std::endl;                // sskj: 344067
    std::cout << "Št. črk, ki jih hranimo:           " << crke << std::endl;                    // sskj: 971799
    std::cout << "Faktor kompresije (črke/vozlišča): " << (1.0 * crke / vozlisca) << std::endl; // sskj: 2.825
};