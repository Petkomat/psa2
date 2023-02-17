package vaje03;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;

import vaje03.Stoparica;
import vaje03.VgrajenaTabela;

public class Main {

	public static void main(String[] args) {
		// Ustrezno izberi dinamicno tabelo in spodaj popravi ime datoteke:
		
		// DinamicnaTabela tabela = new DinamicnaTabela(1.2, 10);
		// DinamicnaTabela tabela = new DinamicnaTabela(100000, 10);
		VgrajenaTabela tabela = new VgrajenaTabela();
		int velikostSkupine = 10000;
		int steviloPonovitev = 10000;
		long[] meritve = new long[steviloPonovitev];
		Stoparica stoparica = new Stoparica();
		Stoparica globalna = new Stoparica();
		globalna.sprozi();
		for (int p = 0; p < steviloPonovitev; p++) {
			stoparica.sprozi();
			for (int i = 0; i < velikostSkupine; i++) {
				tabela.dodajElement("?");
			}
			stoparica.ustavi();
			meritve[p] = stoparica.izmerjeno();
			stoparica.resetiraj();
			
			globalna.ustavi();
			if (globalna.izmerjeno() > 5 * 60000) {
				System.out.println("Ustavljam po 5 minutah");
				if (p < steviloPonovitev - 1) {
					meritve[p + 1] = -1;
				}
				break;
			}
			globalna.sprozi();
		}
		
		FileWriter writer;
		try {
			writer = new FileWriter("C:\\Users\\matejp\\git\\psa2\\vaje03\\meritveVGR.txt");
			writer.write(Arrays.toString(meritve));
			writer.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("Konec");
	}

}
