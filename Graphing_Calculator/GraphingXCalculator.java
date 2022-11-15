import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class GraphingXCalculator extends JPanel {
	
	static double[] xValues = new double[11];
	static double[] yValues = new double[11];
	
	public static void main(String[] args) throws IOException {
		boolean trace = false; // switch for non-debug output
		String inputTestFileName = "";
		
		if (args.length == 1) {	
			if (args[0].equalsIgnoreCase("TRACE")) {
				trace = true;
			} else if (args[0].equalsIgnoreCase("NOTRACE")) {
				trace = false;
			} else {
				inputTestFileName = args[0]; 
			}
		} else if(args.length != 0) {	 
			System.out.println("This calculator allows only a single command parameter with value 'TRACE' or 'NOTRACE' (the default) ");
			System.out.println("or the name of an input .txt file that contains test expressions (one per line).");
			return;
		}
		
		boolean readingFromFile = false;
		BufferedReader br; 
		if (inputTestFileName.length() == 0) {
			br = new BufferedReader(new InputStreamReader(System.in));
			System.out.println("Enter a simple expression (a SINGLE operator with a numeric operand on each side) or END to terminate.");
			System.out.println("Operators are + - * / ^ r  e.g. 25r2 has value 5");
			System.out.println("A 'unary' operator may precede (with no space!) an operand. e.g. -2 - -3 or -2--3 has value +1");
			System.out.println("An operand can be a number or the symbol pi or x");
			System.out.println("If the expression contains x, follow the expression with the keyword 'whenXis' followed by an x value.");
			System.out.println("To print 10 values of x and expression values, provide 'withXbumpof' at the end followed by a bump value.");
		} else {
			readingFromFile = true;
			System.out.println("Test cases will be read from file " + inputTestFileName);
			br = new BufferedReader(new FileReader(inputTestFileName));
		}

		while(true) { // accept expressions from the console or text file
			String enteredExpression = br.readLine();
			if (enteredExpression == null) {
				return;
			}
			enteredExpression = enteredExpression.trim(); 
			if (enteredExpression.equalsIgnoreCase("end")) {
				return;
			}
			if (enteredExpression.length() == 0) {
				continue;
			}
			int commentsOffset = enteredExpression.indexOf("//");
			if (commentsOffset > 0) {
				enteredExpression = enteredExpression.substring(0,commentsOffset).trim();	
			}
			if (enteredExpression.contains("=")) {
				System.out.println("Expression '" + enteredExpression + "' cannot contain the '=' symbol. Reenter expression.");
				continue;
			}
			if(readingFromFile && inputTestFileName.startsWith("Error")) {
				System.out.println(enteredExpression);
			}
			String expression = enteredExpression.toLowerCase().trim();
 
			// do operand substitution for pi
			expression = " " + expression + " "; // add a blank on left and right of expression
			int piOffset = expression.indexOf("pi");
			if (piOffset > 0) { // expression.contains("pi")
				char leftChar = expression.charAt(piOffset-1);
				char rightChar = expression.charAt(piOffset+2);
				if (trace) System.out.println("char to the left of pi is '" + leftChar + "' " 
												+ "and char to the right is '" + rightChar + "'.");
				boolean okOnLeft = false;
				if ((leftChar == ' ') // look for a blank or an operator
						|| (leftChar == '+')  
						|| (leftChar == '-')
						|| (leftChar == '*')
						|| (leftChar == '/')
						|| (leftChar == '^')
						|| (leftChar == 'r')) {
					okOnLeft = true;
				}
				boolean okOnRight = false;
				if ((rightChar == ' ') // look for a blank or an operator
						|| (rightChar == '+')  
						|| (rightChar == '-')
						|| (rightChar == '*')
						|| (rightChar == '/')
						|| (rightChar == '^')
						|| (rightChar == 'r')) {
					okOnRight = true;
				}
	  
				if (!(okOnLeft && okOnRight)) {
					System.out.println("Invalid use of pi operand. Reenter expression"); 
					continue; 
				} else {
					expression = expression.replace("pi", String.valueOf(Math.PI));
				}
			}
			
			// check consistency of "whenxis" and "withXbumpof" keywords
			int withOffset = expression.indexOf("withxbumpof");
			int whenOffset = expression.indexOf("whenxis");
			if(trace) {
				System.out.println("'whenXis' offset is " +  whenOffset  
									+ " and 'withXbumpof' offset is " + withOffset);
			}
			if (((withOffset > 0) && (whenOffset > 0)) && (whenOffset > withOffset)) {
				System.out.println("'withXbumpof' keyword must follow 'whenXis' keyword. Reenter expression.");
				continue;
			}
			if ((withOffset > 0) && (whenOffset < 0)) {
				System.out.println("'withXbumpof' keyword requires that 'whenXis' also be specified. Reenter expression.");
				continue;
			}
			if ((whenOffset < 0) && expression.contains("x")) {
				System.out.println("The expression contains x but a value for x is not provided following the keyword 'whenXis'. Reenter expression.");
				continue;
			}

			// retrieve withXbumpof value if one is provided.
			double bumpValue = 0;
			if (withOffset > 0) {
				String xBumpStringValue = expression.substring(withOffset+11).trim();
				if (xBumpStringValue.length() == 0) {
					System.out.println("Value of x bump not provided after 'withXbumpof' keyword.   Reenter expression.");
					continue;
				}
				try {
					bumpValue = Double.parseDouble(xBumpStringValue);
				} catch(NumberFormatException nfe) {
					System.out.println("Value '" + xBumpStringValue + "'provided following 'withXbumpof' is not numeric. Reenter expression.");
					continue;
				}
				expression = expression.substring(0,withOffset);
			}
			if(trace) System.out.println("x bump value is " + bumpValue
										+ " and new expression is '" + expression + "'.");

			// Check for expression starting with multiple operators prior to x substitution
			if (expression.startsWith("--")
					|| expression.startsWith("++")
					|| expression.startsWith("-+")
					|| expression.startsWith("+-")) {
				System.out.println("Expression starts with multiple operators. Reenter expression.");
				continue;
			}

			// retrieve value of x if one is provided
			double xValue = 0;
			String xStringValue = "";
			if (whenOffset > 0) { // an x value is provided
				xStringValue = expression.substring(whenOffset+7).trim();
				if(trace) System.out.println("xStringValue is '" + xStringValue + "'.");
				expression = expression.substring(0,whenOffset).trim();
				if (!expression.contains("x")) {
					System.out.println("A value is provided for x but the expression does not contain x.  Reenter expression.");
					continue;
				}
				
				try {
					xValue = Double.parseDouble(xStringValue);
				} catch(NumberFormatException nfe) {
					System.out.println("Value provided for x (" + xStringValue + ") is not numeric. Reenter expression.");
					continue;
				}
				if(trace) System.out.println("Value for x is " + xValue
											+ " and new expression is '" + expression + "'.");
			}
   

			// find the operator!
			
			// first check for a "double unary" caused by x value substitution above.
			if (whenOffset > 0) {
				if (expression.startsWith("-+")) {
					expression = "-" + expression.substring(2);
				} else if (expression.startsWith("+-")) {
					expression = expression.substring(1);	  
				} else if (expression.startsWith("++") || expression.startsWith("--")) {
					expression = expression.substring(2);	
				} 
				if (expression.startsWith("+") || expression.startsWith("-")) {
					System.out.println("Expression starts with multiple operators. Reenter expression.");
					continue;
				}
			}
			expression = expression.trim();
			if(trace)  System.out.println("At scan for an operator, expression is '" + expression + "'");

			int operatorOffset = 0;
			char operator = ' ';
			int i;
			for (i = 1; i < expression.length(); i++) { //(1st char shouldn't be an operator)
				if((expression.charAt(i) == '+')
						||(expression.charAt(i) == '-')
						||(expression.charAt(i) == '*')
						||(expression.charAt(i) == '/')
						||(expression.charAt(i) == '^')
						||(expression.charAt(i) == 'r')) {
					operator = expression.charAt(i);
					operatorOffset = i;
					break;
				}
			}
			if ((i == expression.length())   // operator is missing or is 1st
					|| (i == expression.length()-1)) { // or last char
				System.out.println("The expression is not an operator surrounded by operands.  Reenter expression.");
				continue;
			}
			if(trace) System.out.println("In expression '" + expression + "' the operator " + operator + " was found at offset " + operatorOffset);	      

			//find and check the operands!
			String leftOperand  = expression.substring(0,i).trim(); // remove any leading/trailing blanks.
			if(trace)  System.out.println("When first finding leftOperand, String value is '" + leftOperand + "'");
			boolean leftOperandIsX  = false;
			boolean rightOperandIsX = false;
			if (leftOperand.contains("x")) { // leftOperand IS x
				String testLeftOperand = leftOperand; 
				if (testLeftOperand.startsWith("+") || testLeftOperand.startsWith("-")) {
					testLeftOperand = testLeftOperand.substring(1); // remove sign
				}
				if (testLeftOperand.length() > 1) {
					System.out.println("Invalid left operand. Reenter expression"); 
					continue; 
				}
				leftOperandIsX = true;
			}
			leftOperand = leftOperand.replace("x", xStringValue).trim();
			if(trace)  System.out.println("After replace of x with xStringValue, leftOperand is '" + leftOperand + "'");
			String rightOperand = expression.substring(i+1).trim();
			if (rightOperand.contains("x")) {
				String testRightOperand = rightOperand; 
				if (testRightOperand.startsWith("+") || testRightOperand.startsWith("+")) {
					testRightOperand = testRightOperand.substring(1); // remove sign
				}
				if (testRightOperand.length() > 1) {
					System.out.println("Invalid right operand. Reenter expression"); 
					continue; 
				}
				rightOperandIsX = true;
			}

			rightOperand = rightOperand.replace("x", xStringValue).trim();

			if(trace) System.out.println("left operand is " + leftOperand + " and right operand is " + rightOperand);

			if (rightOperand.startsWith("--")
					|| rightOperand.startsWith("++")
					|| rightOperand.startsWith("-+")
					|| rightOperand.startsWith("+-")) {
				System.out.println("Invalid unary operators used with right operand. Reenter expression.");
				continue;
			}

			// replace x with it's String value in the expression
			expression = expression.replace("x", xStringValue);
			if(trace) System.out.println("The expression with x replaced by it's String value is: '"
										+ expression + "'.");

			// convert operands from String to double 
			// Note that parseDouble() will allow a unary operator!
			// But first check for a "double unary" caused by x value substitution above.
			if (whenOffset > 0) { // x value is provided
				if (rightOperand.startsWith("-+")) {
					rightOperand = "-" + rightOperand.substring(2);
				}
				if (rightOperand.startsWith("+-")) {
					rightOperand = rightOperand.substring(1);	  
				}
				if (rightOperand.startsWith("++") || rightOperand.startsWith("--")) {
					rightOperand = rightOperand.substring(2);	  
				}
			}
     
			double leftNumber = 0;
			double rightNumber= 0;
			try {
				if(trace) System.out.println("When converting to double, leftOperand is '" + leftOperand + "'");
				leftNumber = Double.parseDouble(leftOperand);
			} catch(NumberFormatException nfe) {
				System.out.println("In expression '" + expression + "' the left operand " + leftOperand + " is not numeric. Reenter expression.");
				continue;
			}
			
			try {
				if (operator == 'r') { // require a whole (int) root
					try {
						int rightNumberAsInt = Integer.parseInt(rightOperand);
						if (rightNumberAsInt < 0) {
							System.out.println("root must be a positive whole number. Reenter expression.");
							continue;
						}
						rightNumber = rightNumberAsInt;  
					} catch(NumberFormatException nfe) {
						System.out.println("root must be a whole number. Reenter expression.");
						continue;
					}
				} else { // operator is not 'r'
					rightNumber = Double.parseDouble(rightOperand);
				}
			} catch(NumberFormatException nfe) {
				System.out.println("In expression '" + expression + "' the right operand " + rightOperand + " is not numeric. Reenter expression.");
				continue;
			}
      
			// At this point we know that both operands are numeric.
			if (operator == '^') { // exponentiation
				if (rightNumber < 0) {
					System.out.println("The exponent must be positive.  Reenter expression");
					continue;
				}
				if (leftNumber < 0) {
					try {
						int exponentAsAnInt = Integer.parseInt(rightOperand);
					} catch(NumberFormatException nfe) {
						System.out.println("When left operand is negative, exponent must be a whole integer number. Reenter expression.");
						continue;
					}
				}
			}
       
			//_________________________________________________________________
			// At this point the expression looks good, so evaluate it's value...
			double operandBump = 0; 
			double progressingXvalue = xValue;
  
			for (int cycle = 0; cycle < 11; cycle++) {
				if (leftOperandIsX) {
					leftNumber  += operandBump;
				}
				if (rightOperandIsX) {
					rightNumber += operandBump;
				}
				double result = 0;
				boolean mustReenter = false;
				switch (operator) {
					case '+' :
						result = leftNumber + rightNumber;
						break;
					case '-' :
						result = leftNumber - rightNumber;
						break;
					case '*' :
						result = leftNumber * rightNumber;
						break;
					case '/' :
						result = leftNumber / rightNumber;
						break;
					case '^' :
						result = Math.pow(leftNumber,rightNumber);
						break;
					case 'r' :
						if (leftNumber > 0) {
							result = Math.pow( leftNumber,1/rightNumber);
							break;
						} else if (rightNumber%2 == 0) {
							mustReenter = true; break;
						} else {
							result =-Math.pow(-leftNumber,1/rightNumber);
							break;
						}
					default  : throw new IllegalArgumentException("Operator " + operator + " is not recognized.");
				}
				
				// Note the Math class offers square root and cube root methods,
				// but the form used above allows higher-order roots.
				if (mustReenter) {
					System.out.println("Invalid root expression. Reenter expression.");
					continue;
				}
	
				// Looks like a valid expression! Show evaluated result to user! 
				if (bumpValue == 0) {
					System.out.println(enteredExpression + " = "+ result); // show expression and value to user!
					break; // graph not requested
				}
	
				// save & print current cycle values
				xValues[cycle]  = progressingXvalue;
				yValues[cycle] = result;
				System.out.println("Cycle = " + cycle + " operandBump = " + operandBump + " xValue = " + progressingXvalue + " expression value = " + result);
	
				// bump things for next cycle
				operandBump = bumpValue; // bumping starts with 2nd graph point
				progressingXvalue += operandBump;
			} // bottom of for 0 to 11
   
			// print array values
			System.out.println("X Values   Expression Values");
			for (int n =0; n < xValues.length; n++) {
				System.out.println(xValues[n] + "        " + yValues[n]);
			}
			
			GraphingXCalculator gxc = new GraphingXCalculator(enteredExpression, xValues, yValues);
			
		} // bottom of while(true)
		
	} // end of main()  
	
	public GraphingXCalculator(String expression, double[] xValues, double[] yValues) {
		System.out.println("");
		System.out.println("____________________________________________________");
		System.out.println("");
		System.out.println("Hello from the constructor of GraphingXCalculator!");
		if ((expression == null) || (expression.trim().length() == 0)) {
			System.out.println("The expression was null or blank or zero length");
			return;
		}
		if ((xValues == null) || (yValues == null)) {
			System.out.println("Pointers to one or both value arrays are null");
			return;
		}
		if (xValues.length != yValues.length) {
			System.out.println("The xValues array and expressionsArray must be equal length.");
			System.out.println("(xValues[] is length " + xValues.length + 
								" and yValues[] is length " + yValues.length );
			return;
		}
		System.out.println("These values were passed for the expression '" + expression + "'");
		System.out.println("X Values      Expression Values");
		for (int i = 0; i < xValues.length; i++) {
			System.out.println(xValues[i] + "          " + yValues[i]);
		}
		
		JFrame graphWindow = new JFrame();
		
		graphWindow.setTitle(expression);
		graphWindow.getContentPane().add(this, "Center"); // GraphingCalculator is a JPanel being added!
		graphWindow.setSize(500,500);
		graphWindow.setVisible(true); 
		graphWindow.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		
		String[] xScaleValues;  // String versions of the provided xValues
		int[]    xClickOffsets; // where the x values are printed on the X axis
		// convert x scale values from double to String
		xScaleValues = new String[xValues.length];
		for (int i = 0; i < xValues.length; i++) {
		     xScaleValues[i] = String.valueOf(xValues[i]);
		}
		xClickOffsets = new int[xScaleValues.length];
		
		
		Graphics g = getGraphics(); // get drawing tool for this panel...
	
	}
	
	@Override
	public void paint(Graphics g) {
		double  yValueRange;
		double  yRangeToZero;
		int     windowWidth    = getSize().width; // the getSize() method
		int     windowHeight   = getSize().height;// is in the JPanel class.
		//int 	upperLeftDotX   = windowWidth/4;    // adjust the locations of 
		//int 	upperLeftDotY   = windowHeight/4;   // the upper-left "dot" and
		//int 	lowerRightDotX  = windowWidth-100;  // the lower-right "dot" depending
		//int 	lowerRightDotY  = windowHeight-100; // on the current window height & width.
		int     margin         = 50;
		
		int     yAxisYstart    = windowHeight - margin;
		int     yAxisYstop     = margin;
		int     yAxisXlocation = margin;
		int     yAxisLength    = windowHeight - (2*margin);
		int     yClickOffset   = yAxisYstart;
		
		int 	xAxisXstart    = margin;
		int 	xAxisXstop     = windowWidth - margin;
		int 	xAxisYlocation = yAxisYstart;
		int 	xAxisLength    = windowWidth - (2*margin);
		int 	xClickBump     = xAxisLength/(xValues.length-1);
		int 	xAxisZeroPoint = 0;
		int 	xClickOffset   = xAxisXstart;
		int[]	xClickOffsets = new int[xValues.length];
		
		double smallestY = Double.MAX_VALUE;
		double biggestY  = -Double.MAX_VALUE; // not Double.MIN_VALUE !
		for (double y : yValues) {
			if (y < smallestY) {
				smallestY = y;
			}
		    if (y > biggestY) {
		    	biggestY  = y;
		    }
		}
		String yTopScale    = "Y = " + String.valueOf(biggestY);
		String yBottomScale = "Y = " + String.valueOf(smallestY);
		
		if ((smallestY < 0) && (biggestY >= 0)) {
			double percentToYzeroPoint = Math.abs(smallestY)/(Math.abs(smallestY) + Math.abs(biggestY));
			int pixelsToSubtract = (int) (percentToYzeroPoint * yAxisLength); 
			xAxisYlocation -= pixelsToSubtract;
			System.out.println("Window height is " + windowHeight + " xAxisYlocation is at y = " + xAxisYlocation);
		}
		
		g.setColor(Color.BLACK);
		g.setFont(new Font("Times Roman", Font.BOLD, 20)); // medium
		g.drawLine(xAxisXstart, xAxisYlocation,  // from left (x,y) 
		           xAxisXstop,  xAxisYlocation); // to right (x,y)
		g.drawString("X", xAxisXstop+10, xAxisYlocation ); // String, x, y
		
		for (int i = 0; i < xValues.length; i++) {
			xClickOffsets[i] = xClickOffset; // save x offsets for later graphing of x,y values!
			g.drawString("|", xClickOffset-2, xAxisYlocation+5);//down a little
			g.drawString(Double.toString(xValues[i]), xClickOffset-14, xAxisYlocation+25);//down a little
	    	xClickOffset += xClickBump;
	    }
		
		xClickOffset = xAxisXstart; // reset to default left margin   
		if ((xValues[0] < 0) && (xValues[10] >= 0)) { // determine if there IS a zero point to find!
			// find the x offset where the xValue changes from - to +
			for (int i=0; i < xValues.length-1; i++) {
				xClickOffset += xClickBump;
				if ((xValues[i]<=0) && (xValues[i+1]>=0)) {
					break;
				}
			}
			xAxisZeroPoint = xClickOffset;
			System.out.println("Window width is " + windowWidth + " xAxisZeroPoint is at x = " + xAxisZeroPoint);
		}
		
		g.setColor(Color.BLACK);
		g.setFont(new Font("Times Roman", Font.BOLD, 20)); // medium
		if (xAxisZeroPoint == 0) { // draw Y axis at left margin
			g.drawLine(yAxisXlocation, yAxisYstart, yAxisXlocation, yAxisYstop); // from lower (x,y) to upper (x,y)
			g.drawString(yTopScale,    xAxisXstart+10, yAxisYstop + 10); // String
			g.drawString(yBottomScale, xAxisXstart+10, windowHeight);    // at (x,y)
		} else { // x axis has a 0 point, so draw Y axis at the x=0 point
		   g.drawLine(xAxisZeroPoint, yAxisYstart, xAxisZeroPoint, yAxisYstop);
		   g.drawString(yTopScale,    xAxisZeroPoint, yAxisYstop+10);
		   g.drawString(yBottomScale, xAxisZeroPoint, yAxisYstart);
		}
		
		// Convert yValues to y print coordinates (pixels).
		int[]  yPixelOffsets = new int[yValues.length];
		double valueRange  = biggestY - smallestY;
		int    pixelRange  = yAxisYstart - yAxisYstop;

		for (int i = 0; i < yValues.length; i++) {
			double valuePercent = (yValues[i]-smallestY)/valueRange;
			int    pixelOffset  = (int)(pixelRange * valuePercent);
			yPixelOffsets[i] = yAxisYstart - pixelOffset; // y is down-from-the-top, and Ystart is bottom (max)
			System.out.println("xValue is " + xValues[i] + " xPixelOffset is " + xClickOffsets[i]);
		    System.out.println("yValue is " + yValues[i] + " yPixelOffset is " + yPixelOffsets[i]);
		}
		
		g.setColor(Color.red); // setColor for drawing the graph lines
		for (int i = 0; i < yPixelOffsets.length; i++) {
			g.drawOval(xClickOffsets[i]-2, yPixelOffsets[i]-2, 4,4); // tiny 4x4 circle	  
			// Connect the dots! Draw lines between the points
			if (i>0) { 
				g.drawLine(xClickOffsets[i-1],yPixelOffsets[i-1],xClickOffsets[i], yPixelOffsets[i]);
			}
		}
	}
}
