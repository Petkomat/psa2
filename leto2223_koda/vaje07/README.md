# Statična drevesa in intervalne poizvedbe

Ogledali si bomo drevo, ki ga sestavimo iz seznama $s = [a_0, a_1, \dots, a_{n - 1}]$ in je namenjeno hitremu računanju poizvedb tipa

1. vsota vrednosti $a_k$ za $i\leq k\leq j$ in
2. vsota vrednosti $\unicode{x1D7D9}[a_k > 0]$ za $i\leq k\leq j$,

pri čemer smo sposobni še hitrih posodobitev `dodaj(i, j, x)`, ki bi jih neučinkovito izvedli takole:

```
for k = i, i + 1, ..., j:
    s[k] = s[k] + x
```

V drugem primeru v resnici pokrivamo cela števila med $0$ in $n - 1$ z intervali $[i, j]$ ter med poizvedami računamo, koliko
števil med $i$ in $j$ je pokritih z vsaj enim intervalom, tj. velikost unije intervalov, presekane s celimi števili z intervala $[i, j]$.

Ključi v drevesu se nanašajo na indekse v seznamu, dodatno informacijo $(\alpha, \beta)$ v danem vozlišču $v$ pa razumemo tako:

- $\beta$ pove lokalni odgovor (omejen na poddrevo s korenom $v$),
- $\alpha$ na poti `starš(v) --> starš(starš(v)) --> ... --> koren` pa hrani globalni popravek, s katerim prilagodimo odgovore.


## Vsote podseznama

- $\alpha$: vsi listi v danem poddrevesu so bili povečani za toliko,
- $\beta$: trenutna vsota vrednosti v listih poddrevesa, ki upošteva le popravke, ki so bili omejeni na to poddrevo.

Pri odgovarjanju na poizvedbe moramo trenutne vsote posodabljati s pomočjo vrednosti $\alpha$ in številom relevantnih listov, ki smo jih obdelali v danem trenutku.


## Dolžina unije

Kar se posodobitev tiče, dovoljujemo

- dodajanje poljubnega intervala (tj. `dodaj(i, j, x)` za `x > 0`),
- odstranjevanje obstoječega intervala (tj. `dodaj(i, j, -x)` za `x > 0`, za katerega smo pred tem izvedli `dodaj(i, j, x)`).

TODO :)

## Hakeljci

Implementacijo si zelo olajšamo, če naredimo polno drevo, saj ima to nekaj prijetnih posledic:

- vozlišča lahko hranimo v seznamu
- vozlišče, ki je v seznamu vozlišč na mestu $i$, ima
    - otroka na mestih $2i + 1$ in $2i + 2$
    - starša na mestu $\lfloor \frac{i - 1}{2}\rfloor$
- vsi listi so na isti globini
- list, ki pripada indeksu $i$, najdemo v konstantnem času (ker vemo, koliko notranjih vozlišč je treba preskočiti)
- ...

Vseeno je vse skupaj kar tečno za implementacijo. Psevdokoda za odgovore na poizvedbe je približno taka:

1. Naredi polno drevo, ki ima vsaj $n$ listov. Prvih $n$ listov napolni z vrednostmi $a_i$, preostale pa z nevtralnimi vrednostmi (srednje lahko).
2. Najdi lista v drevesu, ki pripada indeksoma $i$ in $j$ (lahko), in njunega prvega skupnega prednika $p$. (lahko)
3. Najdi (levo) pot od lista za $i$ do $p$ in (desno) pot od lista za $j$ do $p$. (lahko)
4. Postopoma računaj ločena odgovora vzdolž leve in desne poti od spodaj navzgor. (zelo tečno)
5. Združi odgovora v skupnega in ga posodobi s pomočjo vrednosti $\alpha$ na poti od $p$ do korena. (tečno)

Pri 4. in 5. je treba zelo paziti, da česa ne posodobimo dvakrat (npr. če je $i = j$ ali sta $i$ in $j$ otroka od $p$ ali ...) itd. itd.


