package tictactoeserver;

import java.util.ArrayList;

public class GameWithComp 
{
    private Player p;            // Oponent against the computer
    private char [] gameBoard;   // Gameboard
    private char[] gameX;
    
    private final char aiPlayer = 'O';
    private final char human = 'X';
    
    // Constructor that will instantiate the comp-game object
    public GameWithComp(Player p) 
    {
        this.p = p;
        this.gameX = new char[9];
        this.gameBoard = new char[9];
        for(int i = 0; i < 9; i++)
        {
            gameX[i] = ' ';
        }
        
    }
    
    //Start the game
    public void startGame() throws Exception
    {
        this.p.out.writeUTF("Connecting with computer.");
        this.p.out.writeUTF("Your move: X    Computer's move: 0");
        this.p.out.writeUTF(getGameBoard());
        int inputs = 0;
        do
        {
            p.out.writeUTF("Your Turn.");
            String resp = p.in.readUTF();

            int respLoc = Integer.parseInt(resp.charAt(0) + "");
            this.gameX[respLoc] = this.human;
            inputs++;
            
            //COMPUTER ADDS MOVE TO THE gameX gameboard.
            addMove(gameX);

            p.out.writeUTF(getGameBoard());
            
        }while(inputs != 9 && !(won(gameX, human) || won(gameX, aiPlayer)));
        
        if(won(gameX, aiPlayer))
        {
            p.out.writeUTF("You lost.");
        }
        else if(!won(gameX, human))
        {
            p.out.writeUTF("Game tie.");
        }
        
        p.in.close();
        p.out.close();
        p.playerSock.close();
    }
    
    public String getGameBoard()
    {
        String gmStr = "";
        for(int i = 0; i < 9; i++)
        {   
            gmStr += this.gameX[i] + "";
        }
        return gmStr;
    }
    
    public void addMove(char [] gb)
    {
        gameX[getComputerMove(gb)]= aiPlayer;
    }
    
    // Determine computer's move
    public int getComputerMove(char [] gb)
    {
        for(int i = 0; i < gb.length; i++)
        {
            this.gameBoard[i] = gb[i];
        }
       return minimax(this.gameBoard, this.aiPlayer);
    }
    
    // This function returns a string that hold the available empty spots
    // given a game board
    public int [] emptySpots(char [] board)
    {
        // Dummy string
        String b = "";
        
        for (int i = 0; i < board.length; i++)
            if (board[i] == ' ')
                b += "" + i;
        int []a = new int[b.length()];
        
        for (int i = 0; i < a.length; i++)
            a[i]= Integer.parseInt(b.charAt(i)+"");
        // return the empty spots array
        return a;
    }
    
    public int minimax(char [] newBoard, char player)
    {
        //Available spots
        int [] availableSpots = emptySpots(newBoard);
        
        // Checks for the terminal states such as win, lose, and tie 
        // and returning a value accordingly
        if (won(newBoard, human))
            return -10;
        else if (won(newBoard, aiPlayer))
            return 10;
        else if(availableSpots.length == 0)
            return 0;
        
        // an array to collect all the objects
        ArrayList<Move> moves = new ArrayList<Move>();

        // loop through available spots
        for (int i = 0; i < availableSpots.length; i++)
        {
            //create an object for each and store the index of that spot 
            Move move = new Move();
        	move.setIndex(availableSpots[i]);

            // set the empty spot to the current player's move
            newBoard[availableSpots[i]] = player;

            /*collect the score resulted from calling minimax 
                on the opponent of the current player*/
            if (player == aiPlayer){
                move.setScore(minimax(newBoard, human));
            }
            else{
                move.setScore(minimax(newBoard, aiPlayer));
            }

            // reset the spot to empty
            newBoard[availableSpots[i]] = ' ';

            // push the object to the array
            moves.add(move);
        }
        
        // if it is the computer's turn loop over the moves and choose the move with the highest score
        int bestMove = -1;
        if(player == aiPlayer)
        {
            int bestScore = -10000;
            for(int i = 0; i < moves.size(); i++)
            {
                if((moves.get(i).getScore()) > bestScore)
                {
                    bestScore = moves.get(i).getScore();
                    bestMove = i;
                }
            }
        }
        else
        {
        // else loop over the moves and choose the move with the lowest score
            int bestScore = 10000;
            for(int i = 0; i < moves.size(); i++)
            {
                if((moves.get(i).getScore()) < bestScore)
                {
                    bestScore = (moves.get(i).getScore());
                    bestMove = i;
                }
            }
        }

        // return the chosen move (object) from the moves array
        return (moves.get(bestMove)).getIndex();
    }
    
    // Checks the winning conditions
    public boolean won(char [] newBoard, char c)
    {
        if( newBoard[0] == c &&  newBoard[1] == c &&  newBoard[2] == c)
            return true;
        else if( newBoard[3] == c &&  newBoard[4] == c &&  newBoard[5] == c)
            return true;
        else if( newBoard[6] == c &&  newBoard[7] == c &&  newBoard[8] == c)
            return true;
        else if( newBoard[0] == c &&  newBoard[4] == c &&  newBoard[8] == c)
            return true;
        else if( newBoard[0] == c &&  newBoard[3] == c &&  newBoard[6] == c)
            return true;
        else if( newBoard[2] == c &&  newBoard[4] == c &&  newBoard[6] == c)
            return true;
        else if( newBoard[2] == c &&  newBoard[5] == c &&  newBoard[8] == c)
            return true;
        else if( newBoard[1] == c &&  newBoard[4] == c &&  newBoard[7] == c)
            return true;
        else
            return false;
    }
    
    // Helper Move class
    public class Move
    {
        private int index;
        private int score;
        
        public void setIndex(int i){
            index = i;
        }
        
        public void setScore(int s){
            score = s;
        }
        
        public int getIndex(){
            return index;
        }
        public int getScore(){
            return score;
        }
    }
}
