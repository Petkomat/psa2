package vaje04;

import java.util.ArrayList;

/**
 * Implementacija splosne semidinamicne strukture za KDDrevesa.
 * Definirana kot podrazred KDDrevesa zaradi lazjega testiranja.
 * 
 * @author matejp
 *
 */
public class SemidinamicnoKDDrevo extends KDDrevo {
	ArrayList<KDDrevo> drevesa;
	
	
	public SemidinamicnoKDDrevo() {
		drevesa = new ArrayList<>();
	}
	
	@Override
	public void dodaj(double[] tocka) {
		int i0 = drevesa.size();
		for(int i = 0; i < drevesa.size(); i++) {
			if (drevesa.get(i) == null) {
				i0 = i;
				break;
			}
		}
		ArrayList<double[]> tocke = new ArrayList<>();
		for (int i = 0; i < i0; i++) {
			if (drevesa.get(i) != null) {
				tocke.addAll(drevesa.get(i).nastejElemente());
				drevesa.set(i, null);
			}
		}
		tocke.add(tocka);
		KDDrevo d = new KDDrevo(tocke, tocka.length);
		if (i0 >= drevesa.size()) {
			drevesa.add(d);
		} else {
			// naredimo na i0-tem mestu
			drevesa.set(i0, d);
		}
	}
	
	@Override
	public boolean jeNeuravnotezeno() {
		return false;
	}
	
	@Override
	public void rekonstruiraj() {
		
	}
	

}
