import java.net.*;
import java.io.*;
import javax.script.ScriptEngineManager;
import javax.script.ScriptEngine;
import javax.script.ScriptException;

public class TCPMathServerPersistentMultiThread 
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
				
				try 
				{
					ThreadDemo t = new ThreadDemo("thrd", out, in, connectionSocket);
					t.strt();
					t.t.join();
				} 
				catch (Exception e) 
				{
					e.printStackTrace();
				}
			}
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}
}




class ThreadDemo extends Thread
{
	public Thread t;
	private DataInputStream in;
	private DataOutputStream out;
	private String threadName;
	private Socket connectionSocket;
	
	public ThreadDemo(String name, DataOutputStream out, DataInputStream in, Socket connectionSocket)
	{
		this.out = out;
		this.in = in;
		this.threadName = name;
		this.connectionSocket = connectionSocket;
	}
	
	public void run()
	{
		try 
		{
			String str = this.in.readUTF();
			while(!(str.trim()).equalsIgnoreCase("quit"))
			{
				str = eval(str.toString().trim());
				this.out.writeUTF(str);
				str = this.in.readUTF();
			}
			in.close();
			out.close();
			connectionSocket.close();
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		
		return;
	}
	
	public void strt()
	{
		if(t == null)
		{
			t = new Thread(this, threadName);
			t.start();			
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