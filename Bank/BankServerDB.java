import java.net.InetAddress;
import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.concurrent.ConcurrentHashMap;

import javax.xml.transform.Result;

public class BankServerDB extends    UnicastRemoteObject implements TellerServerInterface {
	
	ConcurrentHashMap<Integer,CashAccount> accounts = new ConcurrentHashMap<Integer,CashAccount>();
	private Connection connection;
	private PreparedStatement insertStatement;
	private PreparedStatement updateStatement;
	private PreparedStatement deleteStatement;
	private Statement selectAllStatement;
	
	public static void main(String[] args) throws Exception {
		System.out.println("Joshua Swaim");
		new BankServerDB(); // load server and call constructor to start 	
	}                     // the rmiregistry and register the server. 
	
	public BankServerDB() throws Exception {
		super(); // will throw RE if proxy files are not found
		LocateRegistry.createRegistry(1099); // find/start the rmiregistry on this computer
		System.out.println("rmiregistry is up at " + InetAddress.getLocalHost().getHostAddress()); // no need to show RMI port)
		Naming.rebind("TellerServer", this); // register EchoBankServer_Stub with the rmiregistry
		System.out.println("BankServer is up and registered as 'TellerServer'.");
		
		// Restore the accounts collection from the DataBase
		Class.forName("com.ibm.db2j.jdbc.DB2jDriver");
		System.out.println("DB driver loaded!");
		connection = DriverManager.getConnection("jdbc:db2j:C:\\DataBase\\QuoteDB");
		System.out.println("DB opened!");
		
		insertStatement = connection.prepareStatement( "INSERT INTO BANK_ACCOUNTS "
						+ "(ACCOUNT_NUMBER, ACCOUNT_TYPE, CUSTOMER_NAME, BALANCE) "
						+ "VALUES (?,?,?,?)");
		
		updateStatement = connection.prepareStatement( "UPDATE BANK_ACCOUNTS "
														+ "SET BALANCE = ? "
														+ "WHERE ACCOUNT_NUMBER = ?");
		
		deleteStatement = connection.prepareStatement( "DELETE FROM BANK_ACCOUNTS "
														+ "WHERE ACCOUNT_NUMBER = ?");
		
		selectAllStatement = connection.createStatement();
		ResultSet rs = selectAllStatement.executeQuery("SELECT * FROM BANK_ACCOUNTS");
		
		while (rs.next()) { // read the next row of the ResultSet
			// get the column values for this row
			int    accountNumber = rs.getInt   ("ACCOUNT_NUMBER");
			String accountType   = rs.getString("ACCOUNT_TYPE");
			String customerName  = rs.getString("CUSTOMER_NAME");
			double balance       = rs.getDouble("BALANCE");
			System.out.println(" acct#="    + accountNumber
	                 + " acctType=" + accountType
	                 + " custName=" + customerName
	                 + " balance="  + balance);
			CashAccount ca;
			if (accountType.equals("CHECKING")) {
				ca = CheckingAccount.restoreFromDataBase(customerName, accountNumber, balance);
			} else if (accountType.equals("SAVINGS")) {
				ca = SavingsAccount.restoreFromDataBase(customerName, accountNumber, balance);
			} else { 
				System.out.println("SYSTEM ERROR: account type " + accountType + " is not recognized when reading DB table BANK_ACCOUNTS in server constructor.");
				continue; // skip unrecognized account
			}
			accounts.put(accountNumber, ca);
		}
		System.out.println(accounts); // EASY!
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
			// Add a new row to the DB table for this new account
			try {
				insertStatement.setInt   (1, ca.getAccountNumber());
				insertStatement.setString(2, accountType);
				insertStatement.setString(3, customerName);
				insertStatement.setDouble(4, 0); // initial balance for a new account
				insertStatement.executeUpdate();
			} catch(SQLException sqle) {
				return "ERROR: Unable to add new account to the data base." + sqle.toString();
			}
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
			try {
				updateStatement.setDouble (1, ca.getBalance());
				updateStatement.setInt    (2, ca.getAccountNumber());
				updateStatement.executeUpdate();
			} catch(SQLException sqle) {
				return "ERROR: Server is unable to update account in the data base." + sqle.toString();
			}
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
			try {
				updateStatement.setDouble (1, ca.getBalance());
				updateStatement.setInt    (2, ca.getAccountNumber());
				updateStatement.executeUpdate();
			} catch(SQLException sqle) {
				return "ERROR: Server is unable to update account in the data base." + sqle.toString();
			}
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
				try {
					deleteStatement.setInt(1, accountNumber);
					deleteStatement.executeUpdate();
				} catch(SQLException sqle) {
					return "ERROR: Server is unable to delete account from the data base." + sqle.toString();
				}
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
		
	}
}
