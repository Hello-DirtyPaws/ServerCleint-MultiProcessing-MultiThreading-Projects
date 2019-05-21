import java.io.*;
import java.net.*;
import javax.script.ScriptEngineManager;
import javax.script.ScriptEngine;
import javax.script.ScriptException;

public class UDPMathServer
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
				//receiving msg
				DatagramPacket request = new DatagramPacket(new byte[1024], 1024);
				socket.receive(request);
				byte[] requestBuffer = request.getData();
				String requestString = new String(requestBuffer);
				
				//eval msg
				String responseString = eval(requestString.toString().trim());
				
				//sending msg
				byte[] responseBuffer = responseString.getBytes();
				DatagramPacket response = new DatagramPacket(responseBuffer, responseBuffer.length, request.getAddress(), request.getPort());
				socket.send(response);
			}
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}
	
	public static String eval(String expr) throws Exception
	{
		if(expr.length() != 0)
		{
			ScriptEngineManager mgr = new ScriptEngineManager();
			ScriptEngine engine = mgr.getEngineByName("JavaScript");
			return engine.eval(expr).toString();
		}
		else 
		{
			return "";
		}
		
	}
}