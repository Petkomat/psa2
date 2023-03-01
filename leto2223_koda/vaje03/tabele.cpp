#include "tabele.h"
#include <iostream>

/*
TABELA
*/

// Destruktor moramo vseeno implementirati: https://stackoverflow.com/a/12574407/2281318
Tabela::~Tabela() = default;

VgrajenaTabela::~VgrajenaTabela() = default;

void VgrajenaTabela::dodaj(const int& x){
    tabela.push_back(x);
}

int VgrajenaTabela::dostopaj(const int& i) const {
    return tabela[i];
}

int VgrajenaTabela::trenutnaVelikost() const {
    return (int) tabela.size();
}

/*
NASA TABELA
*/
NasaTabela::NasaTabela() 
    : tabela(new int[velikost])
    {

    };

NasaTabela::~NasaTabela() {
    delete [] tabela;
}

void NasaTabela::dodaj(const int& x){
    // ce odkomentirate cout v implementaciji izracunajNovoDolzino()
    // v obeh podrazredih, boste videli, da se klice ustrezna
    int novaDolzina = izracunajNovoDolzino();
    if (naslednji >= velikost){
        auto * nova = new int[novaDolzina];
        // naredimo kopijo
        for (int i = 0; i < velikost; i++){
            nova[i] = tabela[i];
        }
        // unicimo staro
        delete [] tabela;
        tabela = nova;
        // posodobi velikost
        velikost = novaDolzina;   
    }
    tabela[naslednji] = x;
    naslednji++;
}

int NasaTabela::dostopaj(const int& i) const {
    return tabela[i];
}

int NasaTabela::trenutnaVelikost() const {
    return velikost;
};

/*
NASA TABELA: K
*/
KonstantenPrirast::~KonstantenPrirast() = default;

int KonstantenPrirast::izracunajNovoDolzino() {
    // std::cout << "+ K" << std::endl;
    return velikost + K;
}

/*
NASA TABELA: ALFA
*/
KonstantenFaktorPrirasta::~KonstantenFaktorPrirasta() = default;

int KonstantenFaktorPrirasta::izracunajNovoDolzino() {
    // std::cout << "* ALFA" << std::endl;
    return (int) (ALFA * velikost);
}
