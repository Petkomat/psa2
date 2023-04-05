#include <string>


using std::string;


class Slaba {

// vemo, da mora delovati do n = N_DODAJANJ ... .. log n = ...

    static const int N_DODAJANJ = 1000 * 1000 * 10;
    static const int SOSEDJE = 4;

    int casi[N_DODAJANJ];
    int vrednosti[N_DODAJANJ];
    int steviloDodajanj = 0;

    int najblizji[SOSEDJE];
    
    int odgovori(const int & t, const int & stevilo);

    int stevecP = 0;

// NE SPREMINJAJ KODE OD TU NAPREJ
public:
    Slaba();

    /*
    Ukaz je oblike "v 12 1000" (vstavi 1000 ob času 12),
    ali pa         "p 6 100" (poizvedba okoli 100 ob času 6).
    
    Vrne naj odgovor na poizvedbo (če je ukaz poizvedba), ali pa karkoli (npr. 0).
    */
    int obdelaj(const string & ukaz);
};