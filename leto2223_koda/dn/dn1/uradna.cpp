#include <iostream>
#include <algorithm>
#include "resitev.h"


using std::cout; using std::endl;


Resitev::Resitev() {
    for (int nivo = 0; nivo < NIVOJI; nivo++){
        konecZadnjeStrukture[nivo] = 0;
    }
};


int Resitev::obdelaj(const string & ukaz){
    int i = ukaz.find(' ');
    int j = ukaz.rfind(' ');
    int x = std::stoi(ukaz.substr(i, j - i));
    int y = std::stoi(ukaz.substr(j));
    if (ukaz[0] == 'p'){
        return odgovori(x, y);
    } else{
        vstavi(x, y);
        return 0;
    }
}


/*
Posodobimo kvalitete za dano stevilo s kandidati iz urejeniSeznami[nivo][zacetek: konec + 1],
tj. iz strukture na nivoju nivo, ki pripada ustreznemu času.

Ideja:
1) Z bisekcijo najdi mesto, kamor bi v urejen seznam vstavili stevilo
2) Poglej v levo in desno (največ SOSEDJE-krat) in posodobi trenutne kvalitete, če je treba
*/
void Resitev::posodobiKvalitete(const int & stevilo, int nivo, int zacetek, int konec){
    // z bisekcijo najdemo mesto, kamor ga vstavimo v seznam (razen ce je stevilo vecje od vseh),
    // a za to poskrbi desnaMeja
    int i = zacetek;
    int j = konec;
    while (j > i){
        int vmes = (i + j) / 2;
        if (urejeniSeznami[nivo][vmes] < stevilo){
            i = vmes + 1;
        } else{
            j = vmes;
        }
    }
    // Njegova najblizja soseda:
    int levaMeja = i - 1;
    int desnaMeja = i;
    for (int ponovitev = 0; ponovitev < SOSEDJE; ponovitev++){
        int izbrana = -1;
        if (levaMeja >= zacetek && desnaMeja <= konec){
            int levaKvaliteta = abs(stevilo - urejeniSeznami[nivo][levaMeja]);
            int desnaKvaliteta = abs(stevilo - urejeniSeznami[nivo][desnaMeja]);
            if (levaKvaliteta <= desnaKvaliteta){
                izbrana = levaKvaliteta;
                levaMeja--;
            } else {
                izbrana = desnaKvaliteta;
                desnaMeja++;
            }
        } else if (levaMeja >= zacetek){
            izbrana = abs(stevilo - urejeniSeznami[nivo][levaMeja]);
            levaMeja--;
        } else if (desnaMeja <= konec){
            izbrana = abs(stevilo - urejeniSeznami[nivo][desnaMeja]);
            desnaMeja++;
        } else {
            break;
        }
        // izbrana = 12
        // najblizji: 15, 13, 13, 7
        // --->       13, 13, 12, 7
        int mesto = -1;
        while (mesto + 1 < SOSEDJE && (najblizji[mesto + 1] > izbrana || najblizji[mesto + 1] < 0)){
            mesto++;
        }
        for (int k = 1; k <= mesto; k++){
            najblizji[k - 1] = najblizji[k];
        }
        if (mesto >= 0){
            najblizji[mesto] = izbrana;
        }
    }
}



int Resitev::odgovori(const int & t, const int & stevilo) {
    if (steviloDodajanj == 0){
        return -1;
    }
    // najdi zadnji indeks vstavljanja, ki je pred t: bisekcija
    int n = 0;
    int konec = steviloDodajanj; // konec je najmanjši znan neprimeren indeks (***) - razen ce je t >= max(sez)
    int vmes;
    while (konec > n){
        vmes = (n + konec) / 2;
    	if (casiDodajanj[vmes] > t){
            konec = vmes;       // (***) zato tukaj brez -1
        } else{
            n = vmes + 1;
        }
    }
    // resetiramo sosede
    for (int s = 0; s < SOSEDJE; s++){
        najblizji[s] = -1;
    }
    // Ugotovimo, kam moramo pogledati:
    // Po n-tem dodajanju, npr. n = 21 (10101) so aktivne strukture na nivojih 0, 3, in 5
    // Katere: i-ti nivo se aktivira j-tič po dodajanju 2 ** i + j * (2 ** (i + 1))
    // npr. nivo 1: 2, 6, 10, 14, ...
    // 2**i + j * 2 ** (i + 1) <= n
    // j <= (n - 2 ** i) / (2 ** (i + 1)) = (n ^ potenca2) >> (i + 1)
    int potenca2 = 1;
    int i = 0;
    while (potenca2 <= n){
        if (n & potenca2){
            int j = (n ^ potenca2) >> (i + 1);
            int zacetek = potenca2 * j;
            posodobiKvalitete(stevilo, i, zacetek, zacetek + potenca2 - 1);
        }
        potenca2 <<= 1;
        i++;
    }
    int odgovor = 0;
    for (auto const & sos : najblizji){
        if (sos > 0){
            odgovor += sos;
        }
    }
    // toString();
    return odgovor;
}


/*
Vstavi novo število ob času t v strukturo.
Ideja:
1) Čas t doda na konec seznama časov.
2) Število vstavi v poldinamično strukturo tako, da ne pobriše prejšnjih, ampak jo doda na konec 
   danega nivoja (če je dani nivo poln po tem dodajanju).
*/
void Resitev::vstavi(int t, int stevilo){
    casiDodajanj[steviloDodajanj] = t;
    // najdemo ciljni nivo: mesto prve ničle v dvojiškem zapisu (prejšnjega) števila dodajanj
    int potenca2 = 1;
    int ciljniNivo = -1;
    for (int nivo = 0; nivo < NIVOJI; nivo++){
        if ((steviloDodajanj & potenca2) == 0){
            ciljniNivo = nivo;
            break;
        }
        potenca2 <<= 1;
    }
    // prepišemo trenutno aktivne strukture na pravo mesto ...
    potenca2 = 1;
    int tekociZacetekNove = konecZadnjeStrukture[ciljniNivo];
    for (int nivo = 0; nivo < ciljniNivo; nivo++){
        if (steviloDodajanj & potenca2){
            int j = (steviloDodajanj ^ potenca2) >> (nivo + 1);
            int zacetek = j * potenca2;
            // poberi elemente [zacetek, zacetek + potenca2)
            for (int k = 0; k < potenca2; k++){
                urejeniSeznami[ciljniNivo][tekociZacetekNove + k] = urejeniSeznami[nivo][zacetek + k];
            }
            tekociZacetekNove += potenca2;
        }
        potenca2 <<= 1;
    }
    // ... in dodamo na konec še stevilo
    urejeniSeznami[ciljniNivo][tekociZacetekNove] = stevilo;
    tekociZacetekNove++;
    // uredimo ustrezen odsek
    std::sort(
        std::begin(urejeniSeznami[ciljniNivo]) + konecZadnjeStrukture[ciljniNivo],
        std::begin(urejeniSeznami[ciljniNivo]) + tekociZacetekNove
    );
    konecZadnjeStrukture[ciljniNivo] = tekociZacetekNove;
    steviloDodajanj++;
}


void Resitev::toString(){
    cout << "Stevilo dodajanj: " << steviloDodajanj << endl;
    cout << "Casi dodajanj: ";
    for (int i = 0; i < steviloDodajanj; i++){
        cout << casiDodajanj[i] << ", ";
    }
    cout << endl;
    cout << "konciStruktur: ";
    for (auto const & konec : konecZadnjeStrukture){
        cout << konec << ", ";
    }
    cout << endl;
    cout << "Urejeni seznami: " << endl;
    for (int i = 0; i < NIVOJI; i++){
        if (konecZadnjeStrukture[i] == 0){
            cout << "prazno tu" << endl;
        } else{
            for (int j = 0; j < konecZadnjeStrukture[i]; j++){
                cout << urejeniSeznami[i][j] << ", ";
            }
            cout << endl;
        }
    }
    cout << "Kvalitete bližnjih: ";
    for (auto const & x : najblizji){
        cout << x << ", ";
    }
    cout << endl << endl;
}