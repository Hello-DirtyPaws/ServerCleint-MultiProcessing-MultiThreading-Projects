import java.io.*;
import java.net.*;
public class UDPServer
{
	private final static int PORT = 5000;
	public static void main(String[] args) 
	{
		System.out.println("This is the UDP Server.");
		try 
		{
			DatagramSocket socket = new DatagramSocket(PORT);
			while(true)
			{
				DatagramPacket request = new DatagramPacket(new byte[1024], 1024);
				socket.receive(request);
				byte[] requestBuffer = request.getData();
				String requestString = new String(requestBuffer);
				
				String responseString = requestString.toUpperCase();
				byte[] responseBuffer = responseString.getBytes();
				DatagramPacket response = new DatagramPacket(responseBuffer, responseBuffer.length, request.getAddress(), request.getPort());
				socket.send(response);
			}
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
}