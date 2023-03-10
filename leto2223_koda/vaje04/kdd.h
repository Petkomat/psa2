#include <vector>
#include <utility>
#include <memory>


using std::vector;
using std::pair;


class KDDrevo {

    bool jeList = true;
    int dimenzija;
    double mediana;
    vector<double> tocka;
    std::unique_ptr<KDDrevo> levo; // brez kazalca ne moremo vedeti, koliko spomina porabi KDDrevo
    std::unique_ptr<KDDrevo> desno;

public:
    bool jePrazno = false;

    KDDrevo();
    KDDrevo(const vector<vector<double>> & xs, const int dimenzija);
    ~KDDrevo();

    pair<vector<double>, double> najdi(const vector<double> & x) const;
    pair<vector<double>, double> najdiPomoc(
        const vector<double> & x, 
        const std::unique_ptr<KDDrevo> & glavno,
        const std::unique_ptr<KDDrevo> & drugo
    ) const;

    vector<vector<double>> tocke() const;
        
    static double izracunajMediano(const vector<vector<double>> & xs, const int dimension);
    static double izracunajEvklidsko2(const vector<double> & x0, const vector<double> & x1);

    static std::unique_ptr<KDDrevo> narediDrevo(const vector<vector<double>> & xs, const int dimenzija);

};


class KDDSemidinamicno {
    vector<std::unique_ptr<KDDrevo>> drevesa;

public:
    KDDSemidinamicno();

    void vstavi(const vector<double> & x);
    pair<vector<double>, double> najdi(const vector<double> & x) const;
};