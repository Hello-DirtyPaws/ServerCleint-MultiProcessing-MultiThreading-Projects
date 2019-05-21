package tictactoeclient;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.net.*; 

public class TicTacToeClient extends JFrame
{

    //CONNECTION VARIABLES.
    private static final int PORT = 5000;
    private static final String HOSTNAME = "localhost";
    private static Socket socket;
    private static DataOutputStream out = null;
    private static DataInputStream in = null;
    
    //GUI VARIABLES.
    private static JButton[] buttons;
    private static JFrame frame;
    private static JPanel gameBoard, heading, tail;
    
    //OTHER VARIABLES.
    private static boolean turnRemaing;
    
    
    public static void main(String[] args)
    {
        System.out.println("This is TCP client.");
        try 
        {
            socket = new Socket(HOSTNAME, PORT);
            out = new DataOutputStream(socket.getOutputStream());
            in = new DataInputStream(socket.getInputStream());
            
            takeInitialInputs();
            loadGUI();
            playGame();
            closeAll();
            //termination();            
        } 
        catch (Exception e) 
        {
            e.printStackTrace();
        }
    }
    
    public static void takeInitialInputs() throws Exception
    {
        String line = JOptionPane.showInputDialog(null, "Play with computer(c) or player(p):", "Pick Opponent", 3);
            
        //sending a choice to the server
        out.writeUTF(line);                          

        //waiting for another or connecting with other player
        line = in.readUTF();
        JOptionPane.showMessageDialog(null, line); 

        //your move:? opponent's move:?
        line = in.readUTF();
        JOptionPane.showMessageDialog(null, line);   

    }
    
    public static void playGame() throws Exception
    {
        String line;
        do 
        {
            line = in.readUTF();
            displayGame(line);

            line = in.readUTF();    // your turn or waiting for opponent's move or final result
            changeHeading(line);

            if(line.equalsIgnoreCase("your turn.")) 
            {
                enableValidMoves();
                turnRemaing = true;
                
                while(turnRemaing)
                {
                    Thread.sleep(1);
                    //program will be blocked till the player does not take the turn.
                }                
                disableMoves();
            }                
        } while (!line.equalsIgnoreCase("you lost.") && !line.equalsIgnoreCase("you won.") && !line.equalsIgnoreCase("game tie."));
        disableMoves();        
    }
    
    public static void loadGUI()
    {
        //set up frame
        frame = new JFrame("TIC-TAC-TOE");
        frame.setLayout(new BorderLayout());
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(300, 300);
        
        
        heading = new JPanel();  
        heading.setSize(300, 100);
        frame.add(heading, BorderLayout.PAGE_START);
        
        
        //tail = new JPanel();
        //tail.setSize(300, 100);
        //setFooter();
        //frame.add(tail, BorderLayout.PAGE_END);
        
        gameBoard = new JPanel();
        gameBoard.setSize(200, 200);
        gameBoard.setLayout(new GridLayout(3, 3, 10, 10));
        setGameBtns();
        frame.add(gameBoard);

        disableMoves();
        frame.setVisible(true);
    }
    
    public static void setFooter()
    {
        
        JButton quit = new JButton("QUIT");
        quit.addActionListener(
                new ActionListener() 
                    {
                        @Override
                        public void actionPerformed(ActionEvent e) 
                        {
                            disableMoves();
                            termination();
                        }
                    });
        tail.add(quit);
    }
    
    public static void setGameBtns()
    {
        buttons = new JButton[9];
        for (int i = 0; i < 9; i++) 
        {
            buttons[i] = new JButton(" ");
            buttons[i].setFont(new Font("Ariel", Font.PLAIN, 50));
            buttons[i].setName(i + "");
            buttons[i].addActionListener(
                    new ActionListener() 
                    {
                        @Override
                        public void actionPerformed(ActionEvent e) 
                        {
                            try 
                            {
                                out.writeUTF(((JButton)e.getSource()).getName());
                            } 
                            catch (IOException ex) 
                            {
                                ex.printStackTrace();
                            }
                            turnRemaing = false;
                        }
                    });
            gameBoard.add(buttons[i]);
        }
    }
    
    public static void changeHeading(String line) 
    {
        heading.removeAll();
        JLabel headingLbl = new JLabel(line);
        heading.add(headingLbl);
        frame.repaint();
        frame.setVisible(true);
    }
        
    public static void displayGame(String game)
    {
        SwingUtilities.invokeLater(new Runnable() 
        {
            @Override
            public void run() 
            {
                frame.remove(gameBoard);
                for (int i = 0; i < 9; i++) 
                {
                    buttons[i].setText(game.charAt(i) + "");
                }
                frame.add(gameBoard, BorderLayout.CENTER);
                frame.repaint();
                frame.setVisible(true);
            }
        });
    }
    
    public static void disableMoves()
    {
        for(JButton btn: buttons)
        {
            btn.setEnabled(false);
        }
    }
    
    public static void enableValidMoves()
    {
        for(JButton btn: buttons)
        {
            if(btn.getText().compareToIgnoreCase(" ") == 0)
                btn.setEnabled(true);
        }
    }
    
    public static void termination()
    {
        JFrame endChoice = new JFrame("Decision");
        endChoice.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        endChoice.setLayout(new BoxLayout(endChoice.getContentPane(), BoxLayout.PAGE_AXIS));
        endChoice.setSize(300, 100);

        JPanel lblPanel = new JPanel();
        JLabel choiceLbl = new JLabel("Play a new game?");
        lblPanel.add(choiceLbl);

        JPanel btnPanel = new JPanel(new FlowLayout());
        
        JButton yes = new JButton("yes");
        yes.addActionListener(new ActionListener() 
                {
                        @Override
                        public void actionPerformed(ActionEvent e)
                        {                            
                            try 
                            {
                                frame.setVisible(false);
                                endChoice.setVisible(false);
                                out.writeUTF("New Game");
                                takeInitialInputs();
                                reset();
                                playGame();
                                termination();
                            }catch (Exception ex) 
                            {
                                ex.printStackTrace();
                            }
                        }
                });    
        
        JButton no = new JButton("no");
        no.addActionListener(new ActionListener() 
                {
                        @Override
                        public void actionPerformed(ActionEvent e)
                        {
                            frame.dispose();
                            endChoice.dispose();
                            try 
                            {
                                out.writeUTF("END");
                                closeAll();
                                
                            } catch (Exception ex) 
                            {
                                ex.printStackTrace();
                            }
                            
                        }
                });      
        btnPanel.add(yes);
        btnPanel.add(no);

        endChoice.add(lblPanel);
        endChoice.add(btnPanel);
        endChoice.setVisible(true);
    }
    
    public static void reset()
    {
        frame.remove(gameBoard);
        for(int i = 0; i < 9; i++)
        {
            gameBoard.remove(buttons[i]);
            buttons[i] = null;
        }
        
        setGameBtns();
        //changeHeading("New Game");
        frame.add(gameBoard);
        frame.repaint();
        frame.setVisible(true);
    }
    
    public static void closeAll() throws Exception
    {
        out.close();
        in.close();
        socket.close();
        JOptionPane.showMessageDialog(null, "Game End! Thank you for playing.", "THE END", JOptionPane.INFORMATION_MESSAGE);
        System.exit(0);
    }
}
