package vaje12;

import java.util.Arrays;

public class Main {

	public static void main(String[] args) {
		double[] p1 = new double[] {1, 2, 3};
		double[] p2 = new double[] {3, 1};
		// (1 + 2x + 3x^2)(3 + x) = 3 + 7x + 11x^2 + 3x^3
		System.out.println(
				String.format(
						"Produkt polinomov %s in %s je %s",
						Arrays.toString(p1), Arrays.toString(p2), Arrays.toString(FFT.zmnoziPolinome(p1, p2))
				)
		);
		// testirajmo mnozenje
		testirajMnozenjeStevil();
	}
	
	public static void testirajMnozenjeStevil() {
		String[][] a = new String[][] {
			new String[] {"0", "0", "0"},
			new String[] {"1", "1", "1"},
			new String[] {"21", "2", "42"},
			new String[] {"2", "21", "42"},
			new String[] {"5", "12", "60"},
			new String[] {"12", "5", "60"},
			new String[] {"1234", "789", "973626"},
			new String[] {"9999", "9999", "99980001"},
			new String[] {"123434221321", "74243243243289", "9164156918080972273964769"},
			new String[] {"1".repeat(100), "1".repeat(100), "123456790".repeat(11) + "0" + "987654320".repeat(10) + "987654321"}
		};
		for (String[] x_y_xy : a) {
			String x = x_y_xy[0];
			String y = x_y_xy[1];
			String xy = x_y_xy[2];
			String odgovor = FFT.zmnoziStevila(x, y);
			if (!odgovor.equals(xy)) {
				System.out.println(String.format("%s * %s je %s, ne pa %s", x, y, xy, odgovor));
				return;
			}
		}
		System.out.println("Množiti pa znaš!");
	}

}
