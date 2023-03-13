# Naloga

Pogledali smo is čas iskanja enega elementa v 2D in v 10D medtem ko velikost strukture narašča od 1 proti 10 000.
Uporabili smo poldinamično razširitev KD-drevesa.

Preverjamo, ali trditev _časovna zahtevnost iskanja v KD-drevesu je v praksi O(log n)_ drži. Imeli smo tri primere podatkov:

- primer1: točke so enakomerno porazdeljene na kocki $[0, 1]^D$ (tako tiste za vstavljanje kot tiste za iskanje)
- primer2: točke za iskanje kot v primeru 1, točke za vstavljanje pa ležijo na diagonali tega prostora (vse komponente imajo enake)
- primer3: točke za vstavljanje so kot v primeru 1, točke za iskanje pa so elementi $[100, 101]^D$


Namesto časa enega iskanja narišemo povprečno vrednost zadnjih petih (zgladimo vse skupaj).


# Rezultati v 2D

Najprej prikažemo posamične čase.

![posamični časi 2D](casi_po_en2D.PNG "posamični časi 2D")

Težko je oceniti, ali je to res logaritemska rast, je pa jasno razvidno,
da je prvi primer najlažji, tretji pa najtežji, ker so si tam razdalje zelo podobne.

Ker so točke stran od diagonale (npr. kakšen od vogalov kvadrata) 
približno enako oddaljene od številnih točk na diagonali, je drugi težji kot prvi.

Za dobro mero še kumulativni grafi:

![kumulativni časi 2D](casi_kumu2D.PNG "kumulativni časi 2D")

# Rezultati v 10D

Prostora v 10D je precej več: "volumen" 2D-kocke s stranico 2 je 4, volumen 10D kocke pa 1024.
Zato zelo hitro točke nimajo bližnjih sosedov oz. so vsi sosedi še kar daleč stran.

![posamični časi 10D](casi_po_en10D.PNG "posamični časi 10D")

Vidimo, da so vsi primeri približno enako težki, kar potrdijo tudi skupni časi:

![kumulativni časi 10D](casi_kumu10D.PNG "kumulativni časi 10D")


## Primerjava 2D in 10D

Če bi drevesa enako dobro delala v 2 in 10 dimenzijah, bi vse skupaj v 10D
trajalo 5-krat dlje kot v 2D (ali pa še to ne, saj se spremeni le cena izračuna
razdalje).

Vidimo, da je poslabšanje v primeru 1 (dobra sekunda : več kot 40 sekund) precej hujše.
Sklepamo tudi, da izračun razdalj ni glavni potrošnik, sicer bi se tudi preostala dva
primera konkretno poslabšala, a se najtežji (tretji) skoraj ne.

## Opombe o implementaciji


1. Razred `Drevo` ne more imeti polj `Drevo levo` in `Drevo desno`.
   - Če za `Drevo` potrebujemo `x` spomina, potem bi morali ob pretvorbi `cpp --> exe` rešiti enačbo
     `x = 2x + ostalo` ...
   - Zato smo uporabili kazalca `Drevo * levo` in `Drevo * desno`: 
     vsi kazalci imajo enako velikost, saj je njihova vrednost število (mesto v spominu)
2. Semidinamična struktura je implementirana res naivno in povsem generično:
   - Drevo dejansko hrani točke, zato jih moramo ob gradnji novega izvleči iz njega in jih skopirati ... 
   - Namesto tega bi lahko točke hranili direktno v poldinamičnem razredu in le premeščali
     kazalce nanje
   - ...  
 
