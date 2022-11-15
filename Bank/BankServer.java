import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;

public class BankServer extends    UnicastRemoteObject implements TellerServerInterface {
	
	ConcurrentHashMap<Integer,CashAccount> accounts = new ConcurrentHashMap<Integer,CashAccount>();
	
	public static void main(String[] args) throws Exception {
		System.out.println("Joshua Swaim");
		new BankServer(); // load server and call constructor to start 	
	}                     // the rmiregistry and register the server. 
	
	public BankServer() throws Exception {
		super(); // will throw RE if proxy files are not found
		LocateRegistry.createRegistry(1099); // find/start the rmiregistry on this computer
		System.out.println("rmiregistry is up at " + InetAddress.getLocalHost().getHostAddress()); // no need to show RMI port)
		Naming.rebind("TellerServer", this); // register EchoBankServer_Stub with the rmiregistry
		System.out.println("BankServer is up and registered as 'TellerServer'.");
		
		// Getting saved messages from file
		try {
			FileInputStream   fis = new FileInputStream("accounts.ser");
			ObjectInputStream ois = new ObjectInputStream(fis);
			accounts = (ConcurrentHashMap<Integer,CashAccount>) ois.readObject();	//cast type of object found
			ois.close();														//from Object to collection type
			System.out.println("Accounts: ");	// Printing everyone's chat name and password
			System.out.println(accounts);                      	// on the console can be very handy in testing.
		} catch(FileNotFoundException fnfe) {
			System.out.println("accounts.ser is not found, so an empty collection will be used.");
		}
		
	}
	
	public String createNewAccount(String accountType, String customerName) {
		try {
			CashAccount ca;
			if (accountType.equals("CHECKING"))
			    ca = new CheckingAccount(customerName);
			else if (accountType.equals("SAVINGS"))
			    ca = new SavingsAccount(customerName);
			else return "ERROR: Account type " + accountType + " is not recognized by the server. Call TechSupport.";
			
			accounts.put(ca.getAccountNumber(), ca);
			saveAccounts();
			
			return "CREATE: " + ca.toString(); // call toString() on the account bean OBJECT we just made, whatever it was!
		} catch(Exception e) {
			return "ERROR: The TellerServer cannot create new accounts. Call TechSupport." + e.toString();
		}
	}
	
	public String showAccount(int accountNumber, String customerName) {
		CashAccount ca = accounts.get(accountNumber);
		if(ca == null) {
			return "Account #" + accountNumber + " not found.";
		} else if(customerName.equals(ca.getCustomerName())) {
			return ca.toString();
		} else {
			return "Provided customer name does not match the stored customer name (" + ca.getCustomerName() + ") for this account.";
		}
	}
	
	public String depositToAccount(int accountNumber, double amount, String customerName) {
		CashAccount ca = accounts.get(accountNumber);
		if(ca == null) {
			return "Account #" + accountNumber + " not found.";
		} else if(customerName.equals(ca.getCustomerName())) {
			ca.deposit(amount);
			saveAccounts();
			return "DEPOSIT $" + amount + ": " + ca.toString();
		} else {
			return "Provided customer name does not match the stored customer name (" + ca.getCustomerName() + ") for this account.";
		}
	}
	
	public String withdrawFromAccount(int accountNumber, double amount, String customerName) {
		CashAccount ca = accounts.get(accountNumber);
		if(ca == null) {
			return "Account #" + accountNumber + " not found.";
		} else if(customerName.equals(ca.getCustomerName())) {
			try {
				ca.withdraw(amount);
			} catch(OverdraftException oe) {
				return "Insufficient Funds";
			}
			saveAccounts();
			return "WITHDRAW $" + amount + ": " + ca.toString();
		} else {
			return "Provided customer name does not match the stored customer name (" + ca.getCustomerName() + ") for this account.";
		}
	}
	
	public String closeOutAccount(int accountNumber, String customerName) {
		CashAccount ca = accounts.get(accountNumber);
		if(ca == null) {
			return "Account #" + accountNumber + " not found.";
		} else if(customerName.equals(ca.getCustomerName())) {
			if(ca.getBalance() == 0) {
				accounts.remove(accountNumber);
				saveAccounts();
				return "CLOSE: Account #" + accountNumber + " for " + customerName + " has been terminated.";
			} else {
				return "Balance must be $0.00 to close out.";
			}
		} else {
			return "Provided customer name does not match the stored customer name (" + ca.getCustomerName() + ") for this account.";
		}
	}
	
	// Saving accounts
	private synchronized void saveAccounts() { 
		try {
			FileOutputStream   fos = new FileOutputStream("accounts.ser");
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(accounts);
			oos.close();
		} catch(Exception e) {
			System.out.println("accounts collection cannot be saved on disk: " + e);
		}
	}
}
