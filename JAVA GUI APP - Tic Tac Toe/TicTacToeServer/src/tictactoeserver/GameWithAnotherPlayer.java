package tictactoeserver;

import java.io.IOException;

public class GameWithAnotherPlayer 
{
    Player pX, p0;
    char [] gameBoard;
        
    public GameWithAnotherPlayer(Player pX, Player p0) 
    {
        this.pX = pX;
        this.p0 = p0;
        this.gameBoard = new char[9];
        for(int i = 0; i < 9; i++)
        {
            gameBoard[i] = ' ';
        }
    }

    public void startGame() throws Exception
    {
        this.pX.out.writeUTF("Your move: X     " + p0.playerName + " move: 0");
        this.p0.out.writeUTF("Your move: 0     " + pX.playerName + " move: X");
        
        //sends initial gameboard to both players.
        pX.out.writeUTF(getGameBoard());
        p0.out.writeUTF(getGameBoard());
                
        int inputs = 0; 
        boolean flip = true;
        
        String resp;
        char respChar;
        int respLoc;
        
        do 
        {   
            if(flip)
            {
                pX.out.writeUTF("Your turn.");
                p0.out.writeUTF("Waiting for opponents move.");
                
                resp = pX.in.readUTF();
                respChar ='X';
                flip = !flip;
            }
            else
            {
                p0.out.writeUTF("Your turn.");
                pX.out.writeUTF("Waiting for opponents move.");
                
                resp = p0.in.readUTF();
                respChar = '0';
                flip = !flip;
            }
            
            respLoc = Integer.parseInt(resp.charAt(0) + "");
            
            if(gameBoard[respLoc] == ' ')
            {
                inputs++;
                gameBoard[respLoc] = respChar;
            }
            else
            {
                System.out.println("A player tried to overwirte value!");
                //ERROR! Trying to overwrite the game-board.
            }
            p0.out.writeUTF(getGameBoard());
            pX.out.writeUTF(getGameBoard());
        } while (!won(respChar) && (inputs != 9));
        
        if(inputs == 9 && !(won(respChar)))
        {
            p0.out.writeUTF("Game tie.");
            pX.out.writeUTF("Game tie.");
        }
        else if(respChar == 'X')
        {
            pX.out.writeUTF("You won.");
            p0.out.writeUTF("You lost.");
            pX.score++;
        }
        else
        {
            p0.out.writeUTF("You won.");
            pX.out.writeUTF("You lost.");
            p0.score++;
        } 
        closeConn(pX);
        closeConn(p0);
    }
    
    public void closeConn(Player p) throws IOException 
    {
        p.in.close();
        p.out.close();
        p.playerSock.close();
        
    }
    
    public String getGameBoard()
    {
        String gmStr = "";
        for(int i = 0; i < 9; i++)
        {   
            gmStr += this.gameBoard[i] + "";
        }
        return gmStr;
    }
    
    public boolean won(char c)
    {
        if(this.gameBoard[0] == c && this.gameBoard[1] == c && this.gameBoard[2] == c)
            return true;
        else if(this.gameBoard[3] == c && this.gameBoard[4] == c && this.gameBoard[5] == c)
            return true;
        else if(this.gameBoard[6] == c && this.gameBoard[7] == c && this.gameBoard[8] == c)
            return true;
        else if(this.gameBoard[0] == c && this.gameBoard[4] == c && this.gameBoard[8] == c)
            return true;
        else if(this.gameBoard[0] == c && this.gameBoard[3] == c && this.gameBoard[6] == c)
            return true;
        else if(this.gameBoard[2] == c && this.gameBoard[4] == c && this.gameBoard[6] == c)
            return true;
        else if(this.gameBoard[2] == c && this.gameBoard[5] == c && this.gameBoard[8] == c)
            return true;
        else if(this.gameBoard[1] == c && this.gameBoard[4] == c && this.gameBoard[7] == c)
            return true;
        else
            return false;
    }
}
