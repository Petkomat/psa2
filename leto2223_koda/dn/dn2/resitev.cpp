#include <algorithm>
#include <iostream>
#include <limits>
#include "resitev.h"

using std::cout;
using std::endl;

/*

SKOPIRANO Z VAJ (Z MINIMALNIMI POPRAVKI, KER SEM SE HOTEL NAUČITI "predlog")

*/


template <class T>
PoizvedbeZGlavo<T>::PoizvedbeZGlavo() = default;
template <class T>
PoizvedbeZGlavo<T>::PoizvedbeZGlavo(const vector<T> & seznam, T alfa0, T nevtralnaVrednost, T maksi, T mini)
: nevtralna (nevtralnaVrednost), maxVrednost(maksi), minVrednost(mini)
{
    auto n = (int) seznam.size();
    auto [n_vsa, n_noter] = izracunaj_velikost_polnega_drevesa(n);

    n_vozlisc = n_vsa;
    n_notranjih_vozlisc = n_noter;
    vozlisca = vector<pair<T, pair<T, T>>>(n_vsa);
    std::fill(vozlisca.begin(), vozlisca.end(), std::make_pair(alfa0, std::make_pair(nevtralnaVrednost, nevtralnaVrednost)));
    for (int i = 0; i < n; i++){
        vozlisca[najdi_list(i)].first = seznam[i];
    }
}
template <class T>
PoizvedbeZGlavo<T>::~PoizvedbeZGlavo() = default;

template <class T>
tuple<int, int, vector<int>, vector<int>, int, pair<vector<int>, vector<int>>> PoizvedbeZGlavo<T>::izracunaj_poti_itd(int i, int j) const {
    auto i_levo = najdi_list(i);
    auto i_desno = najdi_list(j);
    auto pot_levo = pot_iz_lista(i_levo);
    auto pot_desno = pot_iz_lista(i_desno);
    auto [i_skupni, poti_pod_skupnim] = skupni_prednik_in_poti_pod_njim(pot_levo, pot_desno);
    return std::make_tuple(i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim);
}

template <class T>
void PoizvedbeZGlavo<T>::povecaj(int i, int j, T x){
    auto [i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim] = izracunaj_poti_itd(i, j);
    auto notranja_vozlisca = notranji_koreni(poti_pod_skupnim.first, poti_pod_skupnim.second);
    // povecaj alfe
    vozlisca[i_levo].first += x;
    if (i_levo != i_desno){
        vozlisca[i_desno].first += x;
    }
    for (auto const & koren : notranja_vozlisca){
        vozlisca[koren].first += x;
    }
    // posodobi bete
    for (auto const & koren : notranja_vozlisca){
        posodobi_beta(koren);
    }
    bool naselSkupnega = false;
    auto dolzina = (int) pot_levo.size();
    for (int k = 0; k < dolzina; k++){
        int levo = pot_levo[k];
        int desno = pot_desno[k];
        if (levo == i_skupni){
            naselSkupnega = true;
        }
        posodobi_beta(levo);
        if (! naselSkupnega){
            posodobi_beta(desno);
        }
    }
}

template<class T>
int PoizvedbeZGlavo<T>::najdi_list(int i) const {
    return i + n_notranjih_vozlisc;
}

template<class T>
bool PoizvedbeZGlavo<T>::je_notranje(int i) const{
    return izracunaj_levega(i) < n_vozlisc;
}

template<class T>
pair<pair<T, T>, int> PoizvedbeZGlavo<T>::odgovor_vzdolz_obeh_poti(int i, int j){
    auto [i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim] = izracunaj_poti_itd(i, j);
    pair<T, T> koncna_vrednost;
    
    // izracunaj vzdolz leve in desne poti
    if (i_levo == i_desno){
        koncna_vrednost = std::make_pair(nevtralna, nevtralna);
    } else if (izracunaj_starsa(i_levo) == izracunaj_starsa(i_desno)){
        koncna_vrednost = zdruziBeti(vozlisca[i_levo].second, vozlisca[i_desno].second); // vozlisca[i_levo].second + vozlisca[i_desno].second;
    } else{
        vector<pair<T, T>> vrednost_levo_desno {
            std::make_pair(nevtralna, nevtralna), std::make_pair(nevtralna, nevtralna)
            //std::make_pair(maxVrednost, minVrednost),
            //std::make_pair(maxVrednost, minVrednost)
        };
        odgovor_vzdolz_ene_poti(0, poti_pod_skupnim.first, vrednost_levo_desno);
        odgovor_vzdolz_ene_poti(1, poti_pod_skupnim.second, vrednost_levo_desno);
        koncna_vrednost = zdruziBeti(vrednost_levo_desno[0], vrednost_levo_desno[1]);
    }
    return std::make_pair(koncna_vrednost, i_skupni);
}

template<class T>
pair<int, bool> PoizvedbeZGlavo<T>::posodobitev_z_notranjim(int i_poti, vector<int> & pot, int j, int vozlisce) {
    int koren = -1;
    bool posodobi = false;
    if (j == 0){
        koren = vozlisce;
    } else if (i_poti == 0 && izracunaj_desnega(vozlisce) != pot[j - 1]){
        // notranje na levi poti
        koren = izracunaj_desnega(vozlisce);
        posodobi = true;
    } else if (i_poti == 1 && izracunaj_levega(vozlisce) != pot[j - 1]){
        // notranje na desni poti
        koren = izracunaj_levega(vozlisce);
        posodobi = true;
    }
    return std::make_pair(koren, posodobi);
}

template<class T>
vector<int> PoizvedbeZGlavo<T>::pot_iz_lista(int iList){
    int trenutni = iList;
    vector<int> pot {trenutni};
    while (trenutni > 0){
        trenutni = izracunaj_starsa(trenutni);
        pot.push_back(trenutni);
    }
    return pot;
}

template<class T>
pair<int, pair<vector<int>, vector<int>>> PoizvedbeZGlavo<T>::skupni_prednik_in_poti_pod_njim(vector<int> & pot_levo, vector<int> & pot_desno){
    auto poti_pod_skupnim = std::make_pair(vector<int> {}, vector<int> {});
    poti_pod_skupnim.first.push_back(pot_levo[0]);
    poti_pod_skupnim.second.push_back(pot_desno[0]);
    int i_skupni = -1;
    auto dolzina = (int) pot_levo.size();
    for (int i = 0; i < dolzina; i++){
        int levo = pot_levo[i];
        int desno = pot_desno[i];
        if (levo == desno){
            i_skupni = i;
            break;
        }
    }
    for(int i = 1; i < i_skupni; i++){
        poti_pod_skupnim.first.push_back(pot_levo[i]);
        poti_pod_skupnim.second.push_back(pot_desno[i]);
    }
    return std::make_pair(pot_levo[i_skupni], poti_pod_skupnim);
}

template<class T>
vector<int> PoizvedbeZGlavo<T>::notranji_koreni(vector<int> & pot_levo, vector<int> & pot_desno){
    vector<int> internal;
    auto dolzina = (int) pot_levo.size();
    for (int i = 1; i < dolzina; i++){
        int levo = pot_levo[i];
        int desno = pot_desno[i];
        if (izracunaj_levega(levo) == pot_levo[i - 1]){
            internal.push_back(izracunaj_desnega(levo));
        }
        if (izracunaj_desnega(desno) == pot_desno[i - 1]){
            internal.push_back(izracunaj_levega(desno));
        }
    }
    return internal;
}

template<class T>
pair<int, int> PoizvedbeZGlavo<T>::izracunaj_velikost_polnega_drevesa(int n){
    int globina = 1;
    int listi = 1;
    while (listi < n){
        listi = listi << 1;
        globina += 1;
    }
    int vsa = (1 << globina) - 1;
    int n_notranjih_vozlisc = vsa - (1 << (globina - 1));
    return std::make_pair(vsa, n_notranjih_vozlisc);
}

template<class T>
int PoizvedbeZGlavo<T>::izracunaj_starsa(int i){
    return (i - 1) >> 1;
}

template<class T>
int PoizvedbeZGlavo<T>::izracunaj_levega(int i){
    return (i << 1) + 1;
}

template<class T>
int PoizvedbeZGlavo<T>::izracunaj_desnega(int i){
    return (i << 1) + 2;
}

// dejanske implementacije
template<class T>
void PoizvedbeZGlavo<T>::posodobi_beta(int i){
    T alfa = vozlisca[i].first;
    if (je_notranje(i)){
        auto [_1, l_beta] = vozlisca[izracunaj_levega(i)];
        auto [_2, d_beta] = vozlisca[izracunaj_desnega(i)];
        vozlisca[i].second = zdruziBeti(l_beta, d_beta);
        vozlisca[i].second.first += alfa;
        vozlisca[i].second.second += alfa;
    } else{
        vozlisca[i].second = std::make_pair(alfa, alfa);
    }
}

template<class T>
T PoizvedbeZGlavo<T>::izracunajVrednost(pair<T,T> beta){    
    auto [spodnja, zgornja] = beta;
    return std::max(abs(spodnja), abs(zgornja));
}

template<class T>
pair<T, T> PoizvedbeZGlavo<T>::zdruziBeti(pair<T, T> & beta1, pair<T, T> & beta2){
    T mini = std::min(beta1.first, beta2.first);
    T maxi = std::max(beta1.second, beta2.second);
    pair<T, T> answer = std::make_pair(mini, maxi);
    return answer;
}

template<class T>
void PoizvedbeZGlavo<T>::pristejAlfa(pair<T, T> & beta, T & alfa){
    beta.first += alfa;
    beta.second += alfa;
}

template<class T>
int PoizvedbeZGlavo<T>::odgovori(int i, int j){
    auto [koncna_vrednost, i_skupni] = odgovor_vzdolz_obeh_poti(i, j);
    // popravi z alfami skupnih prednikov
    while (i_skupni >= 0){
        pristejAlfa(koncna_vrednost, vozlisca[i_skupni].first);
        i_skupni = izracunaj_starsa(i_skupni);
    }
    return izracunajVrednost(koncna_vrednost);
}

template<class T>
void PoizvedbeZGlavo<T>::odgovor_vzdolz_ene_poti(int i, vector<int> & pot, vector<pair<T, T>> & vrednost_levo_desno){
    auto n = (int) pot.size();
    for (int j = 0; j < n; j++){
        auto [koren, posodobi_z_notranjim] = posodobitev_z_notranjim(i, pot, j, pot[j]);
        if (posodobi_z_notranjim){
            vrednost_levo_desno[i] = zdruziBeti(vrednost_levo_desno[i], vozlisca[koren].second);
        }
        vrednost_levo_desno[i].first += vozlisca[pot[j]].first;
        vrednost_levo_desno[i].second += vozlisca[pot[j]].first;
    }
}


/*
 * 
 * IMPLEMENTACIJA REŠITVE 
 * 
*/

Resitev::Resitev(vector<long long> emsoOtrok)
: drevo()
{
    // shrani emso vrednosti
    int i = 0;
    for (auto const & emso : emsoOtrok){
        emsoti[i] = emso;
        i++;
        n_otrok++;
    }
    // naredi drevo
    vector<int> nicle(n_otrok - 1);
    std::fill(nicle.begin(), nicle.end(), 0);
    drevo = PoizvedbeZGlavo<int>(nicle, 0, 0, std::numeric_limits<int>::max() - 100, std::numeric_limits<int>::min() + 100);
}

int Resitev::obdelaj(const string & ukaz){
    // ukaz je oblike "o" emso0 emso1 ali "p" emso0 emso1 x 
    long long emso0, emso1;
    if (ukaz[0] == 'p'){
        int x;
        std::sscanf(ukaz.c_str(), "p %lld %lld %d", &emso0, &emso1, &x);
        posodobi(emso0, emso1, x);
        return 0;
    } else{
        std::sscanf(ukaz.c_str(), "o %lld %lld", &emso0, &emso1);
        return odgovori(emso0, emso1);
    }
}


int Resitev::najdiEmso(const long long & emso){
    long long * iPointer = std::lower_bound(std::begin(emsoti), std::begin(emsoti) + n_otrok, emso);
    return std::distance(std::begin(emsoti), iPointer);
}

void Resitev::posodobi(const long long & emso0, const long long & emso1, const int & x){
    int i = najdiEmso(emso0);
    int j = najdiEmso(emso1);
    if (i > 0){
        drevo.povecaj(i - 1, i - 1, x);
    }
    if (j > i){
        drevo.povecaj(i, j - 1, 1);
    }
    if (j < n_otrok - 1){
        drevo.povecaj(j, j, -(x + j - i));
    }
    
}

int Resitev::odgovori(const long long & emso0, const long long & emso1){
    int i = najdiEmso(emso0);
    int j = najdiEmso(emso1);
    return drevo.odgovori(i, j - 1);
}
