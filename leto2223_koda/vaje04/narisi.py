import matplotlib.pyplot as plt
from typing import List, Tuple
import numpy as np


def nalozi(datoteka):
    with open(datoteka) as f:
        casi = [int(line) / 10**6 for line in f]
    return casi


def narisi(casi: List[Tuple[str, List[int]]], kumulativno: bool, glajenje: int = 1) -> None:
    if kumulativno:
        marker = "-"
        y_label = "Skupen čas do dane meritve [s]"
    else:
        marker = ""
        y_label = "Čas za dano meritev [s]"

    for ime, meritve in casi:
        if glajenje > 1:
            meritve = np.convolve(meritve, np.ones(glajenje) / glajenje, mode="valid")
        ts = range(glajenje, len(meritve) + glajenje)
        if kumulativno:
            meritve = np.cumsum(meritve)
        plt.plot(ts, meritve, marker, label=ime, alpha=0.5)

    plt.legend()
    plt.xlabel("Število elementov ob iskanju")
    plt.ylabel(y_label)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    dim = 10
    vsi = [
        ("primer1", nalozi(f"primer1_{dim}.txt")),
        ("primer2", nalozi(f"primer2_{dim}.txt")),
        ("primer3", nalozi(f"primer3_{dim}.txt")),
    ]
    narisi(vsi, False, 5)
