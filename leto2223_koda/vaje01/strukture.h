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
        auto nizZamik = string(zamik, ' ');
        string deli = nizZamik + string(1, crkaTu) + " (int: " + std::to_string(crkaTu + 0) + "): " + (jeBeseda ? "beseda" : "-") + "\n";
        deli += nizZamik + "{";
        // nasledniki
        for(auto const& [crka, naslednik] : nasledniki){
            deli += "\n";
            deli += naslednik.toString(zamik + 4, crka);
        }
        deli += (nasledniki.size() ? nizZamik : "") + "}\n";
        return deli;
    }
};


class Trie {

    Vozlisce koren;
    int steviloCrk = 0;  //  v resnici bajtov, ker je npr. č predstavljen z dvema

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
        steviloCrk += beseda.size();
        Vozlisce* trenutnoPTR = &koren;
        // poskrbimo za nadaljevanja
        for (auto const& crka : beseda){
            (*trenutnoPTR).nasledniki.try_emplace(crka /* emplacing a default constructed object - predlagal sonar */);
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
            }
            datoteka.close(); // zapri datoteko
        }
    }

    vector<string> nastejBesedeUrejeno() const{
        vector<string> besede;
        // dva sklada: vozlisca in pripadajoce delne besede
        vector<const Vozlisce*> vozliscaTODO;
        vector<string> delneBesede;
        // na zacetku: koren in prazna beseda
        vozliscaTODO.push_back(&koren);
        delneBesede.emplace_back("");
        // DFS: najprej koren, potem po vrsti od leve proti desni
        while(! vozliscaTODO.empty()){
            const Vozlisce* trenutno = vozliscaTODO.back();
            vozliscaTODO.pop_back();
            string pripadajocaBeseda = delneBesede.back();
            delneBesede.pop_back();
            if ((*trenutno).jeBeseda){
                besede.push_back(pripadajocaBeseda);
            }
            // na sklad je treba dati v obratnem abecednem redu
            // pozor: čšž sta 2 bajta: 
            // č: -60, -115
            // š: -59, -95
            // ž: -59, -67
            // Ker imata š in ž isti prvi bajt, ureditev besed na koncu vseeno ni čisto prava:
            // kneški bo prišel na vrsto za knez (ker -59 (del 'š') dodamo že pri 'ž')
            // Rešitev za to: pogledati bi bilo treba še eno vozlišče naprej ...
            bool dodal_minus59 = false;
            for (char crka : "žzvutšsrponmlkjihgfedčcba"){
                if (bool najDodam = (*trenutno).nasledniki.count(crka) && (crka != -59 || !dodal_minus59); 
                    !najDodam){
                    continue;
                }
                vozliscaTODO.push_back(&((*trenutno).nasledniki.at(crka)));
                delneBesede.push_back(pripadajocaBeseda + crka);
                if (crka == -59){
                    dodal_minus59 = true;
                }
            }
        }
        return besede;
    }

    string toString() const{
        return "Trie:\n" + koren.toString();
    }

    int getSteviloCrk() const{
        return steviloCrk;
    }
};
