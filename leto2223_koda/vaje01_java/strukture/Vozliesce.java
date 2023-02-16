package strukture;

public class Vozliesce {
    public boolean jeBeseda;
    public Vozliesce[] nasledniki;

    public Vozliesce(int velikostAbecede) {
        nasledniki = new Vozliesce[velikostAbecede];
    }
}
