#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include "tabele.h"



void shraniVektor(std::vector<long long> const & trajanja, std::string const & pot) {
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


void stopajIzvajanje(Tabela & tabela, const std::string & datoteka){
    std::vector<long long> trajanja;

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
    VgrajenaTabela t;
    KonstantenPrirast tabelaK;
    KonstantenFaktorPrirasta tabelaAlfa;

    std::cout << tabelaK.izracunajNovoDolzino() << std::endl;
    std::cout << tabelaAlfa.izracunajNovoDolzino() << std::endl;

    VgrajenaTabela zaOgrevanje;
    for (int i = 0; i < 1e8; i++){
        zaOgrevanje.dodaj(i);
    }
    stopajIzvajanje(t, "casiVgrajena.txt");
    stopajIzvajanje(tabelaK, "casiK.txt");
    stopajIzvajanje(tabelaAlfa, "casiAlfa.txt");
    return 0;
};