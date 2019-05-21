import java.net.*;
import java.io.*;
import javax.script.ScriptEngineManager;
import javax.script.ScriptEngine;
import javax.script.ScriptException;

public class TCPMathServerPersistent 
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
				String line = in.readUTF();
				
				while(!line.equalsIgnoreCase("quit"))
				{
					String newLine = eval(line.toString().trim());
					out.writeUTF(newLine);
					out.flush();
					line = in.readUTF();
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