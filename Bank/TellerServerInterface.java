import java.rmi.Remote;
import java.rmi.RemoteException;

public interface TellerServerInterface extends Remote {

	String createNewAccount(String accountType, String customerName) throws RemoteException;
	String depositToAccount(int accountNumber, double amount, String customerName) throws RemoteException;
	String withdrawFromAccount(int accountNumber, double amount, String customerName) throws RemoteException;
	String showAccount(int accountNumber, String customerName) throws RemoteException;
	String closeOutAccount(int accountNumber, String customerName) throws RemoteException;

}
