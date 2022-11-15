import java.math.BigDecimal;
import java.math.MathContext;

public abstract class CashAccount extends Account {
	private double balance;
	
	public CashAccount() throws Exception {
		super();
	}
	
	public CashAccount(String customerName) throws Exception {
		super(customerName);// call PARENT constructor 
	}
	
	protected CashAccount(String customerName, int accountNumber, double balance) { // added for DB
		super(customerName, accountNumber);//sets acct# & cust from DB!
		this.balance = balance;            //sets balance from DB
	}
	
	public double getBalance() {
		return balance;
	}
	
	public synchronized void deposit(double amount) throws IllegalArgumentException {
		if (amount < 0) throw new IllegalArgumentException("deposit amount must be positive.");
		balance += amount;
	}
	
	public synchronized void withdraw(double amount) throws IllegalArgumentException, OverdraftException {
		if (amount < 0) throw new IllegalArgumentException("withdraw amount must be positive.");
		if (amount > balance) throw new OverdraftException();
		balance -= amount;
	}
	
	public String toString() {
		BigDecimal balanceBD = new BigDecimal(balance, MathContext.DECIMAL32); //sets precision to 7 digits
		balanceBD = balanceBD.setScale(2, BigDecimal.ROUND_UP); // 2 decimal digits
	    return super.toString() + " $" + balanceBD.toPlainString(); // no exponentials! 
	}
}