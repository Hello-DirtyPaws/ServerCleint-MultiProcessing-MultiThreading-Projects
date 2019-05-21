import java.net.*;
import java.io.*;
import javax.script.ScriptEngineManager;
import javax.script.ScriptEngine;
import javax.script.ScriptException;

public class TCPMathServerNonPersistent 
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
				DataInputStream in = new DataInputStream(connectionSocket.getInputStream());
				DataOutputStream out = new DataOutputStream(connectionSocket.getOutputStream());
				String requestString = in.readUTF();
				
				if(!requestString.equalsIgnoreCase("quit"))
				{
					String responseString = eval(requestString.toString().trim());
					out.writeUTF(responseString);
					out.flush();
				}
				
				connectionSocket.close();
				in.close();
				out.close();
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