package lab12;

public class T implements Runnable {

	int pid;

	// Objeto compartilhado.
	Vetor v;

	String s = "Sou a thread";

	public T(int p, Vetor v) {
		this.pid = p;
		this.v = v;
	}

	@Override
	public void run() {

		System.out.println(s + pid);
		for (int i = pid; i < v.vetorA.size(); i+= v.nthreads) {
			soma(v.vetorA.get(i),v.vetorB.get(i));
		}
	}

	public synchronized void soma(int a , int b) {
		v.resultado += a + b;
	}		

}
