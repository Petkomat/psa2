package vaje06;

import java.util.stream.IntStream;

public class BSTZaIntervale {
	private Vozlisce[] vozlisca; // koren na mestu 0, otroka i-tega na mestu 2i + 1, 2i + 2
	private int nListovNivoVisje;

	public BSTZaIntervale(int[] vrednosti) {
		this(IntStream.range(0, vrednosti.length).toArray(), vrednosti);
	}

	/**
	 * 
	 * @param kljuci Urejen seznam kljucev
	 */
	public BSTZaIntervale(int[] kljuci, int[] vrednosti) {
		if (kljuci.length == 0) {
			vozlisca = new Vozlisce[0];
		} else {
			narediVozlisca(kljuci, vrednosti);
		}
	}

	/**
	 * Drevo polnimo tako, da je koren na mestu 0, otroka i-tega vozlisca pa sta na
	 * na mestih 2i + 1 in 2i + 2. Podane vrednosti in kljuci bodo v listih, tj.
	 * vozliscih na zadnjih n mestih. Med polnjenjem je treba paziti, da so kljuci
	 * urejeni. Npr: Ce podamo stiri kljuce (0, 1, 2, 3), je drevo tako: koren levi
	 * desni 0 1 2 3
	 * 
	 * Seznam vozlisc je torej videti tako: [..., 0, 1, 2, 3]
	 * 
	 * Ce podamo le tri kljuce (0, 1, 2), pa dobimo tako drevo: koren levi 2 0 1
	 * 
	 * Seznam vozlisc je torej videti tako: [..., 2, 0, 1]
	 * 
	 * Zgodi se torej lahko, da je del listov nivo vise. Ker je drevo iskalno,
	 * morajo to biti vecji izmed kljucev.
	 * 
	 * Kljuci v starsih so povprecja kljucev njihovih otrok, kar pomeni, da pri
	 * iskanju sledimo levi veji, ce je iskana vrednost <= kljuc, in desni sicer.
	 * 
	 * @param kljuci
	 * @param vrednosti
	 */
	private void narediVozlisca(int[] kljuci, int[] vrednosti) {
		int n = kljuci.length;
		int m = 2 * n - 1;
		vozlisca = new Vozlisce[m];
		nListovNivoVisje = kolikoListovNaPrejsnjemNivoju(n);
		// zadnjih nekaj vozlisc damo na zacetek: n - 1 je indeks prvega lista
		for (int i = 0; i < nListovNivoVisje; i++) {
			int kam = n - 1 + i;
			int kateri = n - nListovNivoVisje + i;
			vozlisca[kam] = new Vozlisce(kljuci[kateri], vrednosti[kateri], vrednosti[kateri]);
		}
		// preostale liste dodamo za zgornjimi
		for (int i = nListovNivoVisje; i < n; i++) {
			int kam = n - 1 + i;
			int kateri = i - nListovNivoVisje;
			vozlisca[kam] = new Vozlisce(kljuci[kateri], vrednosti[kateri], vrednosti[kateri]);
		}
		// notranja vozlisca dodamo v vzratnem vrstem redu, da si zagotovimo,
		// da sta ob ustvarjenju starsa otroka ze prisotna
		for (int i = n - 2; i >= 0; i--) {
			Vozlisce levi = vozlisca[2 * i + 1];
			Vozlisce desni = vozlisca[2 * i + 2];
			int vrL = levi.getBeta();
			int vrD = desni.getBeta();
			int kljucStars = (levi.getKljuc() + desni.getKljuc()) / 2;
			int vrStars = izracunajBetaIzSinov(vrL, vrD);
			vozlisca[i] = new Vozlisce(kljucStars, 0, vrStars);
		}
	}

	private int izracunajBetaIzSinov(int levo, int desno) {
		return Math.max(levo, desno);
	}

	private int kolikoListovNaPrejsnjemNivoju(int nListov) {
		int nNotranjih = nListov - 1;
		int velikostPolnega = 1;
		while (velikostPolnega < nNotranjih) {
			velikostPolnega = 2 * velikostPolnega + 1;
		}
		return velikostPolnega - nNotranjih;
	}

	@Override
	public String toString() {
		String[] strVozlisca = new String[vozlisca.length];
		for (int i = 0; i < strVozlisca.length; i++) {
			strVozlisca[i] = vozlisca[i].toString();
		}
		return String.format("[%s]", String.join(", ", strVozlisca));
	}

	public boolean isEmpty() {
		return vozlisca.length == 0;
	}

	/**
	 * Ce iscemo predhodnika, najde najvecjega med kljuci, za katere velja k <= i0.
	 * <br>
	 * Ce iscemo naslednika, najde najmanjšsga med kljuci, za katere velja k >= i0
	 * 
	 * @param i0
	 * @param predhodnik
	 * @return
	 */
	public int najdiSoseda(int i0, boolean predhodnik) {
		int nVozlisc = vozlisca.length; // = 2 nListov - 1
		int nListov = (nVozlisc + 1) / 2;
		int i = 0; // prvi list
		int j = nListov - 1; // zadnji list
		int vmes;
		while (j - i > 1) {
			vmes = (i + j) / 2;
			if (i0 <= vozlisca[zamakniIndeks(vmes, nListov)].getKljuc()) {
				j = vmes;
			} else {
				i = vmes;
			}
		}
		// nazaj v absolutne koordinate
		i = zamakniIndeks(i, nListov);
		j = zamakniIndeks(j, nListov);
		if (i == j) {
			return i;
		} else if (vozlisca[i].getKljuc() == i0) {
			return i;
		} else if (vozlisca[j].getKljuc() == i0) {
			return j;
		} else if (predhodnik) {
			return i;
		} else {
			return j;
		}
	}

	private int zamakniIndeks(int i, int nListov) {
		return (i + nListovNivoVisje) % nListov + (nListov - 1);
	}

	/**
	 * Izracuna maximum na podseznamu z indeksi [i0, i1]
	 * @param i0
	 * @param i1
	 * @return
	 */
	public int najdiMax(int i0, int i1) {

		int iLevo = najdiSoseda(i0, false); // naslednik od i0
		int iDesno = najdiSoseda(i1, true);// predhodnik od i1
		int max = Integer.MIN_VALUE;
		return max;
	}

	/**
	 * Pristeje vsem elementov z indeksi s podintervala [i0, i1] vrednost c, tako da
	 * posodobi vrednosti alfa in beta ustreznih mestih. Postopek je zelo podoben
	 * tistemu, ko iscemo max.
	 * 
	 * @param i0
	 * @param i1
	 * @param c
	 */
	public void pristejIntervalu(int i0, int i1, int c) {
		int iLevo = najdiSoseda(i0, false); // naslednik od i0
		int iDesno = najdiSoseda(i1, true);// predhodnik od i1
		int iZadnjiSkupniPrednik = -1;
		if (vozlisca[iLevo].getKljuc() > vozlisca[iDesno].getKljuc()) {
			// interval je prazen
		} else if (iLevo == iDesno) {
			// smo v listu
			int alfa = vozlisca[iLevo].getAlfa();
			int beta = vozlisca[iLevo].getBeta();
			vozlisca[iLevo].setAlfaBeta(alfa + c, beta + c);
			iZadnjiSkupniPrednik = (iLevo - 1) / 2;
		} else {
			// popravimo najprej levi in desni list
			int alfa = vozlisca[iLevo].getAlfa();
			int beta = vozlisca[iLevo].getBeta();
			vozlisca[iLevo].setAlfaBeta(alfa + c, beta + c);
			alfa = vozlisca[iDesno].getAlfa();
			beta = vozlisca[iDesno].getBeta();
			vozlisca[iDesno].setAlfaBeta(alfa + c, beta + c);
			// popravimo do skupnega prednika
			int starsLevo = (iLevo - 1) / 2;
			int starsDesno = (iDesno - 1) / 2;
			// na vsakem koraku obdelamo le enega starsa, da ne zgresimo skupnega prednika
			while (starsLevo != starsDesno) {
				if (starsLevo > starsDesno) {
					if (iLevo % 2 == 1) {
						// levi sin --> popraviti je treba se celo desno poddrevo
						alfa = vozlisca[iLevo + 1].getAlfa();
						beta = vozlisca[iLevo + 1].getBeta();
						vozlisca[iLevo + 1].setAlfaBeta(alfa + c, beta + c);
					} else {
						// desni sin --> samo posljemo vrednost navzgor
					}
					beta = Math.max(vozlisca[2 * starsLevo + 1].getBeta(), vozlisca[2 * starsLevo + 2].getBeta());
					vozlisca[starsLevo].setBeta(beta + vozlisca[starsLevo].getAlfa());
					iLevo = starsLevo;
					starsLevo = (iLevo - 1) / 2;
				} else {
					if (iDesno % 2 == 0) {
						// desni sin --> preveriti je treba se celo levo poddrevo
						alfa = vozlisca[iDesno + 1].getAlfa();
						beta = vozlisca[iDesno + 1].getBeta();
						vozlisca[iLevo + 1].setAlfaBeta(alfa + c, beta + c);
					}
					beta = Math.max(vozlisca[2 * starsDesno + 1].getBeta(), vozlisca[2 * starsDesno + 2].getBeta());
					vozlisca[starsDesno].setBeta(beta + vozlisca[starsDesno].getAlfa());
					iDesno = starsDesno;
					starsDesno = (iDesno - 1) / 2;
				}
			}
			// sedaj smo v skupnem predniku ...
			iZadnjiSkupniPrednik = starsLevo; // = starsDesno
		}
		// ... in popravljamo le se bete (lahko prekinemo, ko se beta prvic ne spremeni)
		while (iZadnjiSkupniPrednik >= 0) {
			int betaLevo = vozlisca[2 * iZadnjiSkupniPrednik + 1].getBeta();
			int betaDesno = vozlisca[2 * iZadnjiSkupniPrednik + 2].getBeta();
			int novaBeta = Math.max(betaLevo, betaDesno) + vozlisca[iZadnjiSkupniPrednik].getAlfa();
			if (novaBeta == vozlisca[iZadnjiSkupniPrednik].getBeta()) {
				break;
			}
			vozlisca[iZadnjiSkupniPrednik].setBeta(novaBeta);
			iZadnjiSkupniPrednik = (iZadnjiSkupniPrednik - 1) / 2;
		}
	}
}
