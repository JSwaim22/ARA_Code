import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.rmi.Naming;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;


public class TellerClient implements ActionListener {
	public static void main(String[] args) {
		if (args.length == 0) {
			System.out.println("Provide BankServer network address as the command line parameter."); 
			return; 
		}
		try {
			new TellerClient(args[0]);
		} catch(Exception e) {
			System.out.println("Problem connecting to BankServer. " + e);
		}
	}

	// Instance Variables
	private TellerServerInterface server;
	private String newLine = System.lineSeparator();
	
	//GUI objects
	private JFrame      window                = new JFrame("BANK TELLER");	
	private JTextField  accountTextField      = new JTextField(8); 
	private JTextField  amountTextField       = new JTextField(8);
	private JTextField  customerNameTextField = new JTextField(24);
	private JLabel      accountLabel          = new JLabel("Enter account #");
	private JLabel      amountLabel           = new JLabel("Enter $ amount");
	private JLabel      customerNameLabel     = new JLabel("Enter customer name Last,First  Middle");
	private JButton     closeButton           = new JButton("Close Out Account");
	private JButton     clearButton           = new JButton("Clear Screen");
	private JButton     showAccountButton     = new JButton("Show Account");
	private JButton     depositButton         = new JButton("Deposit");
	private JButton     withdrawButton        = new JButton("Withdraw");
	private JButton     openNewCheckingButton = new JButton("Create New Checking");
	private JButton     openNewSavingsButton  = new JButton("Create New Savings");
	private JTextArea   displayTextArea       = new JTextArea();
	private JTextArea   transactionLogTextArea= new JTextArea();
	private JScrollPane displayScrollPane     = new JScrollPane(displayTextArea);
	private JScrollPane logScrollPane         = new JScrollPane(transactionLogTextArea);
	private JPanel      topPanel              = new JPanel();
	private JPanel      middlePanel           = new JPanel();
	private JPanel      bottomPanel           = new JPanel();
	
	
	//****************************************************************************	
	public TellerClient(String serverAddress) throws Exception { // CONSTRUCTOR
		System.out.println("Connecting to BankServer.");
		server = (TellerServerInterface) Naming.lookup("rmi://" + serverAddress + "/TellerServer");
		System.out.println("Connected to BankServer!");
	  
		// Build the GUI
		topPanel.add(clearButton);
		topPanel.add(showAccountButton);
		topPanel.add(accountLabel);
		topPanel.add(accountTextField);
		topPanel.add(amountLabel);
		topPanel.add(amountTextField);
		topPanel.add(depositButton);
		topPanel.add(withdrawButton);
		window.getContentPane().add(topPanel, "North");
		
		middlePanel.setLayout(new GridLayout(2,1)); // 2 rows, 1 col 
		middlePanel.add(displayScrollPane);
		middlePanel.add(logScrollPane);
		window.getContentPane().add(middlePanel, "Center");
	
		bottomPanel.add(closeButton);
		bottomPanel.add(customerNameLabel);
		bottomPanel.add(customerNameTextField);
		bottomPanel.add(openNewCheckingButton);
		bottomPanel.add(openNewSavingsButton);
		window.getContentPane().add(bottomPanel, "South");
		
		// Set attributes of GUI objects
		displayTextArea.setEditable(false);        // keep cursor out
		transactionLogTextArea.setEditable(false); // keep cursor out
		displayTextArea.setText("Current transaction details.");
		transactionLogTextArea.setText("Transactions for this customer in this session.");
		displayTextArea.setFont(new Font("default", Font.BOLD, 20));
		showAccountButton.setBackground(Color.black);
		showAccountButton.setForeground(Color.yellow);
		depositButton.setBackground(Color.blue);
		withdrawButton.setBackground(Color.green);
		depositButton.setForeground(Color.white);
		withdrawButton.setForeground(Color.black);
		clearButton.setBackground(Color.yellow);
		closeButton.setBackground(Color.red);
	    openNewCheckingButton.setBackground(Color.cyan);
		openNewSavingsButton.setBackground(Color.cyan);
		
		// Sign up for/activate event notification (ALL buttons, NO text fields.)
		showAccountButton.addActionListener(this); 
		openNewCheckingButton.addActionListener(this);
		openNewSavingsButton.addActionListener(this); 
		depositButton.addActionListener(this);
		withdrawButton.addActionListener(this);
		clearButton.addActionListener(this); 
		closeButton.addActionListener(this); 
		
	    // Show window	
		window.setLocation(10, 10); // horizontal, vertical
		window.setSize(1200, 400); // width,height in pixels
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	    window.setVisible(true);  
	}
	
	//****************************************************************************
	public void actionPerformed(ActionEvent ae) {
	  if (ae.getSource() == clearButton) { // this function is handled "locally"
	 	 displayTextArea.setText("Current transaction details.");
	     transactionLogTextArea.setText("Transactions for this customer in this session.");
	     amountTextField.setText("");      // clear all
	     customerNameTextField.setText("");// the other
	     accountTextField.setText("");     // text fields.
	     System.out.println("clear button was pushed");
	     return;
	  }
	  
	  try {	
		  if (ae.getSource() == openNewSavingsButton) createNewAccount("SAVINGS");
		  if (ae.getSource() == openNewCheckingButton)createNewAccount("CHECKING");
		  if (ae.getSource() == depositButton)        depositToAccount();
		  if (ae.getSource() == withdrawButton)       withdrawFromAccount();
		  if (ae.getSource() == showAccountButton)    showAccount();
		  if (ae.getSource() == closeButton)          closeOutAccount();
		  System.out.println(((JButton)ae.getSource()).getText() + " button was pushed"); 
	  } catch(Exception e) {
		  displayTextArea.setText(e.getMessage());
	  }
	}
	
	//*********************************************************************************
	// Methods called from actionPerformed() follow
	//*******************************************************************************
	private void createNewAccount(String accountType) throws Exception {
		notAccount("createNewAccount");
		notAmount("createNewAccount");
		String customerName = getCustomerName("createNewAccount");
		String serverReply = server.createNewAccount(accountType, customerName);
		displayTextArea.setText(serverReply);
		if(!serverReply.startsWith("ERROR:")) {
				transactionLogTextArea.append("\n" + serverReply);
		}
	}
	
	//*******************************************************************************
	private void showAccount() throws Exception {
		int accountNumber = getAccountNumber("showAccount");
		notAmount("showAccount");
		String customerName;
		if(accountNumber != 0) {
			customerName = getCustomerName("showAccount");
		} else {
			customerName = getShortCustomerName("showAccount");
		}
		String serverReply = server.showAccount(accountNumber, customerName);
		displayTextArea.setText(serverReply);
	}
	//*******************************************************************************
	private void depositToAccount() throws Exception {
		int accountNumber = getAccountNumber("depositToAccount");
		double amount = getAmount("depositToAccount");
		String customerName = getCustomerName("depositToAccount");
		String serverReply = server.depositToAccount(accountNumber, amount, customerName);
		displayTextArea.setText(serverReply);
		if(!serverReply.startsWith("ERROR:")) {
				transactionLogTextArea.append("\n" + serverReply);
				amountTextField.setText("");      // clear all
		}
	}
	
	//*******************************************************************************
	private void withdrawFromAccount() throws Exception {
		int accountNumber = getAccountNumber("withdrawFromAccount");
		double amount = getAmount("withdrawFromAccount");
		String customerName = getCustomerName("withdrawFromAccount");
		String serverReply = server.withdrawFromAccount(accountNumber, amount, customerName);
		displayTextArea.setText(serverReply);
		if(!serverReply.startsWith("ERROR:")) {
				transactionLogTextArea.append("\n" + serverReply);
				amountTextField.setText("");      // clear all
		}
	}
	
	//********************************************************************************
	private void closeOutAccount() throws Exception {
		int accountNumber = getAccountNumber("closeOutAccount");
		notAmount("closeOutAccount");
		String customerName = getCustomerName("closeOutAccount");
		String serverReply = server.closeOutAccount(accountNumber, customerName);
		displayTextArea.setText(serverReply);
		if(!serverReply.startsWith("ERROR:")) {
				transactionLogTextArea.append("\n" + serverReply);
		}
	}
	
	//*************************************************************************************
	//* Get/Edit methods follow (these are called by the actionPerformed() methods above) *    
	//*************************************************************************************
	private String getCustomerName(String callingMethodName) throws Exception {
	  String customerName = customerNameTextField.getText().trim();
	  if (customerName.length() == 0)
	      throw new IllegalArgumentException("Customer Name is required to " + callingMethodName);
	  if (!customerName.contains(","))
		  throw new IllegalArgumentException("Customer Name must be in format: Last, First Middle");
	  if (customerName.startsWith(","))
		  throw new IllegalArgumentException("Missing last name. Customer Name must be in format: Last, First Middle");
	  if (customerName.endsWith(","))
		  throw new IllegalArgumentException("Missing first name. Customer Name must be in format: Last, First Middle");
	  return customerName;
	}
	
	//*************************************************************************************
	private String getShortCustomerName(String callingMethodName) throws Exception {
	  String customerName = customerNameTextField.getText().trim();
	  if (customerName.length() == 0)
	      throw new IllegalArgumentException("At least a short Customer Name is required to show all customer accounts.");
	  return customerName;
	}
	//*********************************************************************************
	private int getAccountNumber(String callingMethodName) throws Exception {
	  String accountNumberString = accountTextField.getText().trim();
	  if (accountNumberString.length() == 0)
	      throw new IllegalArgumentException("Account number is required for " + callingMethodName);
	  int accountNumber;
	  try {
	      accountNumber = Integer.parseInt(accountNumberString);
	  } catch(NumberFormatException nfe) {
	      throw new IllegalArgumentException("Account number must be an integer.");
	  }
	  if (accountNumber < 0)
	      throw new IllegalArgumentException("Account number must be positive.");
	  return accountNumber;
	}
	
	//*********************************************************************************
	private double getAmount(String callingMethodName) throws Exception {
	  String amountString = amountTextField.getText().trim();
	  if (amountString.length() == 0)
	      throw new IllegalArgumentException("An amount is required for " + callingMethodName);
	  if (!amountString.contains("."))
	      throw new IllegalArgumentException("An amount must contain a decimal point.");
	  if (amountString.contains(","))
	      throw new IllegalArgumentException("An amount must not contain a comma.");
	  if (amountString.startsWith("$"))
		  amountString = amountString.substring(1).trim(); // drop 1st char
	  // now check for 2 decimal digits following the decimal point:
	  int decimalOffset = amountString.indexOf(".");
	  String decimalPortion = amountString.substring(decimalOffset+1);
	  if (decimalPortion.length() != 2)
		  throw new IllegalArgumentException("Amount must have two decimal digits following the decimal point.");
	  double amount;
	  try {
		  amount = Double.parseDouble(amountString);
	  } catch(NumberFormatException nfe) {
		  throw new IllegalArgumentException("Amount must be numeric.");
	  }
	  if (amount <= 0)
		  throw new IllegalArgumentException("Amount must be positive.");
	  return amount;
	} 
	
	//****************************************************************************
	private void notAmount(String callingMethodName) {
	  String amountNumberString = amountTextField.getText().trim();
	  if (amountNumberString.length() != 0)
	      throw new IllegalArgumentException("An amount should not be specified for " + callingMethodName); 
	}
	
	//****************************************************************************
	private void notAccount(String callingMethodName) {
	  String accountNumberString = accountTextField.getText().trim();
	  if (accountNumberString.length() != 0)
	      throw new IllegalArgumentException("An account number should not be specified for " + callingMethodName); 
	}
	
	//****************************************************************************
	private void notCustomerName(String callingMethodName) {
	  String customerNameString = customerNameTextField.getText().trim();
	  if (customerNameString.length() != 0)
	      throw new IllegalArgumentException("A customer name should not be specified for " + callingMethodName); 
	}
}
