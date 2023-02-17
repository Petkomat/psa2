package vaje06;

public class Vozlisce {
	private int kljuc;
	private int alfa;
	private int beta;
//	private leto2223.strukture.Vozlisce levi;
//	private leto2223.strukture.Vozlisce desni;
//	private leto2223.strukture.Vozlisce stars;
	
	public Vozlisce(int kljuc) {
		this.kljuc = kljuc;
	}
	
	public Vozlisce(int kljuc, int alfa, int beta) {
		this.kljuc = kljuc;
		this.alfa = alfa;
		this.beta = beta;
	}
	
	@Override
	public String toString() {
		return String.format("V(%d, %d, %d)", kljuc, alfa, beta);
	}
	
	public int getKljuc() {
		return kljuc;
	}

	public int getAlfa() {
		return alfa;
	}
	
	public int getBeta() {
		return beta;
	}
	
	public void setBeta(int b) {
		this.beta = b;
	}

	public void setAlfaBeta(int a, int b) {
		this.alfa = a;
		this.beta = b;
	}
	
//	public void poveziZOtrokom(leto2223.strukture.Vozlisce otrok, boolean jeLevi) {
//		if (jeLevi) {
//			levi = otrok;
//		} else {
//			desni = otrok;
//		}
//		otrok.stars = this;
//	}
}
