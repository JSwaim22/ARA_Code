import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

public class PrivateChatRoomServer implements Runnable, ActionListener {
	
	// Maps
	ConcurrentHashMap<String,ObjectOutputStream> whosIn = new ConcurrentHashMap<String,ObjectOutputStream>();
	ConcurrentHashMap<String,String> passwords = new ConcurrentHashMap<String,String>();
	ConcurrentHashMap<String, Vector<Object>>  savedMessages = new ConcurrentHashMap<String, Vector<Object>>();
	
	// Socket
	private ServerSocket ss;
	
	// Main
	public static void main(String[] args) throws Exception {
		System.out.println("Joshua Swaim");
		if(args.length > 0) {
			System.out.println("Parameters have been ignored.");
		}
		new PrivateChatRoomServer();
	}
	
	// PrivateChatRoomServer
	public PrivateChatRoomServer() throws Exception {
		
		// Booting up Server
		try {
			ss = new ServerSocket(4444);
		} catch(Exception e) {
			throw new IllegalArgumentException("Port 4444 not available"); // also returns.
		}
		System.out.println("PrivateChatRoomServer is up at "
				+ InetAddress.getLocalHost().getHostAddress()
                + " on port " + ss.getLocalPort());
		
		// Getting passwords from file
		try {
			FileInputStream   fis = new FileInputStream("passwords.ser");
			ObjectInputStream ois = new ObjectInputStream(fis);
			passwords = (ConcurrentHashMap<String,String>) ois.readObject();	//cast type of object found
			ois.close();														//from Object to collection type
			System.out.println("Previously in the chat room: ");	// Printing everyone's chat name and password
			System.out.println(passwords);                      	// on the console can be very handy in testing.
		} catch(FileNotFoundException fnfe) {
			System.out.println("passwords.ser is not found, so an empty collection will be used.");
		}
		
		// Getting saved messages from file
		try {
			FileInputStream   fis = new FileInputStream("SavedChatMessages.ser");
			ObjectInputStream ois = new ObjectInputStream(fis);
			savedMessages = (ConcurrentHashMap<String,Vector<Object>>) ois.readObject();	//cast type of object found
			ois.close();														//from Object to collection type
			System.out.println("Previously sent messages: ");	// Printing everyone's chat name and password
			System.out.println(savedMessages);                      	// on the console can be very handy in testing.
		} catch(FileNotFoundException fnfe) {
			System.out.println("SavedChatMessages.ser is not found, so an empty collection will be used.");
		}
		
		// Branching into run() method.
		new Thread(this).start();
	}
	
	// Action Performed
	public void actionPerformed(ActionEvent e) {

	}
	
	// Run
	public void run() { // client threads enter here 
		
		// Variables
		Socket             s                = null;
		ObjectInputStream  ois              = null;
		ObjectOutputStream oos              = null;
		ObjectOutputStream previousOOS      = null; // used on "rejoin"
		String             joinMessage      = null;
		String             chatName         = null;
		String             providedPassword = null;
		String             storedPassword   = null;
		String             clientAddress    = null;
		
		// Connection Attempt
		try {
			s = ss.accept(); // wait for next client to connect
			clientAddress = s.getInetAddress().getHostAddress();
			System.out.println("New client connecting from " + clientAddress);
			ois = new ObjectInputStream(s.getInputStream());  // Don't make ois and oos in ADJACENT statements.
			joinMessage = ((String) ois.readObject()).trim(); // Must cast 1st message read from type Object to String.
			oos = new ObjectOutputStream(s.getOutputStream());// trim() drops leading/trailing (but not imbeded) blanks.
		} catch(Exception e) { // connecting client may not be using oos or firstMessage was not a String
			System.out.println("Client " + clientAddress + " join protocol not OOS or 1st message not String. " + e );
			if (s.isConnected()) {
				try {
					s.close(); // hang up
				} catch(IOException ioe){
					// already hung up!
				}
			}
			return; // return to the Thread object to terminate this client thread. 
		} finally { // create a next-client thread whether catch was entered or not. 
			new Thread(this).start();
		}
		// If we are still running here then s, ois, oos are all good and the join message was a String! 
		// If not, we have dumped this caller and are waiting again (with a new thread) in accept() above for the next client to connect.
		
		// Join Attempt
		try {
			int blankOffset = joinMessage.indexOf(" ");			// scan joinMessage for a blank
			if (blankOffset < 0) {								// negative offset return means no blank found.
				try {
					System.out.println("No blank in join message: " + joinMessage);
					oos.writeObject("Invalid format in 1st message."); // not specific in case of hacker caller
					oos.close(); // kill connection
				} catch(Exception e) {
					// do nothing
				}
				return;      // kill client session thread
			}
			
			chatName = joinMessage.substring(0,blankOffset).toUpperCase(); // 2 parm substring() form is from-to (non-inclusive)
			providedPassword = joinMessage.substring(blankOffset).trim();  // 1 parm substring() means here-to-end
			
			if (passwords.containsKey(chatName)) {	// is this chatName a KEY in the passwords collection?
				storedPassword = passwords.get(chatName);// if YES, retrive the stored pw for this chatName
				if (providedPassword.equals(storedPassword)) {	// case-sensitive compare to pw just entered by user      	   
					if (whosIn.containsKey(chatName)) { // Is this chatName already in the chat room?        
						previousOOS = whosIn.get(chatName); // get previous oos before we replace it.
						whosIn.replace(chatName, oos);      // replace old oos with rejoin oos
						previousOOS.writeObject("Session terminated due to rejoin from another location.");
						previousOOS.close(); // shut down previous connection. (this prompts leave processing)
						System.out.println(chatName + " is rejoining.");
					}
				} else { // a password was retrieved from the passwords collection, but entered pw is not = to it.
					oos.writeObject("Your entered password " + providedPassword + " is not the same as the password stored for chat name " + chatName);
					oos.close(); // hang up.
					System.out.println("Invalid password: " + providedPassword + " instead of " + storedPassword + " for " + chatName);
					return;      // and kill this client thread
				} // end of processing for the case: pw was found in the passwords collection
			} else {	
				// If chatName is NOT IN the passwords collection then this chatName has NEVER joined before. 
				// So we will be happy to join them now, and ACCEPT (vs. test) their provided password.
				passwords.put(chatName, providedPassword);
				savePasswords(); // save updated passwords list on disk.
				savedMessages.put(chatName, new Vector<Object>());
				saveMessages(); // save updated message list on disk.
				System.out.println(chatName + " is a new client in the chat room.");
			}
			
			oos.writeObject("Welcome to the chat room " + chatName + " !");	// send "join is successful" to new client
			
			// Send missed messages
			Vector<Object> messagesSavedForThisClient = savedMessages.get(chatName);
			oos.writeObject(messagesSavedForThisClient);
			messagesSavedForThisClient.clear();
			saveMessages();
			
			whosIn.remove(chatName.toLowerCase());
			whosIn.put(chatName,oos);	// add new-join client
			
			// Adding chatNames not in chat to list
			for (String key : passwords.keySet()) {
				if(!whosIn.containsKey(key.toLowerCase()) && !whosIn.containsKey(key.toUpperCase())){
					whosIn.put(key.toLowerCase(),oos);	
				}
			}
			
			System.out.println(chatName + " is joining");	// trace message on server console
			
			String[] whosInArray = whosIn.keySet().toArray(new String[0]);
			Arrays.sort(whosInArray); // note the sort() method is not in the array object, it is in a separate Arrays class. 
			
			sendToAllClients("Welcome to " + chatName + " who has just joined (or rejoined) the chat room!");    
			sendToAllClients(whosInArray);
	
			// Now, as a trace message, show who the server is sending to the clients as "in the chat room".
			String whosInString = "";
			for (String name : whosInArray)
			     whosInString += name + ", ";
			System.out.println("Currently in the chat room: " + whosInString);
			
		} catch (Exception e) {	// bottom of try for join processing                         
			System.out.println("Connection failure during join processing from " + chatName + " at " + clientAddress + " " + e);
			if (s.isConnected()) {
				try {
					s.close();	// hang up
				} catch(IOException ioe) {
					// already hung up!	
				}
			}
			return; // kill this client's thread
		}
		
		// "SEND/RECEIVE processing" 
		try {
			while (true) { // client thread loops forever here  
				Object message = ois.readObject(); // wait for this client to send something
				if (message instanceof String) {
					String messageString = (String) message; // Copy and CAST pointer 
					System.out.println("Received '" + messageString + "' from " + chatName);
					sendToAllClients(chatName + " says: " + messageString);
				} else if (message instanceof ArrayList) {	
					ArrayList messageArray = (ArrayList) message;
					String[] messageList = new String[messageArray.size()];
					for(int i = 0; i < messageArray.size(); i++) {
						messageList[i] = (String) messageArray.get(i);
					}
					System.out.println("Received '" + messageList[0] + "' from " + chatName);
					sendToSaveForSelectedRecipients(chatName, messageList);
				} else {
					System.out.println("not a string" + message);
					System.out.println(message.getClass());
				}
			}
		} catch(Exception e) { // connection from client failed, probably because they left the chat room!
			// LEAVE processing
			ObjectOutputStream currentOOS = whosIn.get(chatName);
			if (currentOOS == oos) {
				// if OOS is still the same, then this IS the thread for the client that IS in the chat room now.
				whosIn.remove(chatName);
				whosIn.put(chatName.toLowerCase(), oos);
				
				String[] whosInArray = whosIn.keySet().toArray(new String[0]);
				Arrays.sort(whosInArray); // note the sort() method is not in the array object, it is in a separate Arrays class. 
				
				sendToAllClients("Goodbye to " + chatName + " who just left the chat room."); 
				sendToAllClients(whosInArray);
				System.out.println(chatName + " is leaving the chatroom.");
		
				// Now, as a trace message, show who the server is sending to the clients as "in the chat room".
				String whosInString = "";
				for (String name : whosInArray)
				     whosInString += name + ", ";
				System.out.println("Currently in the chat room: " + whosInString);
			} else {
				// if NOT = then this was the thread for the client in a PREVIOUS session.
			}
		}
	}
	
	
	// Sending message to all clients in the chat
	private synchronized void sendToAllClients(Object message) { // "synchronized" restricts client threads to enter one-at-a-time
		ObjectOutputStream[] oosArray = whosIn.values().toArray(new ObjectOutputStream[0]);
		for (ObjectOutputStream clientOOS : oosArray) {
			try {
				clientOOS.writeObject(message);
			} catch (IOException e) {
				// do nothing if send error because it's probably because this client has just left the chat room.	
			}
	    }
	}
	
	// Sending message to some clients in or out of the chat
	private synchronized void sendToSaveForSelectedRecipients(String sender, String[] messageAndRecipients) {
		if(messageAndRecipients[0].startsWith("PRIVATE ")) {
			// Appending list of recipients to message
			String recipientsList = " to ";
			boolean toSender = false;
			for(int i = 1; i < messageAndRecipients.length; i++) {
				if(messageAndRecipients[i].equals(sender)) {
					toSender = true;
				}
				recipientsList += messageAndRecipients[i] + ", ";
			}
			String messageSender = "You sent " + messageAndRecipients[0] + recipientsList;
			messageAndRecipients[0] += " sent from " + sender + recipientsList;
			
			// Sending confirmation message back to sender
			if(!toSender) {
				ObjectOutputStream senderOOS = whosIn.get(sender.toUpperCase());
				try {
					senderOOS.writeObject(messageSender);
				} catch (IOException e) {
					// do nothing if send error because it's probably because this client has just left the chat room.	
				}	
			}
			
			// Sending or saving message for selected recipients
			for(int i = 1; i < messageAndRecipients.length; i++) {
				// Sending
				if (whosIn.containsKey(messageAndRecipients[i].toUpperCase())) {
					ObjectOutputStream recipientOOS = whosIn.get(messageAndRecipients[i].toUpperCase());
					try {
						recipientOOS.writeObject(messageAndRecipients[0]);
					} catch (IOException e) {
						// do nothing if send error because it's probably because this client has just left the chat room.	
					}
				// Saving
				} else if(whosIn.containsKey(messageAndRecipients[i].toLowerCase())) {
					Vector<Object> messagesSavedForThisClient = savedMessages.get(messageAndRecipients[i].toUpperCase());
					Date now = new Date();
					messagesSavedForThisClient.add("On " + now + " received " + messageAndRecipients[0] + " from " + sender);
					saveMessages();
				}
			}
		} else {
			// Sending message to everyone in chat
			ObjectOutputStream[] oosArray = whosIn.values().toArray(new ObjectOutputStream[0]);
			for (ObjectOutputStream clientOOS : oosArray) {
				try {
					clientOOS.writeObject(sender + " says: " + messageAndRecipients[0]);
				} catch (IOException e) {
					// do nothing if send error because it's probably because this client has just left the chat room.	
				}
		    }
			
			// Appending list of out of chat recipients to message
			String recipientsList = " to ";
			for(int i = 1; i < messageAndRecipients.length; i++) {
				if(whosIn.containsKey(messageAndRecipients[i].toLowerCase())) {
					recipientsList += messageAndRecipients[i] + ", ";
				}
			}
			// Saving messages for out of chat recipients
			if(recipientsList != " to ") {
				// Sending confirmation message back to sender
				String messageSender = "You sent " + messageAndRecipients[0] + recipientsList;
				ObjectOutputStream senderOOS = whosIn.get(sender.toUpperCase());
				try {
					senderOOS.writeObject(messageSender);
				} catch (IOException e) {
					// do nothing if send error because it's probably because this client has just left the chat room.	
				}	
				// Saving messages for out of chat recipients
				Date now = new Date();
				messageAndRecipients[0] = "On " + now + " received " + messageAndRecipients[0] + " from " + sender;
				for(int i = 1; i < messageAndRecipients.length; i++) {
					if(whosIn.containsKey(messageAndRecipients[i].toLowerCase())) {
						Vector<Object> messagesSavedForThisClient = savedMessages.get(messageAndRecipients[i].toUpperCase());
						messagesSavedForThisClient.add(messageAndRecipients[0]);
						saveMessages();
					}
				}
			}
		}
	}
	
	// Saving passwords
	private synchronized void savePasswords() { 
		try {
			FileOutputStream   fos = new FileOutputStream("passwords.ser");
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(passwords);
			oos.close();
		} catch(Exception e) {
			System.out.println("passwords collection cannot be saved on disk: " + e);
		}
	}
	
	// Saving messages
	private synchronized void saveMessages() {
		try {
			FileOutputStream   fos = new FileOutputStream("SavedChatMessages.ser");
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(savedMessages);
			oos.close();
		} catch(Exception e) {
			System.out.println("message cannot be saved on disk: " + e);
		}
	}
}
