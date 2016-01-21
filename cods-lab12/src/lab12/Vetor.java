package lab12;

import java.util.ArrayList;

public class Vetor {
	ArrayList<Integer> vetorA;
	ArrayList<Integer> vetorB;
	
	int nthreads;
	int resultado = 0;
	
	
	public Vetor(ArrayList<Integer> vetorA, ArrayList<Integer> vetorB, int nthreads){
		this.vetorA = vetorA;
		this.vetorB = vetorB;
		this.nthreads = nthreads; 
	}
	
	
	
	@Override
	public String toString() {
		return this.resultado + "";
	}

}
