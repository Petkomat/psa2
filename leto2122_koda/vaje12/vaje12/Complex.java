package vaje12;

/**
 * Minimalisticna implementacija kompleksnih stevil - samo tisto, kar rabimo za FFT :)
 * @author matejp
 *
 */
public class Complex {
	private final double re;
	private final double im;

	public Complex(double re, double im) {
		this.re = re;
		this.im = im;
	}
	
	public Complex(double re) {
		this(re, 0.0);
	}
	
	public Complex times(Complex other) {
		// (a, b) * (c, d) = (ac - bd, ad + bc)
		return new Complex(re * other.re - im * other.im, re * other.im + im * other.re);
	}
	
	public Complex divide(double x) {
		return new Complex(re / x, im / x);
	}
	
	public Complex plus(Complex other) {
		return new Complex(re + other.re, im + other.im);
	}
	
	public Complex minus(Complex other) {
		return new Complex(re - other.re, im - other.im);
	}
	
	public double toDouble() {
		return re;
	}
	
	public static double[] toDouble(Complex[] c) {
		double[] d = new double[c.length];
		for (int i = 0; i < c.length; i++) {
			d[i] = c[i].toDouble();
		}
		return d;
	}
	
	public static Complex[] toComplex(double[] c) {
		Complex[] d = new Complex[c.length];
		for (int i = 0; i < c.length; i++) {
			d[i] = new Complex(c[i]);
		}
		return d;
	}
}
