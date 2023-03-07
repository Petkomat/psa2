#include "kdd.h"
#include <algorithm>
#include <memory>

// #include <vector>
// #include <utility>


using std::vector;
using std::pair;

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
    
    vector<double> optX;
    double optR;
    return pair<vector<double>, double>(optX, optR);
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
    vector<vector<double>> xs;
    return xs;
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
