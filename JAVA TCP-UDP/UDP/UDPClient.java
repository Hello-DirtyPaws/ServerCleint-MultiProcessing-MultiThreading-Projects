import java.io.*;
import java.net.*;
import java.util.Scanner;
public class UDPClient
{
	private final static int PORT = 5000;
	private static final String HOSTNAME = "localhost";
	public static void main(String[] args) 
	{
		System.out.println("This is the UDP Client.");
		String requestString = "";
		
		while(!requestString.equalsIgnoreCase("quit"))
		{
			try 
			{
				DatagramSocket socket = new DatagramSocket(0);
				Scanner scanner = new Scanner(System.in);
				
				System.out.print("Enter the input: ");
				requestString = scanner.nextLine();
				
				byte[] requestBuffer = requestString.getBytes();
				InetAddress host = InetAddress.getByName(HOSTNAME);
				DatagramPacket request = new DatagramPacket(requestBuffer, requestBuffer.length,host,PORT);
				socket.send(request);
				DatagramPacket response = new DatagramPacket(new byte[1024], 1024);
				socket.receive(response);
				String result = new String(response.getData());
				if(!requestString.equalsIgnoreCase("quit"))
				{
					System.out.println("Server Response: " + result);
				}
				
			} 
			catch (IOException e) 
			{
				e.printStackTrace();
			}
	
		}		
	}
}