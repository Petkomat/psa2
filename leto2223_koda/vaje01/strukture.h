#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>

/* 
using namespace std;

TO bi omogočilo

string x
namesto
std::string x

*/

using std::string;
using std::vector;

class Vozlisce {

public:
    bool jeBeseda;
    // ker ne ppoznam dolžine v naprej, ne morem reči tega:
    // Vozlisce nasledniki[25]

    std::map<char, Vozlisce> nasledniki;

    Vozlisce(){
        jeBeseda = false;
        // ker je nasledniki map, je že inicializiran na prazno
    }

    /*
        Radi bi, da je toString(vozlisce) nekaj takega: 

        <crka>: je/ni beseda
        {
            a: opis naslednika pri a
            b: opis naslednika pri b
            ...
        }
    */
    string toString(int zamik=0, char crkaTu=' ') const {
        // nekoc bom znal format
        string nizZamik = string(zamik, ' ');
        string deli = nizZamik + string(1, crkaTu) + ": " + (jeBeseda ? "beseda" : "-") + "\n";
        deli += nizZamik + "{";
        // nasledniki
        for(auto const& [crka, naslednik] : nasledniki){
            deli += "\n";
            deli += naslednik.toString(zamik + 2, crka);
        }
        deli += (nasledniki.size() ? nizZamik : "") + "}\n";
        return deli;
    }
};


class Trie {

    Vozlisce koren;

public:
    Trie(){
        koren = Vozlisce();
    };

    int steviloVozlisc() const{
        vector<Vozlisce> sklad;
        sklad.push_back(koren);
        int stevilo = 0;
        while (! sklad.empty()){
            Vozlisce trenutno = sklad.back();
            sklad.pop_back();
            stevilo++;
            // ni treba for(std::pair<char, Vozlisce> par : trenutno.nasledniki),
            // raje preprosto
            for(auto const& [_, naslednik] : trenutno.nasledniki){
                sklad.push_back(naslednik);
            }
        }
        return stevilo;
    }

    void vstavi(string const& beseda){
        Vozlisce* trenutnoPTR = &koren;
        // poskrbimo za nadaljevanja
        for (auto const& crka : beseda){
            (*trenutnoPTR).nasledniki.try_emplace(crka /* emplacing a default constructed object */);
            trenutnoPTR = &(*trenutnoPTR).nasledniki.at(crka);
        }
        // na koncu označimo, da se je beseda tu končala
        (*trenutnoPTR).jeBeseda = true;
    }

    void vstaviIzDatoteke(string const& pot){
        std::fstream datoteka;
        datoteka.open(pot, std::ios::in); // odpri datoteko
        if (datoteka.is_open()){
            string vrstica;
            while(getline(datoteka, vrstica)){  // preberi vrstico in jo shrani v spremenljivko vrstica
                vstavi(vrstica);
                std::cout << vrstica << " je dolga " << vrstica.length()  << std::endl;
            }
            datoteka.close(); // zapri datoteko
        }
    }

    vector<string> nastejBesedeUrejeno() const{
        vector<string> besede;
        // dva sklada: vozlisca in pripadajoce delne besede
        vector<Vozlisce> vozliscaTODO;
        vector<string> delneBesede;
        // na zacetku: koren in prazna beseda
        vozliscaTODO.push_back(koren);
        delneBesede.emplace_back("");
        // DFS: najprej koren, potem po vrsti od leve proti desni
        while(! vozliscaTODO.empty()){
            Vozlisce trenutno = vozliscaTODO.back();
            vozliscaTODO.pop_back();
            string pripadajocaBeseda = delneBesede.back();
            delneBesede.pop_back();
            if (trenutno.jeBeseda){
                besede.push_back(pripadajocaBeseda);
            }
            // na sklad je treba dati v obratnem abecednem redu
            for (char crka : "žzvutšsrponmlkjihgfedčcba"){
                if (trenutno.nasledniki.count(crka)){
                    vozliscaTODO.push_back(trenutno.nasledniki.at(crka));
                    delneBesede.push_back(pripadajocaBeseda + crka);
                }
            }
        }
        return besede;
    }

    string toString() const{
        return "Trie:\n" + koren.toString();
    }
};
