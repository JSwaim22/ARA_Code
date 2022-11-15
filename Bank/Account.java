import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class Account implements Serializable { // bank account bean class (DATA OBJECT) 
	// The 5 (a "handful" of things to do in every bean class (when adding a field)
	// 1. declare the fields (private)
	// 2. write getter & setter method for each field
	// 3. consider additional constructors that take parameters ("GROUP setter")
	// 4. provide a toString() method ("GROUP getter")
	// 5. "implement" Serializable to allow externalization except "transient" fields
	private static int lastAccountNumber; 
	private static int getLastAccountNumber() throws Exception {
		if (lastAccountNumber == 0) { // JVM is just coming up!
			// our DB "hack"
			// lastAccountNumber = READ Data Base;
			ObjectInputStream ois = new ObjectInputStream(new FileInputStream("LastAccountNumber"));
			lastAccountNumber = (Integer) ois.readObject();
			ois.close();
		}
		lastAccountNumber++; // bump for this new account
		// WRITE updated nextAccountNumber to data base
		ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("LastAccountNumber"));
		oos.writeObject(lastAccountNumber);
		oos.close();
		return lastAccountNumber;
	}
	
	private String customerName;  // NO initial value!
	private int    accountNumber; // set from  bank DB vs. app
	
	public Account() throws Exception { // no-arg "default" constructor	
		System.out.println("In default constructor assigning account number.");
		accountNumber = getLastAccountNumber(); // static to Account object 
	}
	
	public Account(String customerName) throws Exception { // "group setter" for parameters
		this(); // call the no-arg constructor AND must be 1st line!
		setCustomerName(customerName); 
	}
	
	protected Account(String customerName, int accountNumber) { // added for DB
		setCustomerName(customerName);
		this.accountNumber = accountNumber; // sets EXISTING acct# instead of
	}                                   	// calling no-arg constructor.
	
	public int getAccountNumber() {
		return accountNumber;	
	}
	
	public String getCustomerName() {
		return customerName;
	}
	
	public void setCustomerName(String customerName) {
		 this.customerName = customerName;
	}
	
	public String toString() { // ("GROUP getter" - the INTRODUCE YOURSELF method! 
		return getClass().getName() + " #" + getAccountNumber() + " for " + getCustomerName(); 
	}
}