/*
Avtor: Job Petrovcic
*/
#include "kdd.h"
#include <vector>

class semi {
public:
  void vstavi(const std::vector<double> &element);
  pair<std::vector<double>, double> najdi(const vector<double> &element) const;
  semi() = default;

private:
  int siz = 0;
  std::unique_ptr<KDDrevo> drevesa_[100];
};