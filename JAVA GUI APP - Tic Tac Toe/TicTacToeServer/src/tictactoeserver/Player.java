package tictactoeserver;

import java.io.*;
import java.net.*;

public class Player 
{
    Socket playerSock;
    DataInputStream in;
    DataOutputStream out;
    
    int playerId;
    String playerName;
    int score;

    public Player(Socket playerSock, int playerId) throws IOException 
    {
        this.playerSock = playerSock;
        this.playerId = playerId;
        this.in = new DataInputStream(this.playerSock.getInputStream());
        this.out = new DataOutputStream(this.playerSock.getOutputStream());
        this.score = 0;
        this.playerName = "Player" + this.playerId;
    }
}
