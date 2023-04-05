#include <string>

using std::string;


class Resitev {

    // rezerviramo dovolj prostora
    static const int N_DODAJANJ = 1000 * 1000 * 10;
    static const int SOSEDJE = 4;
    static const int NIVOJI = 20;
    int urejeniSeznami[NIVOJI][N_DODAJANJ];
    int steviloSeznamov = 0;
    int casiDodajanj[N_DODAJANJ];
    int steviloDodajanj = 0;
    int konecZadnjeStrukture[NIVOJI];

    int najblizji[SOSEDJE];
    
    int odgovori(const int & t, const int & stevilo);
    void vstavi(int t, int stevilo);

    void posodobiKvalitete(const int & stevilo, int nivo, int zacetek, int konec);

    void toString();


// NE SPREMINJAJ KODE OD TU NAPREJ
public:
    Resitev();

    /*
    Ukaz je oblike "v 12 1000" (vstavi 1000 ob času 12),
    ali pa         "p 6 100" (poizvedba okoli 100 za čas 6).
    
    Vrne naj odgovor na poizvedbo (če je ukaz poizvedba), ali pa karkoli (npr. 0).
    */
    int obdelaj(const string & ukaz);
};