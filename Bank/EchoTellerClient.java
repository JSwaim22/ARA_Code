import java.rmi.Naming;

public class EchoTellerClient {

	public static void main(String[] args) throws Exception{
		TellerServerInterface server = (TellerServerInterface) Naming.lookup("rmi://localhost/TellerServer"); // retrieve Stub proxy from server's rmi registry
		System.out.println(server.createNewAccount("checking", "Swaim"));
	}

}
