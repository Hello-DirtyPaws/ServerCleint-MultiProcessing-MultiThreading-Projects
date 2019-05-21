import java.net.*;
import java.io.*;
public class TCPStringServerPersistentMultithread 
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
				
				try 
				{
					ThreadDemo t = new ThreadDemo("thrd", out, in);
					t.start();
					//t.t.join();
				} 
				catch (Exception e) 
				{
					e.printStackTrace();
				}
				
				System.out.println("Thread finished");	
								
				System.out.println("Closing connection.");
				//connectionSocket.close();
				//in.close();
				//out.close();
			}
		} 
		catch (IOException e) 
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
	
	public ThreadDemo(String name, DataOutputStream out, DataInputStream in)
	{
		this.out = out;
		this.in = in;
		this.threadName = name;
	}
	
	public void run()
	{
		try 
		{
			String str = this.in.readUTF();
			while(!str.equalsIgnoreCase("quit"))
			{
				str = str.toUpperCase();
				this.out.writeUTF(str);
				str = this.in.readUTF();
			}
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
}