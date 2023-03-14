#include "jures_kdd.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <cstdarg>

// #include <vector>
// #include <utility>


using std::vector;
using std::pair;

KDDrevo::KDDrevo(const vector<vector<double>> & xs, const int dimenzija)
    : dimenzija(dimenzija), tocka(xs[0])
{
  auto n = (int) xs.size();
  if (n > 1){
    jeList = false;
    auto d = (int) xs[0].size();
        
    mediana = izracunajMediano(xs, dimenzija); 
    
    tocka = vector<double>(0); // samo v listih hranimo

    vector<vector<double>> leveTocke((n >> 1) + n % 2);
    vector<vector<double>> desneTocke(n >> 1);

    int j = 0;
    for(int i = 0; i < n; i++) {
      if (xs[i][dimenzija] <= mediana)
	leveTocke[j++] = xs[i];
      else
	desneTocke[i - j] = xs[i];
    }
        
    levo = KDDrevo::narediDrevo(leveTocke, (dimenzija + 1) % d);
    desno = KDDrevo::narediDrevo(desneTocke, (dimenzija + 1) % d);
    }
}

std::unique_ptr<KDDrevo> KDDrevo::narediDrevo(const vector<vector<double>> & xs, const int dim){
  return std::make_unique<KDDrevo>(xs, dim);
}

KDDrevo::~KDDrevo() = default;

pair<vector<double>, double> KDDrevo::najdi(const vector<double> & x) const {
  if (jeList)
    return pair<vector<double>, double>(tocka, izracunajEvklidsko2(tocka, x));
  else if (x[dimenzija] <= mediana)
    return najdiPomoc(x, levo, desno);
  else
    return najdiPomoc(x, desno, levo);
  
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

/*
Avtor: Jure Smolar
*/
vector<vector<double>> KDDrevo::tocke() const {
  vector<vector<double>> acc;
  tocke_aux(acc);
  return acc;
}

void KDDrevo::tocke_aux(vector<vector<double>> & acc) const {
  if (jeList)
    acc.push_back(tocka);
  else {
    if (levo)
      levo->tocke_aux(acc);
    if (desno)
      desno->tocke_aux(acc);
  }
}

// brez static
double KDDrevo::izracunajMediano(const vector<vector<double>> & xs, const int dimension) {
  auto n = (int) xs.size();
  vector<double> xDim(n);
  for (int i = 0; i < n; i++)
    xDim[i] = xs[i][dimension];

  int iMediana = (n - 1) >> 1;  // [0 1 2 3] --> 1, [0 1 2] --> 1
  std::nth_element(xDim.begin(), xDim.begin() + iMediana, xDim.end());
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

/*******************************************************************************

Semidinamično

********************************************************************************/

vector<vector<double>> KDDSemidinamicno::tocke() const {
  // Pogledamo točke vseh inhabited dreves.
  vector<vector<double>> xs;
  for(auto && nivo: nivoji)
    if (nivo)
      nivo->tocke_aux(xs);
  return xs;
}

void KDDSemidinamicno::vstavi(const vector<double> & x) {
  // Vstavimo na prvo prazno drevo, ali v novega.
  // Po poti pobiramo elemente manjših dreves in jih praznimo.
  vector<vector<double>> xs {x};
  for (auto && nivo: nivoji) {
    if (nivo) {
      nivo->tocke_aux(xs);
      nivo = nullptr; // Avtomatično uniči, ker imamo unique_ptr.
    } else {
      nivo = KDDrevo::narediDrevo(xs, 0);
      return;
    }
  } // else:
  nivoji.push_back(KDDrevo::narediDrevo(xs, 0));
}

pair<vector<double>, double> KDDSemidinamicno::najdi(const vector<double> & x) const {
  // Iščemo po vseh inhabited poddrevesih.
  pair<vector<double>, double> kandidat;
  for (auto && nivo: nivoji) {
    if (nivo) {
      auto const & nov_kandidat = nivo->najdi(x);
      if (nov_kandidat.second < kandidat.second)
	kandidat = nov_kandidat;
    }
  }
  return kandidat;
}

KDDSemidinamicno::KDDSemidinamicno() = default;

KDDSemidinamicno::KDDSemidinamicno(const vector<vector<double>> & xs) {
  int n = size(xs);
  int procesirani = 0;
  int k = 0;

  // Ko prvič gradimo drevo, razdelimo xs na podsezname dolžine potenc dvojke z binarnim štetjem.
  // S tem zagotovimo, da imamo željeno poldinamično strukturo ter vsa drevesa na pravem mestu.

  while (n != 0) {
    if (n % 2) {
      const int nov = procesirani + (1 << k);
      const vector<vector<double>> sub(&xs[procesirani], &xs[nov]);
      nivoji.push_back(KDDrevo::narediDrevo(sub, 0));
      procesirani = nov;
    }
    else
      nivoji.push_back(nullptr);
    n >>= 1;
    k++;
  }
}

KDDSemidinamicno::~KDDSemidinamicno() = default;


