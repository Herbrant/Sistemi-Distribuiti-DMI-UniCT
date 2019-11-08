import java.io.*;
import java.net.*;


public class Repeat{
    public static final int PORT = 1050;

    public static void main(String[] args) throws IOException{
        ServerSocket serverSocket = new ServerSocket(PORT);
        System.out.println("RepeatServer: started");
        System.out.println("Server socket: " + serverSocket);
        Socket clientSocket = null;
        BufferedReader in = null;
        PrintWriter out = null;

        try{
            clientSocket = serverSocket.accept();
            System.out.println("Client " + clientSocket + " connected");

            InputStreamReader isr = new InputStreamReader(clientSocket.getInputStream());
            in = new BufferedReader(isr);

            OutputStreamWriter osw = new OutputStreamWriter(clientSocket.getOutputStream());
            BufferedWriter bw = new BufferedWriter(osw);
            out = new PrintWriter(bw, true);

            while(true){
                String str = in.readLine();
                if(str.equals("END"))
                    break;
                System.out.println("Client requests: " + str);
                out.println(str);
            }
        }
        catch(IOException e){
            System.err.println("Accept failed");
            System.exit(-1);
        }

        System.out.println("RepeatServer: closing");
        out.close();
        in.close();
        clientSocket.close();
        serverSocket.close();
    }
}