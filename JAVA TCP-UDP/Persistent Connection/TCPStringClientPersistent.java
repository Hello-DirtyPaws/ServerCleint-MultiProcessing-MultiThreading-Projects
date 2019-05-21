import java.net.*;
import java.io.*;
import java.util.*;
public class TCPStringClientPersistent 
{
	private static final int PORT = 5000;
	private static final String HOSTNAME = "localhost";
	private static DataOutputStream out = null;
	private static DataInputStream in = null;
	
	public static void main(String[] args) 
	{
		System.out.println("This is TCP client.");
		String line = "";
		Scanner scanner = new Scanner(System.in);
		
		Socket socket = null;
		try {
				socket = new Socket(HOSTNAME, PORT);
				out = new DataOutputStream(socket.getOutputStream());
				in = new DataInputStream(socket.getInputStream());				
				while (!line.equalsIgnoreCase("quit")) 
				{
					line = scanner.nextLine();
					out.writeUTF(line);
					out.flush();
					
					if(!line.equalsIgnoreCase("quit"))
					{
						String response = in.readUTF();
						System.out.println(response);
					}
				}
				out.close();
				in.close();
				socket.close();
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
	}
}