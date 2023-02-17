package vaje12;

import java.util.Arrays;

public class FFT {
	/**
	 * Hitra Fourierova transformacija (pa tudi njen inverz).
	 * 
	 * @param a          seznam compleksnih stevil
	 * @param jeInverzno
	 * @return
	 */
	public static Complex[] fft(Complex[] a, boolean jeInverzno) {
		int n = a.length;
		if (n == 1) {
			return a;
		} else {
			Complex[] sodi = new Complex[n / 2];
			Complex[] lihi = new Complex[n / 2];
			for (int i = 0; 2 * i < n; i++) {
				sodi[i] = a[2 * i];
				lihi[i] = a[2 * i + 1];
			}
			Complex[] sodiFFT = fft(sodi, jeInverzno);
			Complex[] lihiFFT = fft(lihi, jeInverzno);
			Complex w = new Complex(1.0);
			double alfa = 2 * Math.PI / n * (jeInverzno ? -1 : 1);
			Complex w0 = new Complex(Math.cos(alfa), Math.sin(alfa));
			for (int i = 0; 2 * i < n; i++) {
				Complex c = w.times(lihiFFT[i]);
				a[i] = sodiFFT[i].plus(c);
				a[i + n / 2] = sodiFFT[i].minus(c);
				if (jeInverzno) {
					a[i] = a[i].divide(2.0);
					a[i + n / 2] = a[i + n / 2].divide(2.0);
				}
				w = w.times(w0);
			}
		}
		return a;
	}

	public static double[] zmnoziPolinome(double[] a, double[] b) {
		int stA = a.length - 1;
		int stB = b.length - 1;
		int n = 1; // dolzina razsirjenih seznamov: vsaj za 1 vec kot stopnja produkta
		while (n <= stA + stB) {
			n *= 2;
		}
		// razsiri do ustrezne dolzine in pretvori v compleksna stevila
		Complex[] aC = new Complex[n];
		Complex[] bC = new Complex[n];
		for (int i = 0; i < n; i++) {
			aC[i] = i <= stA ? new Complex(a[i]) : new Complex(0.0);
			bC[i] = i <= stB ? new Complex(b[i]) : new Complex(0.0);
		}
		// transformiraj
		Complex[] aFFT = FFT.fft(aC, false);
		Complex[] bFFT = FFT.fft(bC, false);
		// po tockah zmnozi
		Complex[] abFFT = new Complex[n];
		for (int i = 0; i < n; i++) {
			abFFT[i] = aFFT[i].times(bFFT[i]);
		}
		// odtransformiraj
		double[] ab = Complex.toDouble(FFT.fft(abFFT, true));
		return Arrays.copyOf(ab, stA + stB + 1);
	}

	public static String zmnoziStevila(String a, String b) {
		// pretvorimo v seznam
		double[] aD = predelajVSeznam(a);
		double[] bD = predelajVSeznam(b);
		// zmnozimo
		double[] ab = zmnoziPolinome(aD, bD);
		// poskrbimo za vecmestna stevila: zadnjo stevko zapisemo, ostalo nesemo naprej
		String[] odgovor = new String[ab.length];
		for (int i = 0; i < ab.length; i++) {
			int n = (int) Math.round(ab[i]); // zaokrozimo, ker so vhodi bili cela stevila
			if (i < ab.length - 1) {
				odgovor[odgovor.length - i - 1] = Integer.toString(n % 10);
				ab[i + 1] += n / 10;
			} else {
				odgovor[odgovor.length - i - 1] = Integer.toString(n);
			}
		}
		// poskrbimo za vodilne nicle
		int odKod = 0;
		while (odgovor[odKod].equals("0")) {
			if (odKod == odgovor.length - 1) {
				break;
			}
			odKod++;
		}
		return String.join("", Arrays.copyOfRange(odgovor, odKod, odgovor.length));
	}

	private static double[] predelajVSeznam(String x) {
		double[] xD = new double[x.length()];
		for (int i = 0; i < xD.length; i++) {
			xD[xD.length - 1 - i] = x.charAt(i) - '0';
		}
		return xD;
	}

}
