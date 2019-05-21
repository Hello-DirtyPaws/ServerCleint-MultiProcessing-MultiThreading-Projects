import java.net.*;
import java.io.*;
public class TCPServer 
{
	private static int PORT = 5000;
	public static void main(String[] args) 
	{
		try 
		{
			ServerSocket server = new ServerSocket(PORT);
			System.out.println("This is the TCP server.");
			while(true)
			{
				Socket connectionSocket = server.accept();
				System.out.println("Client accepted.");
				DataInputStream in = new DataInputStream(connectionSocket.getInputStream());
				DataOutputStream out = new DataOutputStream(connectionSocket.getOutputStream());
				String line = in.readUTF();
				String newLine = line.toUpperCase();
				out.writeUTF(newLine);
				out.flush();
				System.out.println("Closing connection.");
				connectionSocket.close();
				in.close();
				out.close();
			}
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
}