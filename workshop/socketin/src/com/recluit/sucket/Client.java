package com.recluit.sucket;

import java.net.*;
import java.io.*;

public class Client{
    public static void main(String... nombre) throws IOException {
        final String IP = "0.0.0.0";
        final int PORT = 3550;

        Socket sock = null;
        InputStreamReader isr = null;
        BufferedReader br = null;
        DataOutputStream dos = null;
        OutputStream out = null;
        try{
            sock = new Socket(IP,PORT);
            isr =new InputStreamReader(sock.getInputStream());
            dos = new DataOutputStream(sock.getOutputStream());
            br = new BufferedReader(isr);

            out = sock.getOutputStream();
            String line = "salida\n";
            char[] strArray;
            strArray = line.toCharArray();
            while (true) {
                for( int index = 0; index < strArray.length; index++){
                    out.write(strArray[index]);
                }
                out.flush();
                System.out.println("data sent " );
                System.out.println("Datos Leidos del SERVER: \n"+br.readLine());
                System.out.println("\n");
            }
        }catch (IOException ioe) {
            System.err.println(ioe);
        }finally {
            sock.close();
            br.close();
            isr.close();
        }
    }
}
