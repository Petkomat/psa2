#include <iostream>
#include <algorithm>
#include "slaba.h"


using std::cout; using std::endl;


Slaba::Slaba() = default;


int Slaba::obdelaj(const string & ukaz){
    int i = ukaz.find(' ');
    int j = ukaz.rfind(' ');
    int x = std::stoi(ukaz.substr(i, j - i));
    int y = std::stoi(ukaz.substr(j));
    if (ukaz[0] == 'p'){
        return odgovori(x, y);
    } else{
        casi[steviloDodajanj] = x;
        vrednosti[steviloDodajanj] = y;
        steviloDodajanj++;
        return 0;
    }
}

int Slaba::odgovori(const int & t, const int & stevilo){
    for (int s = 0; s < SOSEDJE; s++){
        najblizji[s] = -1;
    }

    int * upperBoundPTR = std::upper_bound(std::begin(casi), std::begin(casi) + steviloDodajanj, t);
    int upperBound = std::distance(std::begin(casi), upperBoundPTR);
    int quality;
    int temp;
    for (int i = 0; i < upperBound; i++){
        quality = abs(stevilo - vrednosti[i]);
        if (quality < najblizji[0] || najblizji[0] < 0){
            najblizji[0] = quality;
            for (int s = 0; s < SOSEDJE - 1; s++){
                if (najblizji[s] < najblizji[s + 1] || najblizji[s + 1] < 0){
                    temp = najblizji[s + 1];
                    najblizji[s + 1] = najblizji[s];
                    najblizji[s] = temp;
                }
            }
        }
    }
    int total = 0;
    for (auto const & x : najblizji){
        if (x > 0){
            total += x;
        }
    }
    stevecP++;
    if (stevecP % 100 == 0){
        cout << ".";
    }
    if (stevecP % 1000 == 0){
        cout << endl;
    }
    return total;
}



