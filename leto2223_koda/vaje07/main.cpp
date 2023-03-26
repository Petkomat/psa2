#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <utility>
#include <numeric>  // std::iota
#include "sip.h"


using std::vector;
using std::pair;
using std::cout;
using std::endl;


bool preveriVse(PoizvedbeZGlavo & zGlavo, PoizvedbeSSilo & sSilo, int n){
    for (int i = 0; i < n; i++){
        for (int j = i; j < n; j++){
            if (zGlavo.odgovori(i, j) != sSilo.odgovori(i, j)){
                cout << "napacen odgovor pri n = " << n << " za interval [" << i << ", " << j << "]: ";
                cout << zGlavo.odgovori(i, j) << " != " << sSilo.odgovori(i, j) << endl;
                return false;
            }
        }
    }
    return true;
}

/*
Dodamo neko število na vsakega od podintervalov [0, n].
Po vsakem dodajanju preverimo pravilnost vseh poizvedb.
*/
void preveri_vsote(){
    for (int n = 5; n < 18; n++){
        vector<int> vrednosti(n);
        std::iota(vrednosti.begin(), vrednosti.end(), 0);
        VsotaZGlavo zGlavo {vrednosti};
        VsotaSSilo sSilo {vrednosti};
        for (int i = 0; i < n; i++){
            for (int j = i; j < n; j++){
                zGlavo.povecaj(i, j, (i + 1) * (j + 1));
                sSilo.povecaj(i, j, (i + 1) * (j + 1));
                if (! preveriVse(zGlavo, sSilo, n)){
                    return;
                }
            }
        }
    }
}


vector<pair<int, int>> narediIntervale(int n ){
    vector<pair<int, int>> intervali;
    for (int k = 0; k < 3; k++){
        for (int i = 0; i < n; i++){
            for (int j = i; j < n; j++){
                intervali.push_back(std::make_pair(i, j));
            }
        }
    }
    return intervali;
}


/*
Trikrat dodamo vsak podinterval [0, n] in ga trikrat odvzamemo
(v drugačnem vrstnem redu, kot smo dodajali).

Vsakič preverimo pravilnost vseh poizvedb.
*/
void preveri_unije(){
    std::default_random_engine re;
    re.seed(12345);
    for (int n = 5; n < 18; n++){
        vector<int> vrednosti(n);
        std::fill(vrednosti.begin(), vrednosti.end(), 0);
        VelikostUnijeZGlavo zGlavo {vrednosti};
        VelikostUnijeSSilo sSilo {vrednosti};

        auto intervali = narediIntervale(n);
        std::shuffle(std::begin(intervali), std::end(intervali), re);
        for (auto const & [i, j] : intervali){
            zGlavo.povecaj(i, j, 1);
            sSilo.povecaj(i, j, 1);
            if (! preveriVse(zGlavo, sSilo, n)){
               cout << "napaka pri dodajanju " << i << " . " << j << endl;
               return;
            }
        }
        std::shuffle(std::begin(intervali), std::end(intervali), re);
        for (auto const & [i, j] : intervali){
            zGlavo.povecaj(i, j, -1);
            sSilo.povecaj(i, j, -1);
            if (! preveriVse(zGlavo, sSilo, n)){
               cout << "napaka pri odstranjevanju" << endl;
               return;
            }
        }
        
    }
}


int main()
{
    preveri_vsote();
    preveri_unije();
}
