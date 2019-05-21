package tictactoeserver;

import java.io.*;
import java.net.*;
import java.util.LinkedList;


public class TicTacToeServer extends Thread 
{
    private static int PORT = 5000;
    private static LinkedList<Player> players = new LinkedList<Player>();

    public static void main(String[] args) 
    {
        try 
        {
            ServerSocket server = new ServerSocket(PORT);
            System.out.println("This is the TCP server.");
            int playerId = 0;
            while (true) 
            {
                //get a new client.
                Socket clientSock = server.accept();
                
                //make a player from the client and add the player to the players linked list.
                Player p = new Player(clientSock, playerId++);
                players.add(p);
                
                //start a thread to serve the player with
                Serve handleClient = new Serve(p);
                handleClient.start();
            }
        } 
        catch (IOException e) 
        {
            e.printStackTrace();
        }
    }
}