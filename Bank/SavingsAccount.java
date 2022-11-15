public class SavingsAccount extends CashAccount {
	private double interestReceived;
	
	public SavingsAccount() throws Exception {
		super();
	}
	
	public SavingsAccount(String customerName) throws Exception {
		super(customerName);
	}
	
	private SavingsAccount(String customerName, int accountNumber, double balance) { // added for DB
	   super(customerName, accountNumber, balance);	// restores a SavingsAccount 	
	}                                           	// with DB data.
	
	public static SavingsAccount restoreFromDataBase(String customerName, int accountNumber, double balance) { // added for DB
		return new SavingsAccount(customerName, accountNumber, balance);	
	}
	
	public double getInterestReceived() {
		return interestReceived;
	}
	
	public void addInterest(double amount) {
		if (amount < 0) throw new IllegalArgumentException("Interest amount must be positive.");
		deposit(amount); // subtract fee from account balance
		interestReceived += amount; // keep track f total fees paid.
	}
	
	public String toString() {
		return super.toString() + " total interest accrued $" + getInterestReceived(); 
	}
}