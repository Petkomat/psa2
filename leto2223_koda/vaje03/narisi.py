import matplotlib.pyplot as plt
from typing import List, Tuple
import numpy as np


def nalozi(datoteka):
    with open(datoteka) as f:
        casi = [int(line) / 10**6 for line in f]
    return casi


def narisi(casi: List[Tuple[str, List[int]]], kumulativno: bool) -> None:        
    ts = range(len(casi[0][1]))
    if kumulativno:
        marker = "-"
        y_label = "Skupen čas do dane meritve [s]"
    else:
        marker = ""
        y_label = "Čas za dano meritev [s]"

    for ime, meritve in casi:
        if kumulativno:
            meritve = np.cumsum(meritve)
        plt.plot(ts, meritve, marker, label=ime, alpha=0.5)

    plt.legend()
    plt.xlabel("Zaporedna številka merjenja (10 000 dodajanj)")
    plt.ylabel(y_label)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    vsi = [
        ("+ K (= 100 000)", nalozi("casiK.txt")),
        ("vgrajena", nalozi("casiVgrajena.txt")),
        ("* ALFA (= 1.2)", nalozi("casiAlfa.txt")), 
    ]
    narisi(vsi, True)
