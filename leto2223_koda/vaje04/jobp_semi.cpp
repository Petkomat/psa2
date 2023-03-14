#include "jobp_semi.h"
#include <cassert>
#include <iostream>
#include <limits>
#include <memory>

/*
Avtor: Job Petrovcic
*/
using std::vector;
std::vector<KDDrevo> vse;
void semi::vstavi(const std::vector<double> &element) {
  int zadnji = 0;

  vector<vector<double>> nov;
  nov.push_back(element);
  while (zadnji < siz && drevesa_[zadnji]) {
    for (auto v : drevesa_[zadnji]->tocke())
      nov.push_back(v);
    drevesa_[zadnji] = nullptr;
    ++zadnji;
  }

  if (zadnji == siz) {
    ++siz;
  }
  auto novoDrevo = KDDrevo::narediDrevo(nov, 0);
  drevesa_[zadnji] = std::move(novoDrevo);
  // std::cout << "konec" << std::endl;
}

pair<std::vector<double>, double>
semi::najdi(const vector<double> &element) const {
  std::pair<std::vector<double>, double> ret = {
      {}, std::numeric_limits<double>::infinity()};

  bool prvi = 1;
  for (int i = 0; i < siz; ++i)
    if (drevesa_[i]) {
      auto que = drevesa_[i]->najdi(element);
      if (prvi || (que.second < ret.second)) {
        prvi = 0;
        ret = que;
      }
    }
  return ret;
}

// std::vector<KDDrevo> drevesa_;
// std::vector<std::vector<double>> elementi_;