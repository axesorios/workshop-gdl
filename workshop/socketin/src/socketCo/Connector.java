package socketCo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Connector {
	
	static BufferedReader readInput;
	public static String padRight(String s, int n) {
	return String.format("%1$-" + n + "s", s);
	}

	public void establishConnection(String mensaje) throws IOException {
	final String IP = "localhost";
	final Integer PORT = 3550;
	Socket s = new Socket(IP, PORT);
	InputStreamReader stream = new InputStreamReader(s.getInputStream());
	BufferedReader reader = new BufferedReader(stream);
	PrintWriter writer = new PrintWriter(s.getOutputStream());
	writer.print(mensaje);
	writer.flush();

	String str = null;
	while ((str = reader.readLine()) != null) {
	System.out.println(str + "\n");
	}
	writer.close();
	reader.close();
	s.close();
	}
	public static void main(String... eduardo) throws IOException {
	String mensaje = "";
	readInput = new BufferedReader(new InputStreamReader(System.in));

	String str2 = String.format("%-127s", readInput.readLine()).replace(' ', ' ');
	System.out.println("Mensaje Enviado como parametro 'RFC': " + str2.length()
	+ "\n");
	new Connector().establishConnection(str2);
	}
	

}
