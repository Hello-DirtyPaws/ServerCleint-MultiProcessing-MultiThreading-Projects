//package tictactoeserver;

import java.io.*;
import java.net.*;
import java.util.LinkedList;
import javax.swing.JOptionPane;


public class TicTacToeServer extends Thread 
{
    private static int PORT = 5000;
    private static LinkedList<Player> players = new LinkedList<Player>();

    public static void main(String[] args) 
    {
        try 
        {
            ServerSocket server = new ServerSocket(PORT);
            JOptionPane.showMessageDialog(null,"This is the TCP server.");
            String accept;
            accept = JOptionPane.showInputDialog(null,"Terminate server(y/n): ","Termination",0);
            //System.out.println("This is the TCP server.");
            int playerId = 0;
            while (!accept.equalsIgnoreCase("y")) 
            {
                //get a new client.
                Socket clientSock = server.accept();
                
                //make a player from the client and add the player to the players linked list.
                Player p = new Player(clientSock, playerId++);
                players.add(p);
                
                //start a thread to serve the player with
                Serve handleClient = new Serve(p);
                handleClient.start();
                accept = JOptionPane.showInputDialog(null,"Terminate server(y/n): ","Termination",0);
            }
            JOptionPane.showMessageDialog(null,"Server Terminating...");
            server.close();
        } 
        catch (IOException e) 
        {
            e.printStackTrace();
        }
    }
}