#include "kdd.h"
#include <algorithm>
#include <memory>
#include <limits>


// Ni treba (spet) vključiti vector-jev in utility-jev


using std::vector;
using std::pair;


/*********************************************************************************************
 * KD Drevo
**********************************************************************************************/


KDDrevo::KDDrevo() = default;


KDDrevo::KDDrevo(const vector<vector<double>> & xs, const int dimenzija)
    : dimenzija(dimenzija), tocka(xs[0])
{
    
}



std::unique_ptr<KDDrevo> KDDrevo::narediDrevo(const vector<vector<double>> & xs, const int dim){
    auto koren = std::make_unique<KDDrevo>(xs, dim);
    if (xs.size() > 1){
        koren -> jeList = false;
        double med = izracunajMediano(xs, dim);
        koren -> mediana = med;
        koren -> tocka = vector<double>(0); // samo v listih hranimo
        auto d = (int) xs[0].size();
        vector<vector<double>> leveTocke;
        vector<vector<double>> desneTocke;
        for (auto const & x : xs){
            if (x[dim] <= med){
                leveTocke.push_back(x);
            } else{
                desneTocke.push_back(x);
            }
        }
        koren -> levo = KDDrevo::narediDrevo(leveTocke, (dim + 1) % d);
        koren -> desno = KDDrevo::narediDrevo(desneTocke, (dim + 1) % d);
    }
    return koren;
}


KDDrevo::~KDDrevo() = default;

pair<vector<double>, double> KDDrevo::najdi(const vector<double> & x) const {
    if (jeList){
        return pair<vector<double>, double>(tocka, izracunajEvklidsko2(tocka, x));
    }
    else if (x[dimenzija] <= mediana)
    {
        return najdiPomoc(x, levo, desno);
    } else {
        return najdiPomoc(x, desno, levo);
    }
}


pair<vector<double>, double> KDDrevo::najdiPomoc(
    const vector<double> & x, 
    const std::unique_ptr<KDDrevo> & glavno,
    const std::unique_ptr<KDDrevo> & drugo
) const {
    auto [optX, optR] = glavno -> najdi(x);
    if (double doMediane = mediana - x[dimenzija]; doMediane * doMediane < optR){
        auto [cezmejniX, cezmejnaR] = drugo -> najdi(x);
        if (cezmejnaR < optR){
            optX = cezmejniX;
            optR = cezmejnaR;
        }
    }
    return pair<vector<double>, double>(optX, optR);
}


vector<vector<double>> KDDrevo::tocke() const {
    vector<vector<double>> tocke;
    if (jeList){
        tocke.push_back(tocka);
    } else {
        for (auto const & t : levo -> tocke()){
            tocke.push_back(t);
        }
        for (auto const & t : desno -> tocke()){
            tocke.push_back(t);
        }
    }
    return tocke;
}
        
// brez static
double KDDrevo::izracunajMediano(const vector<vector<double>> & xs, const int dimension){
    auto n = (int) xs.size();
    vector<double> xDim(n);
    for (int i = 0; i < n; i++){
        xDim[i] = xs[i][dimension];
    }
    // se več o funkciji sort: https://cplusplus.com/reference/algorithm/sort/
    std::sort(xDim.begin(), xDim.end());
    int iMediana = (n - 1)  / 2;  // [0 1 2 3] --> 1, [0 1 2] --> 1
    return xDim[iMediana];
}

// brez static
double KDDrevo::izracunajEvklidsko2(const vector<double> & x0, const vector<double> & x1){
    double e = 0.0;
    double razlika;
    for (int i = 0; i < x0.size(); i++){
        razlika = x0[i] - x1[i];
        e += razlika * razlika;
    }
    return e;
}



/*********************************************************************************************
 * KD Drevo: semidinamično 
**********************************************************************************************/

KDDSemidinamicno::KDDSemidinamicno() = default;


void KDDSemidinamicno::vstavi(const vector<double> & x) {
    auto mesto = (int) drevesa.size(); // v najslabsem primeru smo tukaj
    vector<vector<double>> elementi; elementi.push_back(x);
    // naberi vse
    for (int i = 0; i < drevesa.size(); i++){
        if (drevesa[i] == nullptr){
            mesto = i;
            break;
        }
        // tu se dogaja veliko kopiranja
        auto novi_elementi = drevesa[i] ->  tocke();
        elementi.reserve(elementi.size() + novi_elementi.size());
        elementi.insert(elementi.end(), novi_elementi.begin(), novi_elementi.end());
    }
    // pocisti prejsnja
    for (int i = 0; i < mesto; i++){
        drevesa[i] = nullptr;
    }
    // dodaj novo
    auto novoDrevo = KDDrevo::narediDrevo(elementi, 0);
    if (mesto == drevesa.size()){
        drevesa.push_back(std::move(novoDrevo));
    } else{
        drevesa[mesto] = std::move(novoDrevo);
    }
}

pair<vector<double>, double> KDDSemidinamicno::najdi(const vector<double> & x) const {
    vector<double> optX;
    double optR = std::numeric_limits<double>::infinity();
    for (auto const & drevo : drevesa){
        if (drevo == nullptr){
            continue;
        }
        auto [optXTam, optRTam] = drevo -> najdi(x);
        if (optRTam < optR){
            optX = optXTam;
            optR = optRTam;
        }
    }
    return pair<vector<double>, double>(optX, optR);
}
