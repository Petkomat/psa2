package vaje04;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Stack;

/**
 * Implementacija KDDrevesa, s katero lahko
 * <ul>
 * <li>naredimo drevo
 * <li>vanj dodamo tocko
 * <li>rekonstruiramo drevo (tako, da je spet uravnotezeno)
 * </ul>
 * Iskanje sosedov kdaj naslednjic, ko bo to aktualno :) <br>
 * <br>
 * Drevo je lahko
 * <li>prazno: ima doloceno le
 * <ul>
 * <li>razseznost prostora
 * <li>dimnezijo, vzdolz katere delimo
 * <li>globino (0) in stevilo tock, ki jih drevo vsebuje (0)
 * </ul>
 * <li>neprazno: poleg gornjega, dolocimo tudi
 * <ul>
 * <li>levega in desnega otroka (oboje sta KDDrevesi)
 * <li>mejo: tocke, za katere velja tocka[dim] <= meja, gredo v levega otroka,
 * preostale v desnega
 * <li>tocko, ki je shranjena v korenu tega drevesa. Za tocko velja tocka[dim] =
 * meja.
 * </ul>
 * </ul>
 * 
 * @author matejp
 *
 */
public class KDDrevo {
	int k;
	int n_vozlisc;
	int globina;
	int dim;
	double meja;
	double[] tocka;

	KDDrevo leviOtrok;
	KDDrevo desniOtrok;
	KDDrevo stars;

	/**
	 * Konstruktor, ki ga moramo definirati zaradi podrazreda SemidinamicnoKDDrevo
	 */
	public KDDrevo() {
	}
	
	/**
	 * Konstruktor za prazno drevo.
	 */
	public KDDrevo(int k) {
		this(new ArrayList<double[]>(), k);
	}

	/**
	 * Glavni kosntruktor
	 * 
	 * @param tocke Seznam tock. Vsi njegovi elementi morajo biti dolzine k
	 * @param k     Dimenzija prostora, v katerem so tocke
	 */
	public KDDrevo(ArrayList<double[]> tocke, int k) {
		// this(tocke, 0, k); // rekurzivna implementacija, ki se bo sesula ob zelo
		// globokih drevesih
		this(k, tocke.size(), 0);
		narediIterativno(tocke, k);
	}

	/**
	 * Pomozni konstruktor, ki nastavi samo k, n_vozlisc in dimenzijo, vzdolz katere
	 * dano vozlisce deli prostor.
	 * 
	 * @param k
	 * @param n_vozlisc
	 * @param dim
	 */
	public KDDrevo(int k, int n_vozlisc, int dim) {
		this.k = k;
		this.n_vozlisc = n_vozlisc;
		this.dim = dim;
		meja = Double.NaN;
		tocka = null;

		stars = null;
		leviOtrok = null;
		desniOtrok = null;
	}

	/**
	 * Nastavi this na drugo drevo.
	 * 
	 * @param koren
	 */
	private void noviKoren(KDDrevo koren) {
		this.k = koren.k;
		this.n_vozlisc = koren.n_vozlisc;
		this.globina = koren.globina;
		this.dim = koren.dim;
		this.meja = koren.meja;
		this.tocka = koren.tocka;
		this.leviOtrok = koren.leviOtrok;
		this.desniOtrok = koren.desniOtrok;
		this.leviOtrok.stars = this;
		this.desniOtrok.stars = this;
	}

	/**
	 * Naredi KDDrevo na iterativen, a zato manj razumljiv nacin kot rekurzivni
	 * konstruktor. Potek:
	 * <ul>
	 * <li> naredimo vsa drevesa, a ne racunamo globin in povezav stars-otrok
	 * <li> nastavimo povezave stars-otrok
	 * <li> izracunamo globine
	 * </ul>
	 * 
	 * @param vseTocke
	 * @param k
	 */
	private void narediIterativno(ArrayList<double[]> vseTocke, int k) {
		// Hranimo pare (indeks starsa, drevo), a da ne pisemo se razreda Par damo eno
		// komponento na null: (iStars, null, drevo)
		ArrayList<Trojka<Integer, Integer, KDDrevo>> drevesa = new ArrayList<>();
		// seznam listov
		ArrayList<KDDrevo> listi = new ArrayList<>();
		// sklad trojic (dimenzija, iStars, tocke)
		Stack<Trojka<Integer, Integer, ArrayList<double[]>>> todo = new Stack<>();
		todo.push(new Trojka<Integer, Integer, ArrayList<double[]>>(0, -1, vseTocke));
		while (!todo.isEmpty()) {
			Trojka<Integer, Integer, ArrayList<double[]>> p = todo.pop();
			int dim = p.getPrvi();
			int iStars = p.getDrugi();
			ArrayList<double[]> tocke = p.getTretji();
			KDDrevo d;
			if (drevesa.size() == 0) {
				// koren
				d = this;
			} else {
				// naredimo novo drevo
				d = new KDDrevo(k, tocke.size(), dim);
			}
			drevesa.add(new Trojka<Integer, Integer, KDDrevo>(iStars, null, d));
			if (tocke.size() == 0) {
				// prazno drevo
				listi.add(d);
			} else {
				// neprazno drevo

				// otroka
				ArrayList<double[]> levo = new ArrayList<>();
				ArrayList<double[]> desno = new ArrayList<>();
				ArrayList<Double> xs = new ArrayList<>();
				for (int i = 0; i < tocke.size(); i++) {
					xs.add(tocke.get(i)[dim]);
				}
				// najdemo mediano in razdelimo tocke
				double m = mediana(xs);
				for (int i = 0; i < tocke.size(); i++) {
					double x = tocke.get(i)[dim];
					if (x < m) {
						levo.add(tocke.get(i));
					} else if (x > m) {
						desno.add(tocke.get(i));
					} else if (Double.isNaN(d.meja)) {
						d.meja = x;
						d.tocka = tocke.get(i);
					} else {
						// nekam ga pac posljemo
						levo.add(tocke.get(i));
					}
				}
				int indeksToDrevo = drevesa.size() - 1;
				// dodamo na todo levega in desnega sina
				todo.push(new Trojka<Integer, Integer, ArrayList<double[]>>((dim + 1) % k, indeksToDrevo, levo));
				todo.push(new Trojka<Integer, Integer, ArrayList<double[]>>((dim + 1) % k, indeksToDrevo, desno));
			}
		}
		// poskrbi za povezave otrok-stars
		for (Trojka<Integer, Integer, KDDrevo> t : drevesa) {
			int iStars = t.getPrvi();
			KDDrevo d = t.getTretji();
			if (iStars >= 0) {
				KDDrevo stars = drevesa.get(iStars).getTretji();
				d.stars = stars;
				if (stars.desniOtrok == null) {
					// desni otrok bo v seznamu pred levim
					stars.desniOtrok = d;
				} else if (stars.leviOtrok == null) {
					stars.leviOtrok = d;
				} else {
					throw new RuntimeException("Drevo ima najvec dva otroka!");
				}
			}
		}
		// izracunaj globine
		ArrayList<KDDrevo> noviListi;
		while (!listi.isEmpty()) {
			noviListi = new ArrayList<>();
			for (KDDrevo list : listi) {
				KDDrevo stars = list.stars;
				if (stars != null && stars.leviOtrok == list) {
					stars.globina = Math.max(stars.leviOtrok.globina, stars.desniOtrok.globina) + 1;
					noviListi.add(stars);
				}
			}
			listi = new ArrayList<>();
			listi.addAll(noviListi);
		}
	}

	/**
	 * Rekurzivni konstruktor za drevo. Ne deluje za globoka drevesa :)
	 * 
	 * @param tocke
	 * @param nivo
	 * @param k
	 */
	public KDDrevo(ArrayList<double[]> tocke, int nivo, int k) {
		this.k = k;
		n_vozlisc = tocke.size();
		dim = nivo % k;
		meja = Double.NaN;
		tocka = null;
		stars = null;
		if (tocke.size() == 0) {
			globina = 0;
		} else {
			ArrayList<double[]> levo = new ArrayList<>();
			ArrayList<double[]> desno = new ArrayList<>();
			ArrayList<Double> xs = new ArrayList<>();
			for (int i = 0; i < tocke.size(); i++) {
				xs.add(tocke.get(i)[dim]);
			}
			// najdemo mediano in razdelimo tocke
			double m = mediana(xs);
			for (int i = 0; i < tocke.size(); i++) {
				double x = tocke.get(i)[dim];
				if (x < m) {
					levo.add(tocke.get(i));
				} else if (x > m) {
					desno.add(tocke.get(i));
				} else if (Double.isNaN(meja)) {
					meja = x;
					tocka = tocke.get(i);
				} else {
					// nekam ga pac posljemo
					levo.add(tocke.get(i));
				}
			}
			// rekurzivni klic
			leviOtrok = new KDDrevo(levo, nivo + 1, k);
			desniOtrok = new KDDrevo(desno, nivo + 1, k);
			leviOtrok.stars = this;
			desniOtrok.stars = this;
			globina = Math.max(leviOtrok.globina, desniOtrok.globina) + 1;
		}

	}

	@Override
	public String toString() {
		if (!jePrazno()) {
			return String.format("Drevo(%s; tocka[%d] <= %f)", Arrays.toString(tocka), dim, meja);
		} else {
			return "Drevo()";
		}
	}

	public boolean jePrazno() {
		return tocka == null;
	}

	public void dodaj(double[] tocka) {
		KDDrevo list = this;
		// gremo do dol ...
		while (!list.jePrazno()) {
			list = tocka[list.dim] <= list.meja ? list.leviOtrok : list.desniOtrok;
		}
		// v praznem listu
		list.tocka = tocka;
		list.meja = tocka[list.dim];
		list.leviOtrok = new KDDrevo(new ArrayList<>(), list.dim + 1, k);
		list.desniOtrok = new KDDrevo(new ArrayList<>(), list.dim + 1, k);
		list.leviOtrok.stars = list;
		list.desniOtrok.stars = list;
		// ... pa nazaj gor
		while (list != null) {
			list.n_vozlisc++;
			list.globina = Math.max(list.leviOtrok.globina, list.desniOtrok.globina) + 1;
			list = list.stars;
		}
	}

	/**
	 * Na novo zgradi drevo, tako da je to spet uravnotezeno
	 */
	public void rekonstruiraj() {
		ArrayList<double[]> tocke = nastejElemente();
		KDDrevo d = new KDDrevo(tocke, k);
		noviKoren(d);
	}

	/**
	 * Vrne seznam tock, ki so v drevesu.
	 * @return
	 */
	public ArrayList<double[]> nastejElemente() {
		ArrayList<double[]> tocke = new ArrayList<>();
		Stack<KDDrevo> todo = new Stack<>();
		if (!this.jePrazno()) {
			todo.push(this);
		}
		while (!todo.isEmpty()) {
			KDDrevo vrh = todo.pop();
			tocke.add(vrh.tocka);
			if (!vrh.leviOtrok.jePrazno()) {
				todo.add(vrh.leviOtrok);
			}
			if (!vrh.desniOtrok.jePrazno()) {
				todo.add(vrh.desniOtrok);
			}
		}
		return tocke;
	}

	/**
	 * Preprosto pravilo, ki pove, kdaj naj rekonstruiramo drevo: takrat, ko globina preseze petkratnik
	 * globine, ki bi ga imelo uravnotezeno drevo.
	 * @return
	 */
	public boolean jeNeuravnotezeno() {
		return globina > 1 && globina > 5 * Math.log(n_vozlisc) / Math.log(2);
	}

	/**
	 * Izracuna mediano, tako da je ta vedno element seznama tock, iz katerih jo racunamo.
	 * Prirejeno po https://stackoverflow.com/a/28822243/2281318
	 * @param xs
	 * @return
	 */
	private static double mediana(ArrayList<Double> xs) {
		int n = xs.size() / 2;
		if (xs.size() % 2 == 0)
			return nth(xs, n); // Raje ne (nth(xs, n - 1)+ nth(xs, n)) / 2.0;
		else
			return nth(xs, n);
	}

	/**
	 * Najde n-ti najvecji element v seznamu. Za pivot izberemo sredinskega.
	 * Prirejeno po https://stackoverflow.com/a/28822243/2281318
	 * @param xs
	 * @param n 0 <= n < xs.size()
	 * @return
	 */
	public static double nth(ArrayList<Double> xs, int n) {
		ArrayList<Double> underPivot;
		ArrayList<Double> overPivot;
		ArrayList<Double> equalPivot;

		ArrayList<Double> xsCurrent = new ArrayList<>();
		xsCurrent.addAll(xs);
		while (true) {
			double pivot = xsCurrent.get(n / 2); // ...
			underPivot = new ArrayList<>();
			overPivot = new ArrayList<>();
			equalPivot = new ArrayList<>();
			for (double obj : xsCurrent) {
				if (obj < pivot) {
					underPivot.add(obj);
				} else if (obj > pivot) {
					overPivot.add(obj);
				} else {
					equalPivot.add(obj);
				}
			}
			if (equalPivot.size() == 0) {
				throw new RuntimeException("Wrong");
			}
			if (n < underPivot.size()) {
				xsCurrent = underPivot;
			} else if (n < underPivot.size() + equalPivot.size()) {
				return pivot;
			} else {
				xsCurrent = overPivot;
				n = n - underPivot.size() - equalPivot.size();
			}
		}
	}

}
