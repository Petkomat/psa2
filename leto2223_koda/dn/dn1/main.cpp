#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

#include "resitev.h"


using std::vector;
using std::string;
using std::cout;
using std::endl;


vector<string> preberiVrstice(const string & pot){
    vector<string> vrstice;

    std::fstream datoteka;
    datoteka.open(pot, std::ios::in);
    if (datoteka.is_open()){
        string vrstica;
        while(getline(datoteka, vrstica)){
            vrstice.push_back(vrstica);
        }
        datoteka.close();
    } else {
        cout << "Nisem mogel prebrati datoteke " << pot << "\nLep pozdrav, tvoj c++" << endl;
    }
    return vrstice;
}


void preveri(const string & naloga, long long maxTrajanjeMS){
    cout << "Preverjam " << naloga << endl;

    auto ukazi = preberiVrstice(naloga + ".in");
    auto resitve = preberiVrstice(naloga + ".out");
    vector<int> odgovori(resitve.size());

    auto t0 = std::chrono::high_resolution_clock::now();
    int iPoizvedba = 0;
    Resitev R;
    // Slaba R;

    for (const auto & ukaz : ukazi){
        if (ukaz[0] == 'p'){
            odgovori[iPoizvedba] = R.obdelaj(ukaz);
            iPoizvedba++;
        } else{
            R.obdelaj(ukaz);
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    // milisekunde
    auto trajanje = (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count() / 1000;
    int napaka = -1;
    int nNapak = 0;
    for (int i = 0; i < resitve.size(); i++){
        if (std::stoi(resitve[i]) != odgovori[i]){
            if (nNapak == 0){
                // najdi poizvedbo z danim indeksom
                iPoizvedba = 0;
                string poizvedba;
                for (const auto & ukaz : ukazi){
                    if (ukaz[0] == 'p'){
                        if (iPoizvedba == i){
                            poizvedba = ukaz;
                            break;
                        }
                        iPoizvedba++;
                    }
                }
                cout << "Prva napaka se zgodi pri poizvedbi st. " << i << " (>= 0): " << poizvedba << "."
                     << " Pravilen odgovor = " << resitve[i] << " != " << odgovori[i] << endl;
            }
            nNapak++;
        }
    }
    if (nNapak == 0){
        cout << "Vsi odgovori so pravilni." << endl;
    } else{
        cout << "Skupno stevilo napak: " << nNapak << endl;
    }
    string jeNi = trajanje <= maxTrajanjeMS ? "je" : "ni";
    cout << "Resitev " << jeNi << " dovolj hitra: " << trajanje << " ms (dovoljeno: " << maxTrajanjeMS << " ms)" << endl;
}


long long izracunajMaxT(){
    auto t0 = std::chrono::high_resolution_clock::now();
    int vsota = 0;
    for (int i = 0; i < 1000 * 1000 * 10; i++){
        int j = i;
        int stevec = 0;
        while (j > 0){
            j /= 2;
            stevec++;
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    auto trajanje = (std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0)).count() / 1000;
    return trajanje;
}

int main(){
    // Da se prilagodimo hitrim in pocasnim racunalnikom:
    // namesto long long maxT = 500; raje dinamicno dolocimo:
    long long maxT = izracunajMaxT();
    vector<string> naloge = {
        "nalogaA0", "nalogaA1", "nalogaA2", "nalogaA3",
        "nalogaB0", "nalogaB1", "nalogaB2"
    };
       
    for (auto const & naloga : naloge){
        preveri(naloga, maxT);
        cout << endl;
    }
    return 0;
}