import matplotlib.pyplot as plt
import numpy as np


def nalozi_meritve(datoteka):
    with open(datoteka) as f:
        meritve = eval(f.readline())
        n = len(meritve)
        for i, x in enumerate(meritve):
            if x == -1:
                n = i
                break
        return meritve[:n]


def narisi_vse(skupni_casi):
    graf_alfa = nalozi_meritve("meritveAlfa.txt")
    graf_k = nalozi_meritve("meritveK.txt")
    graf_vgrajeno = nalozi_meritve("meritveVGR.txt")

    grafi = [(graf_alfa, "alfa", "r"), (graf_k, "K", "g"), (graf_vgrajeno, "vgrajeno", "b")]
    for graf, ime, barva in grafi:
        x = list(range(len(graf)))
        if skupni_casi:
            kum_graf = np.cumsum(graf)
            plt.plot(x, kum_graf, barva, label=f"{ime} (skupno)")
        else:
            plt.plot(x, graf, barva + "--", label=f"{ime} (korak)")
    plt.legend()
    plt.title("Primerjava casovnih zahtevnosti")
    plt.tight_layout()
    plt.show()


narisi_vse(True)
narisi_vse(False)
