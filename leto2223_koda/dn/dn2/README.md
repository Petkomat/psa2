# Druga domača naloga

Tokratna naloga je zahtevala hitre odgovore na intervalne poizvedbe, ki so se nanašale na razlike vrednosti.

Pred poganjanjem `main.exe` odzipajte še preostale naloge.


## Glavna ideja

Denimo, da sta trenutno stanje bonbonov (zgornja vrstica) in posodobitev (spodnja vrstica) takšna:

```
0 0 0 0 0 0 0 0 0

    6 7 8 9
```

Razlika `stanje[i] - stanje[i - 1]` se je spremenila pri $i \in \\{ 2, 3, 4, 5, 6 \\}$ (če se $i$ začne pri $0$), in sicer za

- 6 (pri `i = 2`),
- 1 (pri `2 < i < 6`),
- -9 (pri `i = 7`).

Ker se tudi v splošnem v "notranjosti" intervala razlike povečajo za konstantno vrednost 1, lahko strukturo osnujemo tako:

- hranimo kar razlike stanj (namesto dejanskih števil bonbonov),
- poizvedbo `dodaj(i, j, x)` izvedemo s tremi klici, ki ustrezajo trem vrstam sprememb zgoraj.

Pazimo na robne primere (morda prva ali zadnja sprememba nista potrebni).

## Implementacija

Ta naloga zahteva dva premisleka glede na tisto s predavanj/vaj.

### Kako hraniti odgovore za podintervale (vrednosti beta)

Ideja, da bi vrednosti $\beta$ enostavno hranile največjo absolutno vrednost razlik na pripadajočem intervalu, ni čisto v redu,
saj v tem primeru ne moremo hitro posodobiti razlik. Primer:

Denimo, da trenutne razlike `-2, 1, 1` na danem intervalu povečamo za 1. Če bi hranili samo $\beta = 2$,
potem ne vemo, ali naj bo nova vrednost $\beta' = 2 + 1$ ali $\beta' = |-2 + 1|$.
Dodatna težava je, da bi bila v drugem primeru to napačna nova vrednost.

Zato raje za vsak interval v drevesu hranimo največjo in najmanjšo razliko:
v zgornjem primeru je to par $\beta = (-2, 1)$. To lahko hitro posodobimo
na $\beta =(-2 + \alpha, 1 + \alpha)$ in šele pri poizvedbah vrnemo večjo od absolutnih vrednosti komponent.

### Kako poskrbeti za ogromne EMŠO-vrednosti?

EMŠO-vrednosti ne moremo uporabiti za dejanske indekse listov, saj bi porabili preveč prostora.
Namesto tega jih zgolj shranimo v urejen seznam in nato z bisekcijo pred vsako poizvedbo ugotovimo,
kateri indeks pripada danemu EMŠU.

Morda je presenetljivo, a izkaže se, da je taka rešitev precej hitrejša od alternative, ko si ustvarimo
zgoščevalno tabelo `{emšo: indeks, ...}`.
