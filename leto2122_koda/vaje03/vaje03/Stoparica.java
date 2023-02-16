package vaje03;

public class Stoparica {
	private long izmerjeno = 0;
	private long zadnji_zacetek = 0;
	
	public Stoparica() {
		// ne potrebujemo, samo zaradi preglednosti
	}
	
	public void sprozi() {
		zadnji_zacetek =  java.lang.System.currentTimeMillis();
	}
	
	public void ustavi() {
		long zdaj = java.lang.System.currentTimeMillis();
		izmerjeno += zdaj - zadnji_zacetek;
	}
	
	public void resetiraj() {
		izmerjeno = 0;
	}
	
	public long izmerjeno() {
		return izmerjeno;
	}
}
