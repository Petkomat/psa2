package vaje03;

public class DinamicnaTabela {
	private boolean povecajZaK; // pove, katero taktiko povecevanja uporabimo
	private double alfa;
	private int k;
	private String[] tabela;
	private int kamDodam; // indeks naslednjega prostega elementa v tabela

	public DinamicnaTabela(double alfa, int zacetnaKapaciteta) {
		povecajZaK = false;
		this.alfa = alfa;
		tabela = new String[zacetnaKapaciteta];
		kamDodam = 0;

	}

	public DinamicnaTabela(int k, int zacetnaKapaciteta) {
		povecajZaK = true;
		this.k = k;
		tabela = new String[zacetnaKapaciteta];
		kamDodam = 0;
	}
	
	public void dodajElement(String x) {
		if (kamDodam >= tabela.length) {
			// povecaj kapaciteto
			int novaKapaciteta = povecajZaK ? tabela.length + k : (int) (alfa * tabela.length);
			String[] novaTabela = new String[novaKapaciteta];
			for (int i = 0; i < tabela.length; i++) {
				novaTabela[i] = tabela[i];
			}
			tabela = novaTabela;
		}
		tabela[kamDodam] = x;
		kamDodam++;
	}

	public String dobiElement(int index) {
		return tabela[index];
	}
}
