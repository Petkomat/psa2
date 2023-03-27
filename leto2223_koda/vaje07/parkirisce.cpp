#include "parkirisce.h"


using std::tuple;
using std::vector;


int ploscina(const vector<tuple<int, int, int, int>> & pravokotniki){

    VelikostUnijeZGlavo x (vector<int> {0, 0, 0});
    x.povecaj(1, 2, 200);
    return x.odgovori(0, 2);
}