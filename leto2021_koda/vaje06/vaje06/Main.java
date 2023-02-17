package vaje06;

public class Main {
	public static void main(String[] args) {
		int[] vrednosti = new int[] {1, 1000, 100, 500, 1200, 2};
		BSTZaIntervale drevo = new BSTZaIntervale(vrednosti);
		System.out.println("Preizkusamo pred pristevanjem");
		preizkusi(vrednosti, drevo);
		drevo.pristejIntervalu(0, 2, 1);
		drevo.pristejIntervalu(1, 4, 2);
		System.out.println("Preizkusamo po pristevanju");
		preizkusi(new int[] {2, 1003, 103, 502, 1202, 2}, drevo);
		
	}
	
	public static int najdiMax(int[] a, int i0, int i1) {
		int m = a[i1];
		for (int i = i0; i < i1; i++) {
			m = Math.max(m, a[i]);
		}
		return m;
	}
	
	public static void preizkusi(int[] vrednosti, BSTZaIntervale drevo) {
		boolean vseOk = true;
		for (int i = 0; i < vrednosti.length; i++) {
			for (int j = i; j < vrednosti.length; j++) {
				int m = drevo.najdiMax(i, j);
				if (m != najdiMax(vrednosti, i, j)) {
					vseOk = false;
					System.out.println(String.format("max(%d, %d) = %d != %d", i, j, najdiMax(vrednosti, i, j), m));
				}	
			}
		}
		if (vseOk) {
			System.out.println("Bravo, vse je prav.");
		}
	}
}
