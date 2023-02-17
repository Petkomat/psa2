import matplotlib.pyplot as plt
import numpy as np
import itertools


def nalozi_meritve(datoteka, skupni_casi):
    def okrajsaj(sez):
        i = len(sez) - 1
        while sez[i] == 0:
            i -= 1
        return sez[:i + 1]

    with open(datoteka) as f:
        f.readline()
        vstavljanja = eval(f.readline())
        f.readline()
        poizvedbe = eval(f.readline())
    v, o = okrajsaj(vstavljanja), okrajsaj(poizvedbe)
    if not skupni_casi:
        v = [v[0]] + [v[i + 1] - v[i] for i in range(len(v) - 1)]
        # več kot 1 skupina za poizvedbe, ker so bile poizvedbe prehitre
        step = 50
        o = [o[i] - o[i - step] for i in range(step, len(o), step)]
    return v, o


STRUKTURE = ["AVL", "SkipList", "SplayTree"]
VSTAVLJANJE = ["PoVrsti", "RND"]
SIGME = ["0.1", "1.0", "10.0", "100.0", "INF"]
KOMBINACIJE = list(itertools.product(STRUKTURE, VSTAVLJANJE, SIGME))

def narisi_vstavljanja():
    fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(18, 9), sharey=False, sharex=True)
    colors = dict(zip(STRUKTURE, ["green", "blue", "orange"]))
    for sigma in SIGME:
        for i, vstavljanje in enumerate(VSTAVLJANJE):
            for s in STRUKTURE:
                for j, skupni_casi in enumerate([False, True]):
                    vstavljanja, _ = nalozi_meritve(
                        f"merjenje/{s}_vstavi{vstavljanje}_razprsenostPoizvedb{sigma}.txt", skupni_casi
                    )
                    label = s if i == 0 and sigma == SIGME[0] else None  # vse iste
                    axes[j, i].plot(vstavljanja, color=colors[s], label=label, alpha=0.5)
                    axes[j, i].set_xlabel("zaporedna skupina")
                    axes[j, 0].set_ylabel("čas [ms]")
                    nacin_merjenja = "Skupni (kumulatvni) časi dodajanj" if skupni_casi else "Časi dodajanj po skupinah"
                    axes[j, i].set_title(f"{nacin_merjenja} (vstavljanje {vstavljanje})")
    axes[0, 0].legend()
    axes[0, 0].set_ylim([0, 100])
    axes[0, 1].set_ylim([0, 100])
    axes[1, 0].set_ylim([0, 2000])
    axes[1, 1].set_ylim([0, 2000])
    plt.tight_layout()
    plt.savefig("merjenje/casi_vstavljanj.png")


def narisi_poizvedbe():

    colors = dict(zip(STRUKTURE, ["green", "blue", "orange"]))
    for sigma in SIGME:
        fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(18, 9), sharey=False, sharex=False)
        for i, vstavljanje in enumerate(VSTAVLJANJE):
            axes[i, 0].set_ylabel("čas [ms]")
            for j, skupni_casi in enumerate([False, True]):
                for s in STRUKTURE:
                    _, poizvedbe = nalozi_meritve(
                        f"merjenje/{s}_vstavi{vstavljanje}_razprsenostPoizvedb{sigma}.txt", skupni_casi
                    )
                    label = s if i == 0 and j == 0 else None
                    axes[i, j].plot(poizvedbe, color=colors[s], label=label, alpha=0.5)
                    x_label = "zaporedna skupina" if j == 1 else "zaporedna skupina 50 skupin"
                    axes[i, j].set_xlabel(x_label)
                    nacin_merjenja = "Skupni (kumulatvni) časi poizvedb" if skupni_casi else "Časi dodajanj po skupinah"
                    axes[i, j].set_title(f"{nacin_merjenja} (sigma = {sigma}, vstavljanje {vstavljanje})")
        axes[0, 0].legend()
        plt.tight_layout()
        plt.savefig(f"merjenje/casi_poizvedb_sigma{sigma}.png")

narisi_vstavljanja()
# narisi_poizvedbe()
