#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "tabele.h"


void shraniVektor(vector<long long> &trajanja, std::string pot) {
    std::ofstream fw(pot, std::ofstream::out);
    if (fw.is_open())
    {
      for (auto const &element : trajanja) {
        fw << element << "\n";
      }
      fw.close();
    }   else {
        std::cout << "Nisem mogel shraniti v " << pot << std::endl;
    }
}


void stopajIzvajanje(KonstantenPrirast tabela, std::string datoteka){
    vector<long long> trajanja;

    // stopaj
    int n = 1e4;
    for (int skupina = 0; skupina < n; skupina++){
        auto t0 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++){
            tabela.dodaj(i);  // vseeno je, kaj dodajamo
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        auto trajanje = (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count();
        trajanja.push_back(trajanje);
    }
    shraniVektor(trajanja, datoteka);
}


int main()
{

    return 0;
};