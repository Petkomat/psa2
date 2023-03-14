/*
Avtor: Jure Smolar
*/
#include <vector>
#include <utility>
#include <memory>
#include <tuple>
#include <array>

using std::vector;
using std::pair;

class KDDrevo {

  bool jeList = true;
  int dimenzija;
  double mediana;
  vector<double> tocka;
  std::unique_ptr<KDDrevo> levo = nullptr ;
  // brez kazalca ne moremo vedeti, koliko spomina porabi KDDrevo
  std::unique_ptr<KDDrevo> desno = nullptr;

public:
  KDDrevo(const vector<vector<double>> & xs, const int dimenzija);
  ~KDDrevo();
  
  pair<vector<double>, double> najdi(const vector<double> & x) const;
  pair<vector<double>, double> najdiPomoc(const vector<double> & x, 
					  const std::unique_ptr<KDDrevo> & glavno,
					  const std::unique_ptr<KDDrevo> & drugo
					  ) const;
  
  void tocke_aux(vector<vector<double>> & acc) const;
  vector<vector<double>> tocke() const;

  static double izracunajMediano(const vector<vector<double>> & xs, const int dimension);
  static double izracunajEvklidsko2(const vector<double> & x0, const vector<double> & x1);

  static std::unique_ptr<KDDrevo> narediDrevo(const vector<vector<double>> & xs, const int dimenzija);

};

class KDDSemidinamicno {
private:
  vector<std::unique_ptr<KDDrevo>> nivoji;
  int dimenzija;
  vector<vector<double>> dodati;

public:

  KDDSemidinamicno();
  KDDSemidinamicno(const vector<vector<double>> & xs);
  ~KDDSemidinamicno();

  pair<vector<double>, double> najdi(const vector<double> & x) const;

  vector<vector<double>> tocke() const;

  void vstavi(const vector<double> & x);

};
