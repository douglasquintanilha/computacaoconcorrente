package lab12;

public class T implements Runnable{
	
	int pid;
	String s = "Sou a thread";
	
	public T(int p){
		this.pid = p;
	}
	
	@Override
	public void run() {
		System.out.println(s+pid);
	}

}
