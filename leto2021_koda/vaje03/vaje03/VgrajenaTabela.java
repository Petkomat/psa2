package vaje03;

import java.util.ArrayList;

public class VgrajenaTabela {
	ArrayList<String> tabela;
	
	public VgrajenaTabela() {
		tabela = new ArrayList<>();
	}
	
	public void dodajElement(String x) {
		tabela.add(x);
	}

}
