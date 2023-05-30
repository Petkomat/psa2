"""
Paziti je treba, da

1. so intervali dolgi (tako poizvedb kot posodobitev)
2. ne bo out of range (treba long long):
    - a1/b1: kratki intervali posodabljanja (tako da ne pride do prekoračitve), dolge posodobitve
    - a2/b2: go big or go home

NALOGA A: poizvedbe se nanašajo na končno stanje strukture in so na koncu
NALOGA B: mešano

Naštimaj tako, da bo min včasih na meji: poskrbimo na koncu ...
"""


import random
import os
from datetime import date, timedelta


def generate_ids(n):
    random.seed(1234)
    ids = []
    this_date = date(2019, 1, 1)
    three_digits = 0
    while len(ids) < n:
        gender = 500 if random.random() < 0.5 else 505
        this_id = f"{this_date.year}{this_date.month:02}{this_date.day:02}{gender}{three_digits:03}"
        ids.append(this_id)
        three_digits = (three_digits + 1) % 1000
        if three_digits == 0:
            this_date += timedelta(days=1)
    ids.sort()
    return ids


def naloga_a_1(n_children, n_vstavljanj, n_poizvedb):
    return naloga_a(n_children, n_vstavljanj, n_poizvedb, 10, 10**4, "A1")


def naloga_a_2(n_children, n_vstavljanj, n_poizvedb):
    return naloga_a(n_children, n_vstavljanj,  n_poizvedb, 5 * 10**5, 5 * 10**5, "A2")


def naloga_a(n_children, n_updates, n_queries, max_interval_update, max_interval_query, problem_id):
    random.seed(1234)
    ids = generate_ids(n_children)
    # 3 special updates: value[3] == value[4] after that
    events = [
        ("p", ids[0], ids[3], 1),
        ("p", ids[4], ids[7], 6),
        ("p", ids[4], ids[7], 6)
    ]
    for _ in range(n_updates - 3):
        i0 = 10 + int(random.random() * n_children)
        i1 = min(n_children - 1, i0 + random.randint(0, max_interval_update))
        x = random.randint(1, 10)
        events.append(("p", ids[i0], ids[i1], x))
    for _ in range(n_queries - 3):
        i0 = int(random.random() * (n_children - 1))
        i1 = min(n_children - 1, i0 + random.randint(1, max_interval_query))
        events.append(("o", ids[i0], ids[i1]))
    # 3 special
    events.append(("o", ids[0], ids[3]))
    events.append(("o", ids[4], ids[7]))
    events.append(("o", ids[0], ids[7]))
    return ids, events, f"naloga{problem_id}"


def naloga_b_1(n_children, n_vstavljanj, n_poizvedb):
    return naloga_b(n_children, n_vstavljanj, n_poizvedb, 10, 10**4, "B1")


def naloga_b_2(n_children, n_vstavljanj, n_poizvedb):
    return naloga_b(n_children, n_vstavljanj,  n_poizvedb, 5 * 10**5, 5 * 10**5, "B2")


def naloga_b(n_children, n_updates, n_queries, max_interval_update, max_interval_query, problem_id):
    ids, events, problem_name = naloga_a(
        n_children, n_updates, n_queries, max_interval_update, max_interval_query, problem_id
    )
    random.seed(1234)
    random.shuffle(events)
    return ids, events, problem_name


def save_problem(ids, events, problem):
    with open(f"{problem}.emso", "w") as f:
        # print(len(ids), file=f)
        for emso in ids:
            print(emso, file=f)
    with open(f"{problem}.in", "w") as f:
        for event in events:
            str_dogodek = " ".join(map(str, event))
            print(str_dogodek, file=f)


def generate_all():
    n = 10 ** 6
    n1 = 10 ** 4
    n2 = 10 ** 4
    generatorji = [naloga_a_1, naloga_a_2, naloga_b_1, naloga_b_2]
    for generator in generatorji:
        ids, events, problem_id = generator(n, n1, n2)
        save_problem(ids, events, problem_id)
        solutions = f"{problem_id}.out"
        if os.path.exists(solutions):
            os.remove(solutions)


if __name__ == '__main__':
    generate_all()


