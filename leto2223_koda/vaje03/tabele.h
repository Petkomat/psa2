#include <vector>

class Tabela {


public:
    // vse metode so virtualne (virtual), kar omogoča tole, da se v funkiji
    //
    // void stopaj(Tabela * t) { t.dodaj(3); }
    //
    // ki sprejme nekaj, kar je Tabela (npr. VgrajenaTabela ali NasaTabela),
    // potem pokliče prava implementacija metode dodaj (ne ta, ki je definirana v Tabela)!
    //
    // Vse metode so tudi abstraktne (= 0), zato da jih MORAMO povoziti v podrazredih
    // z drugimi besedami: metode so "pure virtual"

    // https://www.geeksforgeeks.org/pure-virtual-functions-and-abstract-classes/

    virtual ~Tabela() = 0;

    virtual void dodaj(const int& x) = 0;
    virtual int dostopaj(const int& i) const = 0;
    virtual int trenutnaVelikost() const = 0;
};


class VgrajenaTabela : public Tabela {

    std::vector<int> tabela;

public:
    // metode tukaj lahko označimo z
    // - override (funkcija je virtualna in povozimo implementacije iz nadrazeda Tabela)
    // - final (funkcija je virtualna in je ne moremo več povoziti v podrazredu)

    ~VgrajenaTabela() final;

    void dodaj(const int& x) final;
    int dostopaj(const int& i) const final;
    int trenutnaVelikost() const final;    
};


class NasaTabela : public Tabela {

protected:
    int velikost = 10;
    int naslednji = 0;
    int * tabela;

public:
    NasaTabela();
    ~NasaTabela() override;

    void dodaj(const int& x) override;
    int dostopaj(const int& i) const final;
    int trenutnaVelikost() const final;

     // te ne bomo implementirali v tem razredu, ampak v podrazredih
    virtual int izracunajNovoDolzino() = 0;
};


class KonstantenPrirast : public NasaTabela {
    const int K = 1e5;

public:
    ~KonstantenPrirast() override;
    int izracunajNovoDolzino() final;
};

class KonstantenFaktorPrirasta : public NasaTabela {
    const double ALFA = 1.2;

public:
    ~KonstantenFaktorPrirasta() override;
    int izracunajNovoDolzino() final;
};