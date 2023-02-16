package main;


import strukture.Trie;

import java.util.List;

public class Main {
    public static void main(String[] args){
        String sloAbeceda = "abcčdefghijklmnoprsštuvzž";
        Trie d = new Trie(sloAbeceda);

        String[] sskj = new String[]{"a", "aa", "aaa", "ab", "abc", "aaaa"};
        for (String beseda : sskj){
            d.vstavi(beseda);
        }
        System.out.println("Število vozlišč: " + d.steviloVozlisc());
        System.out.println("Besede: " + d.nastejBesedeUrejeno());
        d = new Trie(sloAbeceda);
        d.vstaviIzDatoteke("C:/Users/matejp/git/psa2/leto2223/vaje01/malo.txt");

        List<String> urejeneBesede = d.nastejBesedeUrejeno();
        System.out.println("Besede: " + d.nastejBesedeUrejeno());
        System.out.println("|SSKJ| = " + urejeneBesede.size());
        System.out.println("Stevilo vozlisc: " + d.steviloVozlisc());
    }
}
