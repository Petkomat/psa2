package strukture;


import java.io.*;
import java.util.*;


public class Trie {
    private final HashMap<Character, Integer> abeceda;
    private final Vozliesce koren;

    /**
     * Konstruktor za ta razred.
     */
    public Trie(String znaki){
        this.abeceda = new HashMap<>();
        for (int i = 0; i < znaki.length(); i++){
            char x = znaki.charAt(i);
            abeceda.put(x, i);
        }
        this.koren = new Vozliesce(abeceda.size());
    }

    public void vstavi(String beseda){
        Vozliesce trenutno = koren;
        // poskrbimo za nadaljevanja
        for (int i = 0; i < beseda.length(); i++){
            char znak = beseda.charAt(i);
            if (trenutno.nasledniki[abeceda.get(znak)] == null){
                trenutno.nasledniki[abeceda.get(znak)] = new Vozliesce(abeceda.size());
            }
            trenutno = trenutno.nasledniki[abeceda.get(znak)];
        }
        // na koncu označimo, da je tu tudi konec besede
        trenutno.jeBeseda = true;
    }

    public void vstaviIzDatoteke(String pot){
        String beseda;
        try (BufferedReader bralnik = new BufferedReader(new InputStreamReader(new FileInputStream(pot), "UTF-8"))) {
            beseda = bralnik.readLine();
            while (beseda != null){
                beseda = beseda.trim().toLowerCase();
                this.vstavi(beseda);
                beseda = bralnik.readLine();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public int steviloVozlisc(){
        Stack<Vozliesce> zaObdelati = new Stack<>();
        zaObdelati.push(koren);
        int stevilo = 0;
        while (!zaObdelati.isEmpty()){
            Vozliesce trenutno = zaObdelati.pop();
            stevilo++;
            for (Vozliesce naslednik : trenutno.nasledniki){
                if (naslednik != null){
                    zaObdelati.push(naslednik);
                }
            }
        }
        return stevilo;
    }

    public List<String> nastejBesedeUrejeno(){
        // ne vemo, koliko jih bo, zato String[] ni ok
        List<String> vsebuje = new ArrayList<>();
        // za nastevanje po vrsti
        HashMap<Integer, Character> polozaj2crka = new HashMap<>();
        for (Map.Entry<Character, Integer> par : abeceda.entrySet()){
            char crka = par.getKey();
            int polozaj = par.getValue();
            polozaj2crka.put(polozaj, crka);
        }

        Stack<Vozliesce> zaObdelati = new Stack<>();
        Stack<String> delneBesede = new Stack<>();

        zaObdelati.push(koren);
        delneBesede.push("");
        while (!zaObdelati.isEmpty()){
            Vozliesce trenutno = zaObdelati.pop();
            String pripadajocaBeseda = delneBesede.pop();
            if (trenutno.jeBeseda){
                vsebuje.add(pripadajocaBeseda);
            }
            // najprej damo na sklad ž, da pride na vrsto na koncu
            for (int i = abeceda.size() - 1; i >= 0; i--){
                char znak = polozaj2crka.get(i);
                Vozliesce naslednik = trenutno.nasledniki[i];
                if (naslednik != null){
                    zaObdelati.push(naslednik);
                    delneBesede.push(pripadajocaBeseda + znak);
                }
            }
        }
        return vsebuje;
    }
}
