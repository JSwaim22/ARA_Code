public class CheckingAccount extends CashAccount {
	private double feesPaid;
	
	public CheckingAccount() throws Exception {
		super();
	}
	
	public CheckingAccount(String customerName) throws Exception {
		super(customerName);
	}
	
	private CheckingAccount(String customerName, int accountNumber, double balance) { // added for DB
		super(customerName, accountNumber, balance);// restores a CheckingAccount	
	}                                           	// with DB data. 
	
	public static CheckingAccount restoreFromDataBase(String customerName, int accountNumber, double balance) { // added for DB
		return new CheckingAccount(customerName, accountNumber, balance);	
	}
	
	public double getFeesPaid() {	
		return feesPaid;
	}
	
	public void payFee(double amount) throws OverdraftException {
		if (amount < 0) throw new IllegalArgumentException("Fee amount must be positive.");
		withdraw(amount); // subtract fee from account balance
		feesPaid += amount; // keep track of total fees paid.
	}
	
	public String toString() {
		return super.toString() + " total fees paid $" + getFeesPaid(); 
	}
}