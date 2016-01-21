package lab12;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class SomaVetor {
	
	private static final int N = 4;
	static ArrayList<Integer> vetorA;
	static ArrayList<Integer> vetorB;

	public static ArrayList<Integer> leArquivo(String s) throws FileNotFoundException {
		File arquivo = new File(System.getProperty("user.dir") + "/src/dados/" + s);
		Scanner scan = new Scanner(arquivo).useDelimiter(" ");

		ArrayList<Integer> array = new ArrayList<Integer>();

		while (scan.hasNext()) {
			int token = scan.nextInt();
			array.add(token);
		}
		scan.close();

		return array;

	}

	public static void main(String[] args) throws FileNotFoundException, InterruptedException {

		Thread[] threads = new Thread[N];

		

		vetorA = new ArrayList<Integer>(leArquivo("vetor.txt"));
		vetorB = new ArrayList<Integer>(leArquivo("vetor.txt"));
		Vetor v = new Vetor(vetorA, vetorB,N);
		for (int i = 0; i < threads.length; i++) {
			final String m = "Ola da thread " + i;
			System.out.println(m);
			threads[i] = new Thread(new T(i,v));
		}

		for (int i = 0; i < threads.length; i++) {
			threads[i].start();
		}

		for (int i = 0; i < threads.length; i++) {
			threads[i].join();
		}
		System.out.println("Resultado = " + v.resultado);

	}

}
