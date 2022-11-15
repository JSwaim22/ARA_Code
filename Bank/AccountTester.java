public class AccountTester {
	
	public static void main(String[] args)  throws Exception {
		CheckingAccount a1 = new CheckingAccount();
	    a1.setCustomerName(args[0]);
	    a1.deposit(50);
	    a1.withdraw(25);
	    System.out.println(a1.getCustomerName() + " "
	                     + a1.getAccountNumber() + " "
	                     + " fees paid are $" + a1.getFeesPaid()     + " "
	                     + " balance is $" + a1.getBalance());
	    try {
	    	a1.payFee(30);
	    } catch(OverdraftException oe) {
	    	System.out.println("payFee failure due to insufficient funds."); 
	    }
	    
	    a1.deposit(10);
	    try {
	    	a1.payFee(30);
	    } catch(OverdraftException oe) {
	    	System.out.println("payFee failure due to insufficient funds."); 
	    }
	    
	    System.out.println(a1.getCustomerName() + " "
	            + a1.getAccountNumber() + " "
	            + " fees paid are $" + a1.getFeesPaid()     + " "
	            + " balance is $" + a1.getBalance());
	
	    SavingsAccount a2 = new SavingsAccount("Barnes,Betty");
	    a2.deposit(60);
	    a2.addInterest(20);
	    double withdrawAmount = 200;
	    try {
	    	a2.withdraw(withdrawAmount);
	    } catch(OverdraftException oe) {
	    	System.out.println("Withdraw of $" + withdrawAmount + " denied on account #" + a2.getAccountNumber()
	    	                + " due to insufficient funds.");
	    }
	    System.out.println(a2); // println calls toString() on OBJECTS 
	
	}
}