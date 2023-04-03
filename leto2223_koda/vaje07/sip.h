#pragma once

#include <utility>
#include <vector>
#include <string>
#include <tuple>

using std::pair;
using std::vector;
using std::tuple;

/*

Hierarhija razredov:

StaticneIntervalnePoizvedbe:
   - PoizvedbeSSilo:
       - VsoteSSilo
       - VelikostUnijeSSilo
   - PoizvedbeZGlavo:
       - VsoteZGlavo
       - VelikostUnijeZGlavo

Vsak razred mora implementirati povecaj(i, j, x) in odgovori(i, j).

*/



class StaticneIntervalnePoizvedbe{

public:
    StaticneIntervalnePoizvedbe();
    virtual ~StaticneIntervalnePoizvedbe();
    // poveca interval [i, j] za x
    virtual void povecaj(int i, int j, int x) = 0;
    // odgovor na poizvedbo za interval [i, j]
    virtual int odgovori(int i, int j) = 0;
};


class PoizvedbeSSilo : public StaticneIntervalnePoizvedbe {

protected:
    vector<int> vrednosti;

public:
    PoizvedbeSSilo();
    explicit PoizvedbeSSilo(const vector<int> & seznam);

    void povecaj(int i, int j, int x) final;

};



class PoizvedbeZGlavo : public StaticneIntervalnePoizvedbe {



protected:
    // stevilo vseh vozlisc drevesa
    int n_vozlisc;
    // stevilo notranjih vozlisc drevesa
    int n_notranjih_vozlisc;
    // vektor vozlisc, kjer hranimo par (alfa, beta)
    vector<pair<int, int>> vozlisca;
    // listi_poddrevesa[i]: stevilo listov v poddrevesu s korenom v vozliscu z indeksom i
    // (npr. ker je koren drevesa na i = 0, je list_poddrevesa[0] stevilo vseh listov)
    vector<int> listi_poddrevesa;

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
    // izracuna stevilo listov v poddrevesu in shrani rezultat v listi_poddrevesa (od koder potem v prihodnjih klicih bere)
    int n_listov_poddrevo(int i);
    // Vrne true, če je vozlisce z indeksom (položajem) i (v vektoru vozlisca) notranje, in false sicer
    bool je_notranje(int i) const;

    // posodobi vrednost beta vozlisca s položajem i
    virtual void posodobi_beta(int i) = 0;
    /*
        Izračuna (delni) odgovor na poizvedbo vzdolž ene od poti (leve ali desne):

        - iPoti: 0 (leva pot) ali  1 (desna pot)
        - pot: seznam vozlišč strogo pod skupnim prednikom
        - vrednost_levo_desno: delna vrednost vzdolž leve in desne poti; komponenta iPoti se posodobi med klicem funkcije
        - vozlisca_levo_desno: število aktivnih listov vzdolž leve in desne poti; komponenta iPoti se posodobi med klicem funkcije
    */
    virtual void odgovor_vzdolz_ene_poti(int iPoti, vector<int> & pot, vector<int> & vrednost_levo_desno, vector<int> & vozlisca_levo_desno) = 0;
    /*
        Izračuna odgovor vzdolž obeh poti (kjer še ne upoštevamo alfa vrednosti v najglobljem skupnem predniku in višje).
        Vrne trojico (koncna_vrednost, aktivni_listi, i_skupni), kjer je
        - koncna_vrednost: dejanski odgovor
        - aktivni listi: število aktivnih listov (j - i + 1)
        - i_skupni: položaj skupnega prednika v vektorju vozlisca
    */
    tuple<int, int, int> odgovor_vzdolz_obeh_poti(int i, int j);
    
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
    ~PoizvedbeZGlavo() override;
    PoizvedbeZGlavo(const vector<int> & seznam, int alfa0, int nevtralnaVrednost);

    void povecaj(int i, int j, int x) final;
};


class VsotaSSilo : public PoizvedbeSSilo {


public:
    explicit VsotaSSilo(const vector<int> & seznam);
    ~VsotaSSilo() final;

    int odgovori(int i, int j) final;

};


class VsotaZGlavo : public PoizvedbeZGlavo {

protected:
    void posodobi_beta(int i) final;
    void odgovor_vzdolz_ene_poti(int iPoti, vector<int> & pot, vector<int> & vrednost_levo_desno, vector<int> & vozlisca_levo_desno) final;

public:
    explicit VsotaZGlavo(const vector<int> & seznam);
    ~VsotaZGlavo() final;

    int odgovori(int i, int j) final;
    
};


class VelikostUnijeSSilo : public PoizvedbeSSilo {

public:
    explicit VelikostUnijeSSilo(const vector<int> & seznam);
    ~VelikostUnijeSSilo() final;

    int odgovori(int i, int j) final;

};


class VelikostUnijeZGlavo : public PoizvedbeZGlavo {

protected:
    void posodobi_beta(int i) final;
    void odgovor_vzdolz_ene_poti(int iPoti, vector<int> & pot, vector<int> & vrednost_levo_desno, vector<int> & vozlisca_levo_desno) final;

public:
    explicit VelikostUnijeZGlavo(const vector<int> & seznam);
    ~VelikostUnijeZGlavo() final;


    int odgovori(int i, int j) final;

};