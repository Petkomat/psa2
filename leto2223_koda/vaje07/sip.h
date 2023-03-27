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
    virtual void povecaj(int i, int j, int x) = 0;
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
    int n_vozlisc;
    int n_notranjih_vozlisc;
    vector<pair<int, int>> vozlisca;
    vector<int> listi_poddrevesa;

    tuple<int, int, vector<int>, vector<int>, int, pair<vector<int>, vector<int>>>  izracunaj_poti_itd(int i, int j) const;
    int najdi_list(int i) const;
    int n_listov_poddrevo(int i);
    bool je_notranje(int i) const;


    virtual void posodobi_beta(int i) = 0;
    virtual void odgovor_vzdolz_ene_poti(int iPoti, vector<int> & pot, vector<int> & vrednost_levo_desno, vector<int> & vozlisca_levo_desno) = 0;
    tuple<int, int, int> odgovor_vzdolz_obeh_poti(int i, int j);
    
    static pair<int, bool> posodobitev_z_notranjim(int i, vector<int> & pot, int j, int node);
    static vector<int> pot_iz_lista(int i_leaf);
    static pair<int, pair<vector<int>, vector<int>>> skupni_prednik_in_poti_pod_njim(vector<int> & pot_levo, vector<int> & pot_desno);
    static vector<int> notranji_koreni(vector<int> & pot_levo, vector<int> & pot_desno);
    static pair<int, int> izracunaj_velikost_polnega_drevesa(int n);
    static int izracunaj_starsa(int i);
    static int izracunaj_levega(int i);
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