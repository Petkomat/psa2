package vaje04;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.Random;

import vaje03.Stoparica;

public class Main {

	public static void main(String[] args) {
		// dodajamo tocke v
		// a) semidinamicno drevo
		// b) staticno drevo (se lahko porusi ravnotezje)
		// Pravilo za rekonstrukcijo:
		// drevo naredimo na novo, ko globina preseze 5 x log (stevilo tock)

		// testiramo, ali stvar deluje
		// ArrayList<double[]> tocke = new ArrayList<>();
		// for (int i = 0; i < 7; i++) {
		// tocke.add(new double[] {i, i});
		// }
		// KDDrevo dr = new KDDrevo(tocke, 2);

		boolean generirajPrijazno = false;
		int k = 2;
		// izberi strukturo
		
		// KDDrevo d = new KDDrevo(2);
		SemidinamicnoKDDrevo d = new SemidinamicnoKDDrevo();
		
		int n = 10000;
		int velikostSkupine = 10;
		long[] meritve = new long[n];
		Stoparica stoparica = new Stoparica();
		Stoparica globalna = new Stoparica();
		globalna.sprozi();
		for (int i = 0; i < n; i++) {
			stoparica.resetiraj();
			stoparica.sprozi();
			for (int j = 0; j < velikostSkupine; j++) {
				double[] tocka = generirajTocko(velikostSkupine * i +  j, k, generirajPrijazno);
				d.dodaj(tocka);
				// morda moram rekonstruirati
				if (d.jeNeuravnotezeno()) {
					// pogosto printanje je casovno kar zahtevno, zato ga ob merjenju ne izvajamo
					// System.out.println("Globina: " + d.globina);
					d.rekonstruiraj();
					// System.out.println("Rekonstrukcija pri i = " + i);
					// System.out.println("Globina: " + d.globina + "\n");
				}
			}
			stoparica.ustavi();
			meritve[i] = stoparica.izmerjeno();
			globalna.ustavi();
			if (globalna.izmerjeno() > 5 * 60000) {
				System.out.println(globalna.izmerjeno());
				System.out.println("Ustavljam po 5 minutah");
				if (i < n - 1) {
					meritve[i + 1] = -1;
				}
				break;
			}
			globalna.sprozi();
		}
		System.out.println("Skupen cas [ms]: " + globalna.izmerjeno());
		FileWriter writer;
		try {
			String tip = (d instanceof SemidinamicnoKDDrevo) ? "semidinamicno" : "staticno";
			String mnozica = generirajPrijazno ? "prijazna" : "neprijazna";
			writer = new FileWriter(String.format("C:\\Users\\matejp\\git\\psa2\\vaje04\\meritve_%s_%s.txt", tip, mnozica));
			writer.write(Arrays.toString(meritve));
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static double[] generirajTocko(int i, int k, boolean prijaznoGeneriranje) {
		double[] tocka = new double[k];
		if (prijaznoGeneriranje) {
			Random r = new Random(i);
			for (int j = 0; j < k; j++) {
				tocka[j] = r.nextDouble();
			}
		} else {
			Arrays.fill(tocka, i);
		}
		return tocka;
	}

}
