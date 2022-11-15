import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.List;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.UIManager;

public class PrivateChatRoomClient implements ActionListener {
	
	// INSTANCE variables (allocated in the "program object" in Dynamic Storage)
	Socket             s;	
	ObjectOutputStream oos;
	ObjectInputStream  ois;
	String  newLine        = System.lineSeparator(); 
	int     fontSize       = 20;
	int     maxFontSize    = 50;
	int     minFontSize    = 5;
	
	// GUI Objects
	JFrame      chatWindow          = new JFrame(); // like int x = 5; to provide an inital value to the pointer field
	JPanel      topPanel            = new JPanel();
	JPanel      middlePanel         = new JPanel();
	JPanel      bottomPanel         = new JPanel();
	JLabel      sendMsgLabel        = new JLabel("Enter a message here and push a SEND button.");
	JLabel      whosInLabel         = new JLabel("Who's IN the chat room (CAPS) and who's NOT (lower case)");
	JLabel      receivedMsgsLabel   = new JLabel("Received messages (including the ones you sent)");
	JButton     sendPrivateButton   = new JButton("Send PRIVATE msg only to selected recipients");
	JButton     sendPublicButton    = new JButton("Send PUBLIC msg To Everyone In + selected outs");
	JTextField  errMsgTextField     = new JTextField("Error messages will show here.");
	JTextArea   sendChatArea        = new JTextArea();
	JList<String> whosInList        = new JList<String>(); // note JList is declared to hold String objects (chat names)
	JTextArea   receiveChatArea     = new JTextArea();
	JScrollPane sendScrollPane      = new JScrollPane(sendChatArea);
	JScrollPane receiveScrollPane   = new JScrollPane(receiveChatArea); 
	JScrollPane whosInScrollPane    = new JScrollPane(whosInList); 
	 
	//Window Menu items
	MenuBar  menuBar  = new MenuBar();
	Menu     fontMenu = new Menu("Font");
	MenuItem biggerFontMenuItem  = new MenuItem("Bigger");
	MenuItem smallerFontMenuItem = new MenuItem("Smaller");
	
	public static void main(String[] args) {
		if(args.length == 3) {
			try {
			    PrivateChatRoomClient crc = new PrivateChatRoomClient(args[0], args[1], args[2]); // "new" calls the ObjectLoader
			    crc.receive(); // branch the main thread into the program object's receive() method to
			    				// receive & display chat messages received from the server (other clients).
			} catch(Exception e) {   
				System.out.println(e); // print the exception object as the error message
				return; // can't continue if can't load the program!
			}       	// (OR we have encountered an Exception during the receive() process.)
		} else {
			System.out.println("Restart program and input three command line parameters");
			System.out.println("(The host address, the client name, and the password)");
			return;
		}
	}
	
	public PrivateChatRoomClient(String serverAddress, String clientName, String password) throws Exception {
		System.out.println("Joshua Swaim");
		if (serverAddress.contains(" ") || clientName.contains(" ") || password.contains(" ")) {
		    throw new IllegalArgumentException("Parameters may not contain blanks."); // also returns.
		}
		
		System.out.println("Connecting to the chat room server at " + serverAddress + " on port 4444.");
		try {
			s = new Socket(serverAddress,4444); // wait for server to accept connection.
		} catch(Exception e) {
			System.out.println("Connection to Chat Room at " + serverAddress + " on port 4444 has failed.");
		    System.out.println("Is server address correct? Has the server been started on port 4444?"); 
		    return; // terminate so user can fix & restart.
		}
		System.out.println("Connected to the chat server!");
		oos = new ObjectOutputStream(s.getOutputStream()); // oos is declared as an instance variable.
		oos.writeObject(clientName + " " + password);        // send the "join message" (a String)
		ois = new ObjectInputStream(s.getInputStream());   // ois is declared as an instance variable.
		String reply = (String) ois.readObject();          // wait for server response (note cast of receive type to String)
		
		if (reply.startsWith("Welcome")) {
			System.out.println("Join was successful!");
		} else {
			throw new IllegalArgumentException("Join of " + clientName + " with password " + password + " was not successful. Password is incorrect.");
		}
		
		topPanel.setLayout(new GridLayout(1,3)); // a format with 1 row and 3 columns
		topPanel.add(sendMsgLabel);              // goes in row 1 column 1
		topPanel.add(whosInLabel);               // goes in row 1 column 2
		topPanel.add(receivedMsgsLabel);         // goes in row 1 column 3
		chatWindow.getContentPane().add(topPanel,"North");

		middlePanel.setLayout(new GridLayout(1,3)); 
		middlePanel.add(sendScrollPane);
		middlePanel.add(whosInScrollPane);
		middlePanel.add(receiveScrollPane);
		chatWindow.getContentPane().add(middlePanel,"Center");

		bottomPanel.setLayout(new GridLayout(1,3)); 
		bottomPanel.add(sendPublicButton);
		bottomPanel.add(sendPrivateButton); // get ready for "private" client (no yet activated)
		bottomPanel.add(errMsgTextField);
		chatWindow.getContentPane().add(bottomPanel,"South");
		
		chatWindow.setTitle(clientName + "'s CHAT ROOM    (Close this window to leave the chat room.)");
		sendPublicButton.setBackground(Color.green);
		sendPrivateButton.setBackground(Color.yellow);
		
		receiveChatArea.setLineWrap(true);     // cause long text added to be properly
		receiveChatArea.setWrapStyleWord(true);// "wrapped" to the next line.
		sendChatArea.setLineWrap(true);
		sendChatArea.setWrapStyleWord(true);
		
		receiveChatArea.setEditable(false); // keep user from changing the output area!
		errMsgTextField.setEditable(false); // keep user from changing the error message area!
		
		sendPublicButton.addActionListener(this); // sendPublicButton will now call us when the user pushes it!                                      
        										// "this" is our program's address in memory (different every run)
		sendPrivateButton.addActionListener(this);
		
		UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
		
		chatWindow.setMenuBar(menuBar);
	    menuBar.add(fontMenu);
	    fontMenu.add(biggerFontMenuItem);
	    fontMenu.add(smallerFontMenuItem);
	    biggerFontMenuItem.addActionListener(this); // so these FontMenuItems (buttons!) 
	    smallerFontMenuItem.addActionListener(this);// in the font menu can call us!
	    
	    chatWindow.setSize(800,500);    // width,height
	    chatWindow.setLocation(400,0);  // x,y (x is "to the right of the left margin", y is "down-from-the-top")
	    chatWindow.setVisible(true);    // show it
	    chatWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // terminate if user closes window
	
	    Vector<Object> savedMessages = (Vector<Object>) ois.readObject();
		for(Object i : savedMessages) {
			if(i instanceof String) {
				receiveChatArea.append(newLine + i);  // show text on GUI 
				receiveChatArea.setCaretPosition(receiveChatArea.getDocument().getLength()); // scroll to bottom
			}
		}
	
	}

	public void actionPerformed(ActionEvent ae) {
		if(whosInList.getValueIsAdjusting()) return;
		
		errMsgTextField.setText("");                // clear any error message
		errMsgTextField.setBackground(Color.white); // and remove highlight
		
		if (ae.getSource() == sendPublicButton) { // is the sendPublicButton is calling us?
			String chatMessageToSend = sendChatArea.getText().trim(); // remove leading/trailing blanks.
			if (chatMessageToSend.length() == 0) { // a blank message!
				errMsgTextField.setText("No message entered to send.");
				errMsgTextField.setBackground(Color.pink); // highlight to get attention
		        return; // return button's thread to the button.
			} else if(whosInList.isSelectionEmpty()) {
				try {
					oos.writeObject(chatMessageToSend);
					sendChatArea.setText(""); // clear input area
				} catch(Exception e) { // Uh-oh - the network or the server is down!
					// Take no action.
					// If the connection to the server is down, the readObject() method in the ois has
					// no-doubt already seen it and the catch block in receive() is telling the user they
					// must restart the client to re-establish connection with the chat server. 
				} 
		        return; // return button's thread to the button.
			} else {
				try {
					List<String> selectedClientNames = whosInList.getSelectedValuesList();
					selectedClientNames.add(0, chatMessageToSend);
					oos.writeObject(selectedClientNames);
					sendChatArea.setText(""); // clear input area
				} catch(Exception e) { // Uh-oh - the network or the server is down!
			      // Take no action.
			      // If the connection to the server is down, the readObject() method in the ois has
			      // no-doubt already seen it and the catch block in receive() is telling the user they
			      // must restart the client to re-establish connection with the chat server. 
				}
				whosInList.clearSelection();
				return; // return the button's thread to the JButton program
			} // end of processing block for the send button
		} else if (ae.getSource() == sendPrivateButton) { // is the sendPrivateButton is calling us?
			String chatMessageToSend = sendChatArea.getText().trim(); // remove leading/trailing blanks.
			if (chatMessageToSend.length() == 0) { // a blank message!
				errMsgTextField.setText("No message entered to send.");
				errMsgTextField.setBackground(Color.pink); // highlight to get attention
		        return; // return button's thread to the button.
			} else if(whosInList.isSelectionEmpty()) {
				errMsgTextField.setText("No recipients selected for a PRIVATE message.");
				errMsgTextField.setBackground(Color.pink); // highlight to get attention
		        return; // return button's thread to the button.
			} else {
				chatMessageToSend = "PRIVATE " + chatMessageToSend;
				try {
					List<String> selectedClientNames = whosInList.getSelectedValuesList();
					selectedClientNames.add(0, chatMessageToSend);
					oos.writeObject(selectedClientNames);
					sendChatArea.setText(""); // clear input area
				} catch(Exception e) { // Uh-oh - the network or the server is down!
			      // Take no action.
			      // If the connection to the server is down, the readObject() method in the ois has
			      // no-doubt already seen it and the catch block in receive() is telling the user they
			      // must restart the client to re-establish connection with the chat server. 
				}
				whosInList.clearSelection();
				return; // return the button's thread to the JButton program
			} // end of processing block for the send button
		} else if (ae.getSource() == biggerFontMenuItem) { // This button increases the font size in the in and out chatTextAreas
			if (fontSize < maxFontSize) {
				fontSize += 1;
				sendChatArea.setFont(new Font("default", Font.BOLD, fontSize));
				receiveChatArea.setFont(new Font("default", Font.BOLD,fontSize));
			}
			return;
		} else if (ae.getSource() == smallerFontMenuItem) { // This button reduces the font size in the in and out chatTextAreas
		   if (fontSize > minFontSize) {
			   fontSize -= 1;
			   sendChatArea.setFont(new Font("default", Font.BOLD, fontSize)); 
			   receiveChatArea.setFont(new Font("default", Font.BOLD,fontSize));
		   }
		   return;   
		}
	}
	
	public void receive() {
		try {
			while(true) { // capture entering main thread in a do-forever read loop
				Object incomingMessage = ois.readObject(); // WAIT for a message from the server
				if (incomingMessage instanceof String) {
					String receivedChatMessage = (String) incomingMessage; // Copy and CAST pointer 
					// The received object is now also pointed to by the pointer receivedChatMessage which is type String 
					// Pass the String pointer to methods that expect to receive a String as a parameter, or
					// call String methods on the received object using the String pointer.
					receiveChatArea.append(newLine + receivedChatMessage);  // show text on GUI 
					// auto-scroll the JScrollPane to bottom line so the last message will be visible.
					receiveChatArea.setCaretPosition(receiveChatArea.getDocument().getLength()); // scroll to bottom
				} else if (incomingMessage instanceof String[]) { // an array of Strings
					String[] listOfWhosIn = (String[]) incomingMessage; // Copy and CAST pointer 
					whosInList.setListData(listOfWhosIn); // give JList the array of Strings to display on GUI.
				} else if (incomingMessage instanceof ImageIcon) { // a picture! 
					ImageIcon receivedPicture = (ImageIcon) incomingMessage; // Copy and CAST pointer 
					System.out.println("A picture was received from the server. " + receivedPicture);
					System.out.println("Pictures are not processed by the lab 4 PrivateChatRoomClient.");
				} else { // Ignore unexpected object type:
					System.out.println("Unexpected object type received from server: " + incomingMessage);  
				}
			}
		} catch(Exception e) { // readObject() 1st failure also exits the read loop.
			// show error message to user
			errMsgTextField.setBackground(Color.pink); // this will get their attention!
			errMsgTextField.setText("CHAT ROOM SERVER CONNECTION HAS FAILED!");
			receiveChatArea.append(newLine + "You must close this chatWindow and then restart the client to reconnect to the server to continue.");
			receiveChatArea.setCaretPosition(receiveChatArea.getDocument().getLength()); // scroll down
			// disable the GUI function
			sendChatArea.setEditable(false); // keep user from trying to send any more messages.
			sendPublicButton.setEnabled(false);    // stop button pushing
			sendPrivateButton.setEnabled(false);    // stop button pushing
		}
	}
}