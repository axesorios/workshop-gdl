package com.recluit.sucket;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;


public class Client2{

	public void establishConnection(){
		try{
			Socket s = new Socket("0.0.0.0", 3550);
			InputStreamReader stream = new InputStreamReader(s.getInputStream());
			BufferedReader reader = new BufferedReader(stream);
			String str = reader.readLine();
			System.out.println(" text received from client ;" + str);
			reader.close();
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	
	public static void main(String[] args){
		new Client2().establishConnection();
	}
}
