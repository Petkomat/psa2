# Statična drevesa in intervalne poizvedbe

Ogledali si bomo drevo, ki ga sestavimo iz seznama $s = [a_0, a_1, \dots, a_{n - 1}]$ in je namenjeno hitremu računanju poizvedb

1. $p(i, j) = \sum_{k = i}^j a_i$ (vsoti podseznama) in
2. $p(i, j) = \sum_{k = i}^j \unicode{x1D7D9}[a_i > 0]$,

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