package vaje04;

/**
 * Implementacija trojice.
 * 
 * @author matejp
 *
 * @param <U>
 * @param <V>
 * @param <W>
 */
public class Trojka<U, V, W> {
	private U prvi;
	private V drugi;
	private W tretji;
	
	public Trojka(U p, V d, W t) {
		this.prvi = p;
		this.drugi = d;
		this.tretji = t;
	}

	public U getPrvi() {
		return prvi;
	}

	public V getDrugi() {
		return drugi;
	}
	
	public W getTretji() {
		return tretji;
	}
}
