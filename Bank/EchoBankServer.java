import java.net.InetAddress;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class EchoBankServer extends    UnicastRemoteObject implements TellerServerInterface {
	public static void main(String[] args) throws Exception{
		new EchoBankServer(); // load server and call constructor to start 	
	}                     // the rmiregistry and register the server. 
	
	public EchoBankServer() throws Exception {
	   super(); // will throw RE if proxy files are not found
	   LocateRegistry.createRegistry(1099); // find/start the rmiregistry on this computer
	   System.out.println("rmiregistry is up at " + InetAddress.getLocalHost().getHostAddress()); // no need to show RMI port)
	   Naming.rebind("TellerServer", this); // register EchoBankServer_Stub with the rmiregistry
	   System.out.println("EchoBankServer is up and registered as 'TellerServer'.");
	}
	
	public String createNewAccount(String accountType, String customerName) {
		return "Creating a new " + accountType + " account for " + customerName;
	}
	
	public String showAccount(int accountNumber, String customerName) {
	    return "Showing account #" + accountNumber + " of " + customerName;
	}
	
	public String depositToAccount(int accountNumber, double amount, String customerName) {
	    return "Depositing $" + amount + " to account #" + accountNumber + " of " + customerName;
	}
	
	public String withdrawFromAccount(int    accountNumber, double amount, String customerName) {
	    return "Withdrawing $" + amount + " from account #" + accountNumber + " of " + customerName;
	}
	
	public String closeOutAccount(int accountNumber, String customerName) {
		return "Closing out account #" + accountNumber + " of " + customerName + " (balance must be 0)";
	}
}
