#include "sip.h"



/***************************************************
 * 
 *     Abstraktni razredi
 * 
******************************************************/

StaticneIntervalnePoizvedbe::StaticneIntervalnePoizvedbe() = default;
StaticneIntervalnePoizvedbe::~StaticneIntervalnePoizvedbe() = default;


/***************************************************
 * 
 *     Abstraktni razredi: SILA
 * 
******************************************************/

PoizvedbeSSilo::PoizvedbeSSilo() = default;

PoizvedbeSSilo::PoizvedbeSSilo(const vector<int> & seznam)
: vrednosti(seznam) // ga skopira
{
    
}

void PoizvedbeSSilo::povecaj(int i, int j, int x){
    for (int k = i; k <= j; k++){
        vrednosti[k] += x;
    }
}


/***************************************************
 * 
 *     Abstraktni razredi: GLAVA
 * 
******************************************************/

PoizvedbeZGlavo::PoizvedbeZGlavo() = default;

PoizvedbeZGlavo::PoizvedbeZGlavo(const vector<int> & seznam, int alfa0, int nevtralnaVrednost){
    auto n = (int) seznam.size();
    auto [n_vsa, n_noter] = izracunaj_velikost_polnega_drevesa(n);

    n_vozlisc = n_vsa;
    n_notranjih_vozlisc = n_noter;
    vozlisca = vector<pair<int, int>>(n_vsa);
    std::fill(vozlisca.begin(), vozlisca.end(), std::make_pair(alfa0, nevtralnaVrednost));
    listi_poddrevesa = vector<int>(n_vsa);
    std::fill(listi_poddrevesa.begin(), listi_poddrevesa.end(), 0);
    for (int i = 0; i < n; i++){
        vozlisca[najdi_list(i)].first = seznam[i];
    }
}

PoizvedbeZGlavo::~PoizvedbeZGlavo() = default;


tuple<int, int, vector<int>, vector<int>, int, pair<vector<int>, vector<int>>> PoizvedbeZGlavo::izracunaj_poti_itd(int i, int j) const {
    auto i_levo = najdi_list(i);
    auto i_desno = najdi_list(j);
    auto pot_levo = pot_iz_lista(i_levo);
    auto pot_desno = pot_iz_lista(i_desno);
    auto [i_skupni, poti_pod_skupnim] = skupni_prednik_in_poti_pod_njim(pot_levo, pot_desno);
    return std::make_tuple(i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim);
}

void PoizvedbeZGlavo::povecaj(int i, int j, int x){

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

int PoizvedbeZGlavo::najdi_list(int i) const {
    return i + n_notranjih_vozlisc;
}


int PoizvedbeZGlavo::n_listov_poddrevo(int i) {
    if (listi_poddrevesa[i] > 0){
        return listi_poddrevesa[i];
    }
    int i_levo = i;
    int i_desno = i;
    // pojdi levo
    while (je_notranje(i_levo)){
        i_levo = izracunaj_levega(i_levo);
    }
    // pojdi desno
    while (je_notranje(i_desno)){
        i_desno = izracunaj_desnega(i_desno);
    }
    listi_poddrevesa[i] = i_desno - i_levo + 1;
    return listi_poddrevesa[i];
}

bool PoizvedbeZGlavo::je_notranje(int i) const{
    return izracunaj_levega(i) < n_vozlisc;
}


tuple<int, int, int> PoizvedbeZGlavo::odgovor_vzdolz_obeh_poti(int i, int j){
    auto [i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim] = izracunaj_poti_itd(i, j);
    int koncna_vrednost;
    
    // izracunaj vzdolz leve in desne poti
    if (i_levo == i_desno){
        koncna_vrednost = 0;
    } else if (izracunaj_starsa(i_levo) == izracunaj_starsa(i_desno)){
        koncna_vrednost = vozlisca[i_levo].second + vozlisca[i_desno].second;
    } else{
        vector<int> vozlisca_levo_desno {0, 0};
        vector<int> vrednost_levo_desno {0, 0};
        odgovor_vzdolz_ene_poti(0, poti_pod_skupnim.first, vrednost_levo_desno, vozlisca_levo_desno);
        odgovor_vzdolz_ene_poti(1, poti_pod_skupnim.second, vrednost_levo_desno, vozlisca_levo_desno);
        koncna_vrednost = vrednost_levo_desno[0] + vrednost_levo_desno[1];
    }
    int aktivni_listi = i_desno - i_levo + 1;
    return std::make_tuple(koncna_vrednost, aktivni_listi, i_skupni);
}



pair<int, bool> PoizvedbeZGlavo::posodobitev_z_notranjim(int i_poti, vector<int> & pot, int j, int vozlisce) {
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


vector<int> PoizvedbeZGlavo::pot_iz_lista(int iList){
    int trenutni = iList;
    vector<int> pot {trenutni};
    while (trenutni > 0){
        trenutni = izracunaj_starsa(trenutni);
        pot.push_back(trenutni);
    }
    return pot;
}


pair<int, pair<vector<int>, vector<int>>> PoizvedbeZGlavo::skupni_prednik_in_poti_pod_njim(vector<int> & pot_levo, vector<int> & pot_desno){
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

vector<int> PoizvedbeZGlavo::notranji_koreni(vector<int> & pot_levo, vector<int> & pot_desno){
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

pair<int, int> PoizvedbeZGlavo::izracunaj_velikost_polnega_drevesa(int n){
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

int PoizvedbeZGlavo::izracunaj_starsa(int i){
    return (i - 1) >> 1;
}

int PoizvedbeZGlavo::izracunaj_levega(int i){
    return (i << 1) + 1;
}

int PoizvedbeZGlavo::izracunaj_desnega(int i){
    return (i << 1) + 2;
}



/***********************************************
 * 
 *           Vsote podintervalov
 * 
 **********************************************/


VsotaSSilo::VsotaSSilo(const vector<int> & seznam)
: PoizvedbeSSilo(seznam)
{

}
VsotaSSilo::~VsotaSSilo() = default;


int VsotaSSilo::odgovori(int i, int j){
    int vsota = 0;
    for (int k = i; k <= j; k++){
        vsota += vrednosti[k];
    }
    return vsota;
}



VsotaZGlavo::VsotaZGlavo(const vector<int> & seznam)
: PoizvedbeZGlavo(seznam, 0, 0)
{
    for (int i = n_vozlisc - 1; i >= 0; i--){
        posodobi_beta(i);
    }
}
VsotaZGlavo::~VsotaZGlavo() = default;

void VsotaZGlavo::posodobi_beta(int i){
    if (je_notranje(i)){
        int l_beta = vozlisca[izracunaj_levega(i)].second;
        int d_beta = vozlisca[izracunaj_desnega(i)].second;
        int alfa = vozlisca[i].first;
        vozlisca[i].second = l_beta + d_beta + alfa * n_listov_poddrevo(i);
    } else{
        vozlisca[i].second = vozlisca[i].first;
    }
}

int VsotaZGlavo::odgovori(int i, int j){
    auto [koncna_vrednost, aktivni_listi, i_skupni] = odgovor_vzdolz_obeh_poti(i, j);
    // popravi z alfami skupnih prednikov
    while (i_skupni >= 0){
        koncna_vrednost += vozlisca[i_skupni].first * aktivni_listi;
        i_skupni = izracunaj_starsa(i_skupni);
    }
    return koncna_vrednost;
}

void VsotaZGlavo::odgovor_vzdolz_ene_poti(int i, vector<int> & pot, vector<int> & vrednost_levo_desno, vector<int> & vozlisca_levo_desno){
    auto n = (int) pot.size();
    for (int j = 0; j < n; j++){
        auto [koren, posodobi_z_notranjim] = posodobitev_z_notranjim(i, pot, j, pot[j]);
        if (posodobi_z_notranjim){
            vrednost_levo_desno[i] += vozlisca[koren].second;
        }
        if (koren >= 0){
            vozlisca_levo_desno[i] += n_listov_poddrevo(koren);
        }
        vrednost_levo_desno[i] += vozlisca[pot[j]].first * vozlisca_levo_desno[i];
    }
}


/***********************************************
 * 
 *           Velikost unije
 * 
 **********************************************/

VelikostUnijeSSilo::VelikostUnijeSSilo(const vector<int> & seznam)
: PoizvedbeSSilo(seznam)
{

}

VelikostUnijeSSilo::~VelikostUnijeSSilo() = default;

int VelikostUnijeSSilo::odgovori(int i, int j){
    int velikost = 0;
    for (int k = i; k <= j; k++){
        velikost += vrednosti[k] > 0 ? 1 : 0;
    }
    return velikost;
}


VelikostUnijeZGlavo::VelikostUnijeZGlavo(const vector<int> & seznam)
: PoizvedbeZGlavo(seznam, 0, 0)
{
    for (int i = n_vozlisc - 1; i >= 0; i--){
        posodobi_beta(i);
    }
}
VelikostUnijeZGlavo::~VelikostUnijeZGlavo() = default;

void VelikostUnijeZGlavo::posodobi_beta(int i){
    if (vozlisca[i].first > 0){
        vozlisca[i].second = n_listov_poddrevo(i);
    } else if (! je_notranje(i)){
        vozlisca[i].second = 0;
    } else{
        int l_beta = vozlisca[izracunaj_levega(i)].second;
        int d_beta = vozlisca[izracunaj_desnega(i)].second;
        vozlisca[i].second = l_beta + d_beta;
    }
}

int VelikostUnijeZGlavo::odgovori(int i, int j){
    auto [koncna_vrednost, aktivni_listi, i_skupni] = odgovor_vzdolz_obeh_poti(i, j);
    while (i_skupni >= 0){
        if (vozlisca[i_skupni].first > 0){
            return aktivni_listi;
        }
        i_skupni = izracunaj_starsa(i_skupni);
    }
    return koncna_vrednost;
}

void VelikostUnijeZGlavo::odgovor_vzdolz_ene_poti(int i, vector<int> & pot, vector<int> & vrednost_levo_desno, vector<int> & vozlisca_levo_desno){
    auto n = (int) pot.size();
    for (int j = 0; j < n; j++){
        auto [koren, posodobi_z_notranjim] = posodobitev_z_notranjim(i, pot, j, pot[j]);
        int internal_value = posodobi_z_notranjim ? vozlisca[koren].second : 0;
        if (koren >= 0){
            vozlisca_levo_desno[i] += n_listov_poddrevo(koren);
        }
        if (vozlisca[pot[j]].first > 0){
            vrednost_levo_desno[i] = vozlisca_levo_desno[i];
        } else{
            vrednost_levo_desno[i] += internal_value;
        }
    }
}