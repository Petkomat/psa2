#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include "kdd.h"

using std::vector;
using std::pair;


bool urediGledeNaPrvoKomponento (vector<double> x, vector<double> y) {
    return x[0] < y[0];
}

/*
Generira n vektorjev dimenzije d na dva načina:
- povsem naključno: zgenerira enekomerno naključno po prostoru [zamik, zamik + 1)^d
- ne povsem naključno: zgenerira enakomerno na diagonali tega prostora in jih uredi (po normi = prvi koordnati),
                       zato da je "težko" povečevati KD-drevesa na enostaven način.
*/
vector<vector<double>> generirajPodatke(int n, int d, double zamik, bool povsemNakljucno, long long seed){
    std::default_random_engine re;
    re.seed(seed);
    std::uniform_real_distribution unif(zamik, zamik + 1.0);
    vector<vector<double>> xs;
    for (int i = 0; i < n; i++){
        vector<double> x;
        if (povsemNakljucno){
            for (int j = 0; j < d; j++){
                x.push_back(unif(re));
            }
        } else{
            double r = unif(re);
            for (int j = 0; j < d; j++){
                x.push_back(r);
            }
        }
        xs.push_back(x);
    }
    if (!povsemNakljucno){
        std::sort(xs.begin(), xs.end(), urediGledeNaPrvoKomponento);
    }
    return xs;
}


pair<vector<vector<double>>, vector<vector<double>>> testniPrimer(int nVstavi, int nDodatnaIskanja, int d, double zamikIskanj, bool povsemNakljucno){
    return pair(generirajPodatke(nVstavi, d, 0.0, povsemNakljucno, 1234), generirajPodatke(nDodatnaIskanja, d, zamikIskanj, true, 12345));
}


vector<vector<double>> najdiBrutResitve(const vector<vector<double>> & xsVstavi, const vector<vector<double>> & xsIsci){
    vector<vector<double>> resitve;
    return resitve;
    // auto nVstavi = (int) xsVstavi.size();
    // for (int iIsci = 0; iIsci < xsIsci.size(); iIsci++){
    //     auto const & x = xsIsci[iIsci];
    //     int optI = 0;
    //     double optR = std::numeric_limits<double>::infinity();
    //     double r;
    //     int meja = iIsci < nVstavi ? iIsci + 1 : nVstavi; // iVstavi <= iIsci, iVstavi < nVstavi 
    //     for (int iVstavi = 0; iVstavi < meja; iVstavi++){
    //         r = KDDrevo::izracunajEvklidsko2(xsVstavi[iVstavi], x);
    //         if (r < optR){
    //             optI = iVstavi;
    //             optR = r;
    //         }
    //     }
    //     resitve.push_back(xsVstavi[optI]);
    // }
    // return resitve;
}


/*
Vstavi, poisce, vstavi, poisce, ..., vstavi, poisce, potem pa samo se isce (ker je vec iskanj kot vstavljanj).
*/
long long preizkusiSemidinamicno(const vector<vector<double>> & xsVstavi, const vector<vector<double>> & xsIsci, const vector<vector<double>> & praviSosedje){
    int nVstavi = xsVstavi.size();
    int nIsci = xsIsci.size();
    int iVstavi = 0;
    int iIsci = 0;
    KDDSemidinamicno gozd;
    //auto gozd = KDDrevo::narediDrevo(xsVstavi, 0);
    vector<double> sosed;
    long long trajanje = 0;
    while (iVstavi < nVstavi || iIsci < nIsci){
        auto t0 = std::chrono::high_resolution_clock::now();
        if (iVstavi < nVstavi){
            gozd.vstavi(xsVstavi[iVstavi]);
            iVstavi++;
        }
        if (iIsci < nIsci){
            sosed = gozd.najdi(xsIsci[iIsci]).first;
            iIsci++;
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        trajanje += (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count();
        continue;
        if (iIsci <= nIsci && KDDrevo::izracunajEvklidsko2(sosed, praviSosedje[iIsci - 1]) > 1e-8){
            iIsci--;
            std::cout << "Nepravilen sosed pri poizvedbi " << iIsci << std::endl;
            return -1;
        }
    }
    return trajanje;
}

void izpisiOdziv(long long t, int primer){
    if (t < 0){
        std::cout << "Neveljeven rezultat v primeru " << primer << std::endl;
    } else{
        std::cout << "Čas [ms] za primer " << primer << ": " << t / 1000 << std::endl;
    }
}


int main()
{
    int nVstavi = 1e4;
    int nIsci = 1e4;
    int d = 2;
    
    // lahki podatki: vse random
    auto [xsVstavi1, xsLeIsci1] = testniPrimer(nVstavi, nIsci, d, 0.0, true);
    auto resitve1 = najdiBrutResitve(xsVstavi1, xsLeIsci1);
    // tezji podatki: dodajanje je potencialno tecno
    auto [xsVstavi2, xsLeIsci2] = testniPrimer(nVstavi, nIsci, d, 0.0, false);
    auto resitve2 = najdiBrutResitve(xsVstavi2, xsLeIsci2);
    // tezji podatki: iskanje je tecno, ker so tocke dalec
    auto [xsVstavi3, xsLeIsci3] = testniPrimer(nVstavi, nIsci, d, 100.0, false);
    auto resitve3 = najdiBrutResitve(xsVstavi3, xsLeIsci3);

    auto t1 = preizkusiSemidinamicno(xsVstavi1, xsLeIsci1, resitve1);
    izpisiOdziv(t1, 1);
    auto t2 = preizkusiSemidinamicno(xsVstavi2, xsLeIsci2, resitve2);
    izpisiOdziv(t2, 2);
    auto t3 = preizkusiSemidinamicno(xsVstavi3, xsLeIsci3, resitve3);
    izpisiOdziv(t3, 3);
    return 0;
}
