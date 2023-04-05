"""
Čas samo narašča.

Paziti je treba, da

1. se ne bodo seznami kar sami urejali:
   OK, če je mix stvari

2. naivna drevesa ne bodo uravnotežena:
   OK, če je trend jasen: od korena stran (0, 1, -1, 2, -2, ...)

3. se včasih časi podvojijo (ali potrojijo)


NALOGA A: poizvedbe se nanašajo na končno stanje strukture in so na koncu
NALOGA B: mešano
"""


import random
import os


def naloga_a_1(n_vstavljanj, n_poizvedb):
    """
    Vse narašča.
    """
    random.seed(12345)
    casi_vstavljanj = list(range(100, n_vstavljanj + 100))
    vstavljene_vrednosti = list(map(lambda i: 2 * i + 10, range(n_vstavljanj)))

    max_cas = casi_vstavljanj[-1]
    min_vrednost = vstavljene_vrednosti[0]
    max_vrednost = vstavljene_vrednosti[-1]

    casi_poizvedb = list(map(lambda i: max_cas + i % 2, range(n_vstavljanj - 4))) + [max_cas + 10 for _ in range(4)]
    poizvedovane_vrednosti = [random.randint(min_vrednost, max_vrednost) for _ in range(n_poizvedb - 4)]
    poizvedovane_vrednosti.extend([min_vrednost - 5, min_vrednost, max_vrednost, max_vrednost + 5])
    dogodki = []
    for t, v in zip(casi_vstavljanj, vstavljene_vrednosti):
        dogodki.append(("v", t, v))
    for t, v in zip(casi_poizvedb, poizvedovane_vrednosti):
        dogodki.append(("p", t, v))
    return dogodki, "nalogaA1"


def naloga_a_2(n_vstavljanj, n_poizvedb):
    """
    Vrednosti stevil padajo.
    """
    dogodki_a1, _ = naloga_a_1(n_vstavljanj, n_poizvedb)
    # n = 3 --> 0 1 2  ==> i < 1
    # n = 4 --> 0 1 | 2 3 ==> i < 2
    # ==> i < n // 2
    for i in range(n_vstavljanj // 2):
        _, t_levo, v_levo = dogodki_a1[i]
        _, t_desno, v_desno = dogodki_a1[n_vstavljanj - 1 - i]
        dogodki_a1[i] = ("v", t_levo, v_desno)
        dogodki_a1[n_vstavljanj - 1 - i] = ("v", t_desno, v_levo)
    return dogodki_a1, "nalogaA2"


def naloga_a_3(n_vstavljanj, n_poizvedb):
    """
    Mesano.
    """
    random.seed(3)
    dogodki = []
    for _ in range(n_vstavljanj):
        dogodki.append(("v", random.randint(1, 100), random.randint(100, 10 ** 6 - 100)))
    for _ in range(n_poizvedb):
        dogodki.append(("p", random.randint(100, 200), random.randint(1, 10 ** 6)))
    dogodki.sort(key=lambda triplet: triplet[1])  # (kind, t, q)
    return dogodki, "nalogaA3"


def naloga_b_1(n_vstavljanj, n_poizvedb):
    """
    Cik-cakamo z obojim
    :return:
    """
    random.seed(123456)
    dogodki = []
    t_last = 1
    for _ in range(10):
        t_last = random.randint(t_last, 100)
        dogodki.append(("v", t_last, random.randint(100, 10 ** 6 - 100)))
    n_left = [n_vstavljanj - 10, n_poizvedb]
    center = 5 * 10 ** 5
    amplituda = 4 * 10 ** 5
    for j in range(n_vstavljanj + n_poizvedb - 10):
        bias_counts = [n_left[0] * 4, n_left[1]]
        p_vstavi = bias_counts[0] / sum(bias_counts)
        if random.random() < p_vstavi:
            vrsta = "v"
            i = 0
            t = t_last + j % 3
            t_last = t
        else:
            vrsta = "p"
            i = 1
            t = random.randint(100, round(1.2 * t_last))
        predznak = 2 * (j % 2) - 1
        odmik = predznak * round(amplituda * j / (n_vstavljanj + n_poizvedb))
        dogodki.append((vrsta, t, center + odmik))
        n_left[i] -= 1
        if min(n_left) < 0:
            raise ValueError(":)")
    return dogodki, "nalogaB1"


def naloga_b_2(n_vstavljanj, n_poizvedb):
    """
    Mesano, a 10 vstavljanj najprej.
    """
    random.seed(12345)
    dogodki = []
    t_last = 1
    for _ in range(10):
        t_last = random.randint(t_last, 100)
        dogodki.append(("v", t_last, random.randint(100, 10 ** 6 - 100)))
    n_left = [n_vstavljanj - 10, n_poizvedb]
    while max(n_left) > 0:
        bias_counts = [n_left[0] * 4, n_left[1]]
        p_vstavi = bias_counts[0] / sum(bias_counts)
        if random.random() < p_vstavi:
            vrsta = "v"
            i = 0
            t = t_last + sum(n_left) % 3
            t_last = t
        else:
            vrsta = "p"
            i = 1
            t = random.randint(100, round(1.2 * t_last))
        dogodki.append((vrsta, t, random.randint(1, 10 ** 6)))
        n_left[i] -= 1
        if min(n_left) < 0:
            raise ValueError(":)")
    return dogodki, "nalogaB2"


def preveri(ime, dogodki):
    """
    1. Casi vstavljanj ne smejo padati
    2. Vse vrednosti med 1 in 10 ** 6
    3. Pred prvo poizvedbo vsaj 10 vstavljanj
    4. Casi in vrednosti naravna stevila
    :param dogodki:
    :return:
    """
    t_last_insert = 0
    n_insertions = 0
    min_allowed_t = 0
    for tip, cas, vrednost in dogodki:
        if tip == "v":
            assert t_last_insert <= cas, (tip, cas, vrednost, ime)
            t_last_insert = cas
            n_insertions += 1
            if n_insertions == 10:
                min_allowed_t = t_last_insert
        elif tip == "p":
            assert cas >= min_allowed_t
            if ime.startswith("nalogaA"):
                assert cas >= t_last_insert
        else:
            raise ValueError(f"tip: {tip}")
        assert isinstance(vrednost, int)
        assert 1 <= vrednost <= 10 ** 6
        assert cas >= 0
    return True


def shrani(ime, dogodki):
    if not preveri(ime, dogodki):
        raise ValueError(f"Nekaj narobe z {ime}")
    with open(ime, "w") as f:
        for tip, cas, vrednost in dogodki:
            print(f"{tip} {cas} {vrednost}", file=f)


if __name__ == '__main__':
    n1 = 10 ** 5
    n2 = 10 ** 4
    generatorji = [naloga_a_1, naloga_a_2, naloga_a_3, naloga_b_1, naloga_b_2]
    for generator in generatorji:
        podatki, izhodna = generator(n1, n2)
        shrani(f"{izhodna}.in", podatki)
        resitve = f"{izhodna}.out"
        if os.path.exists(resitve):
            os.remove(resitve)




