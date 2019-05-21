import java.net.*;
import java.io.*;
import java.util.*;
public class TCPClient 
{
	private static final int PORT = 5000;
	private static final String HOSTNAME = "localhost";
	
	public static void main(String[] args) 
	{
		System.out.println("This is TCP client.");
		String line = "";
		Scanner scanner = new Scanner(System.in);
		
		while (!line.equalsIgnoreCase("quit")) 
		{
			try {
					Socket socket = null;
					try {
							socket = new Socket(HOSTNAME, PORT);
						}
					catch(UnknownHostException e)
					{
						e.printStackTrace();
					} 
					System.out.println("Connected.");
					DataOutputStream out = new DataOutputStream(socket.getOutputStream());
					DataInputStream in = new DataInputStream(socket.getInputStream());
					
					line = scanner.nextLine();
					
					out.writeUTF(line);
					out.flush();
					String response = in.readUTF();
					
					if(!line.equalsIgnoreCase("quit"))
					{
						System.out.println(response);
					}
					
					out.close();
					in.close();
					socket.close();
			} 
			catch (IOException e) 
			{
				e.printStackTrace();
			}
		}
		
	}
}