#include <string>
#include <vector>
#include <utility>
#include <tuple>

using std::string;
using std::pair;
using std::vector;
using std::tuple;



template <class T>
class PoizvedbeZGlavo {
    T nevtralna;
    T maxVrednost;
    T minVrednost;
    int n_vozlisc;
    int n_notranjih_vozlisc;
    vector<pair<T, pair<T, T>>> vozlisca;
    
    /* Vrne nabor (i_levo, i_desno, pot_levo, pot_desno, i_skupni, poti_pod_skupnim). V opisu spodaj se vsi indeksi vozlisc nanasajo
       na polozaj danega vozlisca v vektorju vozlisca.

      - i_levo/desno: indeksa vozlisc, ki pripadada robovoma intervala [i, j]
      - pot levo/desno: pot od i_levo (oz. i_desno) do korena (indeksi vozlisc) od lista do korena (prvi element je i_levo/i_desno, zadnji pa 0)
      - i_skupni: indeks prvega skupnega prednika vozlisc i_levo in i_desno (če sta poti [23, 11, 5, 2, 0] in [25, 12, 5, 2, 0], je i_skupni = 5)
      - poti_pod_skupnim: če sta poti kot zgoraj, je to par ([23, 11], [25, 12]).
    */
    tuple<int, int, vector<int>, vector<int>, int, pair<vector<int>, vector<int>>>  izracunaj_poti_itd(int i, int j) const;
    
    // Najde list, ki pripada i-temu elementu seznama, za katerega smo naredili drevo
    int najdi_list(int i) const;
    
    // Vrne true, če je vozlisce z indeksom (položajem) i (v vektoru vozlisca) notranje, in false sicer
    bool je_notranje(int i) const;

    // posodobi vrednost beta vozlisca s položajem i
    void posodobi_beta(int i);

    // izracuna dejansko vrednost iz beta vrednosti
    T izracunajVrednost(pair<T, T> beta);
    // zdruzi beti ...
    pair<T, T> zdruziBeti(pair<T, T> & beta1, pair<T, T> & beta2);
    void pristejAlfa(pair<T, T> & beta, T & alfa);

    /*
        Izračuna (delni) odgovor na poizvedbo vzdolž ene od poti (leve ali desne):

        - iPoti: 0 (leva pot) ali  1 (desna pot)
        - pot: seznam vozlišč strogo pod skupnim prednikom
        - vrednost_levo_desno: delna vrednost vzdolž leve in desne poti; komponenta iPoti se posodobi med klicem funkcije
        - vozlisca_levo_desno: število aktivnih listov vzdolž leve in desne poti; komponenta iPoti se posodobi med klicem funkcije
    */
    void odgovor_vzdolz_ene_poti(int iPoti, vector<int> & pot, vector<pair<T, T>> & vrednost_levo_desno);
    
    /*
        Izračuna odgovor vzdolž obeh poti (kjer še ne upoštevamo alfa vrednosti v najglobljem skupnem predniku in višje).
        Vrne trojico (koncna_vrednost, aktivni_listi, i_skupni), kjer je
        - koncna_vrednost: dejanski odgovor
        - i_skupni: položaj skupnega prednika v vektorju vozlisca
    */
    pair<pair<T, T>, int> odgovor_vzdolz_obeh_poti(int i, int j);
    
    /*
        Vrne par (notranji_koren, ali_naj_posodobim), kjer je 
        notranji_koren
        - eno od vozlišč, ki pokrijejo notranjost intervala listov, ki pripadajo intervalu [i, j]
          (desni sin vozlišča na levi poti, ki ni na levi poti, ali levi sin vozlišča na desni poti, ki ni na desni poti);
          v tem primeru je ali_naj_posodobim = true
        - eno od robnih vozlišč za interval [i, j] (bodisi i_levo bodisi i_desno); takrat je ali_naj_posodobim = false
        
    */
    static pair<int, bool> posodobitev_z_notranjim(int i, vector<int> & pot, int j, int node);
    
    // Izračuna pot od lista do korena, npr [23, 11, 5, 2, 0]
    static vector<int> pot_iz_lista(int i_list);
    
    /*
        Če je pot_levo = [23, 11, 5, 2, 0], pot_desno = [25, 12, 5, 2, 0], potem vrne
        (5, [23, 11], [25, 12]).
    */
    static pair<int, pair<vector<int>, vector<int>>> skupni_prednik_in_poti_pod_njim(vector<int> & pot_levo, vector<int> & pot_desno);
    
    /*
        Vrne vsa vozlišča x, za katera velja:
        x je desni sin vozlišča na levi poti, ki ni na levi poti, ali levi sin vozlišča na desni poti, ki ni na desni poti
    */
    static vector<int> notranji_koreni(vector<int> & pot_levo, vector<int> & pot_desno);
    
    // Izračuna velikost najmanjšega polnega drevesa, ki ima vsaj n listov
    static pair<int, int> izracunaj_velikost_polnega_drevesa(int n);
    
    // Izračuna položaj starša vozlišča na položaju i
    static int izracunaj_starsa(int i);
    
    // Izračuna položaj levega otroka vozlišča na položaju i
    static int izracunaj_levega(int i);
    
    // Izračuna položaj desnega otroka vozlišča na položaju i
    static int izracunaj_desnega(int i);

public:
    PoizvedbeZGlavo();
    ~PoizvedbeZGlavo();
    PoizvedbeZGlavo(const vector<T> & seznam, T alfa0, T nevtralnaVrednost, T maksi, T mini);

    // poveca interval [i, j] za x
    void povecaj(int i, int j, T x);
    // odgovor na poizvedbo za interval [i, j]
    int odgovori(int i, int j);
};


class Resitev {
    static const int MAX_OTROK = 1000 * 1000 + 2;

    long long emsoti[MAX_OTROK];
    PoizvedbeZGlavo<int> drevo;
    int n_otrok = 0;

    int najdiEmso(const long long & emso);
    int odgovori(const long long & emso0, const long long & emso1);
    void posodobi(const long long & emso0, const long long & emso1, const int & x);

public:
    // potrebujemo tak konstruktor
    Resitev(vector<long long> emsoOtrok);

    // in tako metodo obdelaj
    int obdelaj(const string & ukaz);
};