import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Stack;

public class Polynomial {
	List<ExpressionAtom> infixExpression = new ArrayList<ExpressionAtom>();

	List<ExpressionAtom> finalExpression;

	ListRepresentation listRepresentation;

	private String identifyUnaryMinuses(String expression) {
		if(expression.startsWith("-")) {
			expression = "%" + expression.substring(1); 
		}

		char[] expressionAtoms = expression.toCharArray();
		String returnExpression = String.valueOf(expressionAtoms[0]);
		for(int i = 1; i < expressionAtoms.length; ++i) {
			if(expressionAtoms[i] == '-' && expressionAtoms[i-1] == '(')
				returnExpression += "%";
			else
				returnExpression += String.valueOf(expressionAtoms[i]);
		}

		return returnExpression;
	}

	private String insertMultiplicationSigns(String expression) {
		char[] expressionAtoms = expression.toCharArray();
		String returnExpression = String.valueOf(expressionAtoms[0]);

		for(int i = 1; i < expressionAtoms.length; ++i) {
			if(!isOperator(expressionAtoms[i]) && !Character.isDigit(expressionAtoms[i]) 
					&& Character.isDigit(expressionAtoms[i-1]))
				returnExpression += "*";
			returnExpression +=  String.valueOf(expressionAtoms[i]);
		}

		return returnExpression;
	}

	private boolean isOperator(char token) {
		return token == '+' || token == '-' || token == '*' || token == '^'
				|| token == '(' || token == ')';
	}

	private List<ExpressionAtom> parseInputPolynomial(String inputExpression) {
		inputExpression = identifyUnaryMinuses(inputExpression);
		inputExpression = insertMultiplicationSigns(inputExpression);

		List<ExpressionAtom> inputExpressionTokens = new ArrayList<ExpressionAtom>();

		char[] inputChars = inputExpression.toCharArray();
		for(int i = 0; i < inputChars.length; ++i) {
			if(isOperator(inputChars[i]) || inputChars[i] == '%') {
				inputExpressionTokens.add(new ExpressionAtom(String.valueOf(inputChars[i]), 
						AtomType.OPERATOR, 1));
			} else {
				int lastIndex = inputExpressionTokens.size() - 1;
				if(lastIndex >= 0 && inputExpressionTokens.get(lastIndex).getAtomType() 
						== AtomType.OPERAND) {
					ExpressionAtom lastElement = inputExpressionTokens.remove(lastIndex);
					if(Character.isDigit(inputChars[i])) {
						lastElement.setCoefficient(lastElement.getCoefficient() * 10 + 
								Character.getNumericValue(inputChars[i]));
					} else {
						lastElement.setVariablesOrOperator(lastElement.getVariablesOrOperator() + 
								String.valueOf(inputChars[i]));
					}
					inputExpressionTokens.add(lastElement);
				} else if(Character.isDigit(inputChars[i])) {
					inputExpressionTokens.add(new ExpressionAtom("", AtomType.OPERAND, 
							Character.getNumericValue(inputChars[i])));
				} else {
					inputExpressionTokens.add(new ExpressionAtom(String.valueOf(
							inputChars[i]), AtomType.OPERAND, 1));
				}
			}
		}

		return inputExpressionTokens;
	}

	/**
	 * Checks if the first operator has higher precedence than the second operator.
	 * 
	 * @param ea1 The first operator.
	 * @param ea2 The second operator.
	 * @return true if the first operator has higher or equal precedence to the seconds operator
	 */
	private boolean isHigherPrecedence(ExpressionAtom ea1, ExpressionAtom ea2) {
		int op1Val = 0;
		int op2Val = 0;

		switch(ea1.getVariablesOrOperator().charAt(0)) {
		case '(':
			op1Val = 1;
			break;
		case '-':
		case '+':
			op1Val = 2;
			break;
		case '%':
			op1Val = 3;
			break;
		case '*':
			op1Val = 4;
			break;
		case '^':
			op1Val = 5;
			break;
		default:
			break;
		}

		switch(ea2.getVariablesOrOperator().charAt(0)) {
		case '(':
			op2Val = 1;
			break;
		case '-':
		case '+':
			op2Val = 2;
			break;
		case '%':
			op2Val = 3;
			break;
		case '*':
			op2Val = 4;
			break;
		case '^':
			op2Val = 5;
			break;
		default:
			break;
		}

		return op1Val >= op2Val;
	}

	/**
	 * Helper method to handle occurrences of a unary minus in the input expression.
	 * 
	 * @param operandStack Stack containing operands of the input expression.
	 */
	private void handleUnaryMinus(Stack<ListRepresentation> operandStack) {
		ExpressionAtom topNode = operandStack.peek().getNodeVal();

		// Case for unary minus applied to exponentiation subexpression
		// Invert the current status of isNegative
		if (topNode.getVariablesOrOperator().length() > 0 && topNode.getVariablesOrOperator().charAt(0) == '^') {
			operandStack.peek().setNegative(!operandStack.peek().isNegative());
		}
		// Case for unary minus applied to multiplication subexpression
		// Multiply the first operand of the multiplication subexpression's coefficient by -1
		else if (topNode.getVariablesOrOperator().length() > 0 && topNode.getVariablesOrOperator().charAt(0) == '*') {
			operandStack.peek().operands.get(0).getNodeVal().setCoefficient(operandStack.peek().operands.get(0).getNodeVal().getCoefficient() * -1);
		} 
		// Fall through case for unary minus applied to all other subexpressions
		// Multiply the top operand's coefficient by -1
		else {
			topNode.setCoefficient(topNode.getCoefficient() * -1);
		}
	}

	/**
	 * Helper method to handle occurrences of a binary minus in the input expression.
	 * 
	 * @param operand The subexpression who's operands have a binary minus applied to.
	 */
	public void handleBinaryMinus(ListRepresentation operand) {
		// Case for binary minus with second operand an exponentiation subexpression
		// Invert the current status of isNegative
		if (operand.getNodeVal().getVariablesOrOperator().length() > 0 && operand.getNodeVal().getVariablesOrOperator().charAt(0) == '^') {
			operand.setNegative(!operand.isNegative());
		}
		// Case for second operand being a single variable
		// Multiply the second operands coefficient by -1
		else if (operand.operands.isEmpty()) {
			operand.getNodeVal().setCoefficient(operand.getNodeVal().getCoefficient() * -1);
		}
		// Case for the second operand being a multiplication subexpression
		// Multiply the first operand of the multiplication subexpression's operands by -1
		else if (operand.getNodeVal().getVariablesOrOperator().charAt(0) == '*') {
			operand.operands.get(0).getNodeVal().setCoefficient(operand.operands.get(0).getNodeVal().getCoefficient() * -1);
		}
		// Fall through case for binary minus where second operand is a subexpression of subexpressions
		else {
			for (ListRepresentation lr : operand.operands) {
				String varOp = lr.getNodeVal().getVariablesOrOperator();
				// Case for exponentiation sub-subexpression
				// Invert the current status of isNegative
				if (varOp.length() > 0 && varOp.charAt(0) == '^') {
					lr.setNegative(!lr.isNegative());
				}
				// Case for multiplication sub-subexpression
				// Multiply the coefficient of the subexpression's first operand by -1
				else if (varOp.length() > 0 && varOp.charAt(0) == '*') {
					lr.operands.get(0).getNodeVal().setCoefficient(lr.operands.get(0).getNodeVal().getCoefficient() * -1);
				}
				// Fall through case for all other sub-subexpressions
				// Multiply sub-subexpression's coefficient by -1
				else {
					lr.getNodeVal().setCoefficient(lr.getNodeVal().getCoefficient() * -1);
				}
			}
		}
	}

	/**
	 * Helper method that combines all subexpressions with the same operator into a single ListRepresentation.
	 * 
	 * @param operator The most recent top of stack operator from the operator stack.
	 * @param operand1 The first operand from the operand stack.
	 * @param operand2 The second operand from the operand stack.
	 * @return compressed version of the ListRepresentation
	 */
	public List<ListRepresentation> handleOperatorCompression(ExpressionAtom operator, ListRepresentation op1, ListRepresentation op2) {
		List<ListRepresentation> operandsList = new ArrayList<ListRepresentation>();

		// Case for when the first operand has a subexpression with the same operator as the top of stack operator
		// Add all the subexpression's operands to the list operandsList
		if (operator.getVariablesOrOperator().equals(op1.getNodeVal().getVariablesOrOperator())) {
			for (ListRepresentation lr : op1.operands) {
				operandsList.add(lr);
			}
		}
		// Case for when the first operand's subexpression does not have the same operator
		// Add the first operand to the list operandsList
		else {
			operandsList.add(op1);
		}
		// Case for when the second operand has a subexpression with the same operator as the top of stack operator
		// Add all the subexpression's operands to the list operandsList
		if (operator.getVariablesOrOperator().equals(op2.getNodeVal().getVariablesOrOperator())) {
			for (ListRepresentation lr : op2.operands) {
				operandsList.add(lr);
			}
		}
		// Case for when the second operand's subexpression does not have the same operator
		// Add the second operand to the list operandsList
		else {
			operandsList.add(op2);
		}

		return operandsList;
	}

	/**
	 * Helper method that takes an operator and two operands (one operand for unary minus)
	 * and combines the operator and its operand(s) into a single ListRepresentation
	 * object which is then pushed back onto the stack. Also converts subtraction expressions 
	 * into their corresponding addition equivalent.
	 * 
	 * @param operatorStack
	 * @param operandStack
	 */
	public void handleStackOperations(Stack<ExpressionAtom> operatorStack, Stack<ListRepresentation> operandStack) {
		ExpressionAtom topOperator = operatorStack.pop();

		// Case for when the operator is unary
		if (topOperator.getVariablesOrOperator().charAt(0) == '%') {
			handleUnaryMinus(operandStack);
		}
		// Case for all binary operators
		else {
			ListRepresentation newLR = new ListRepresentation();
			ListRepresentation operand2 = operandStack.pop();
			ListRepresentation operand1 = operandStack.pop();

			// Convert binary minus to its addition equivalent
			if (topOperator.getVariablesOrOperator().charAt(0) == '-') {
				handleBinaryMinus(operand2);
				topOperator.setVariablesOrOperator("+");
			}

			// Set the nodeVal and operands of the new ListRepresentation
			// Compress subexpressions if needed
			newLR.setNodeVal(topOperator);
			newLR.operands.addAll(handleOperatorCompression(topOperator, operand1, operand2));

			operandStack.push(newLR);
		}
	}

	/**
	 * Takes the polynomial infixExpression represented as a list in infix notation 
	 * and generates its list representation. Converts all binary minus (subtraction) 
	 * to addition and handles a special case of a unary minus or subtraction of 
	 * exponentiation to set a field isNegative that represents the exponentiated 
	 * expression as being negative.
	 * 
	 * @return a valid list representation of the infix expression
	 */
	private ListRepresentation convertToListRepresentation() {
		Stack<ExpressionAtom> operatorStack = new Stack<ExpressionAtom>();
		Stack<ListRepresentation> operandStack = new Stack<ListRepresentation>();

		// Process each item in the infixExpression list
		for (ExpressionAtom ea : infixExpression) {
			// Case for handling operands
			// Push any operands encountered to the operandStack
			if (ea.getAtomType().equals(AtomType.OPERAND)) {
				ListRepresentation lrTemp = new ListRepresentation();
				lrTemp.setNodeVal(ea);
				operandStack.push(lrTemp);
			}
			// Case for handling operators
			else {
				char operator = ea.getVariablesOrOperator().charAt(0);

				// Case for handling left parenthesis
				if (operator == '(') {
					operatorStack.push(ea);
				}
				// Case for handling right parenthesis
				// Handle stack operations until a matching left parenthesis is encountered
				else if (operator == ')') {
					while (operatorStack.peek().getVariablesOrOperator().charAt(0) != '(') {
						handleStackOperations(operatorStack, operandStack);
					}
					// Discard left parenthesis from stack
					operatorStack.pop();
				}
				// Case for all other operators
				// Handle stack operations until the operatorStack is empty or the operator at the 
				// top of the stack is lower precedence than the current operator, then push the
				// current operator to the top of the operatorStack.
				else {
					while (!operatorStack.isEmpty() && isHigherPrecedence(operatorStack.peek(), ea)) {
						handleStackOperations(operatorStack, operandStack);
					}
					operatorStack.push(ea);
				}
			}
		}

		// When all elements from the infix expression have been processed, handle stack 
		// operations until operator stack is empty. The correct list representation should 
		// be the only item remaining on the operand stack.
		while (!operatorStack.isEmpty()) {
			handleStackOperations(operatorStack, operandStack);
		}

		return operandStack.pop();
	}

	/**
	 * Takes the previously computed ListRepresentation structure formed from the infix 
	 * expression and breaks it into its simplest form as a sum of products.
	 * 
	 * @return A list containing the elements to be summed in the final canonical form.
	 */
	private List<ExpressionAtom> evaluateExpression() {
		List<ExpressionAtom> atomsList = new ArrayList<ExpressionAtom>();

		ListRepresentation finalList = new ListRepresentation();
		finalList.setNodeVal(new ExpressionAtom("+", AtomType.OPERATOR, 1));

		// Case for a polynomial with a single variable and no operators
		if (this.listRepresentation.operands.isEmpty()) {
			atomsList.add(listRepresentation.getNodeVal());
			return atomsList;
		}
		// Case for all other polynomials. Calls a recursive helper method to 
		// evaluate the expression and store into finalList.
		else {
			evaluateExpressionHelper(finalList, listRepresentation);
		}

		// Moves all operands from finalList to the sum of products list, atomsList
		for (int i = 0; i < finalList.operands.size(); i++) {
			if (finalList.operands.get(i).getNodeVal().getAtomType().equals(AtomType.OPERAND)) {
				atomsList.add(finalList.operands.get(i).getNodeVal());
			}
		}

		return atomsList;
	}

	/**
	 * Helper method to evaluateExpression() that recursively traverses the ListRepresentation 
	 * of the polynomial and evaluates all the sub-expressions.
	 * 
	 * @param finalList List to store the evaluated expression as a sum of products in ListRepresentation form.
	 * @param lr The polynomial to be evaluated in ListRepresentation form.
	 */
	private void evaluateExpressionHelper(ListRepresentation finalList, ListRepresentation lr) {
		// Base case: returns when a sub-expression with no operands is reached
		if (lr.operands.isEmpty()) {
			return;
		}

		// Traverse all the sub-expressions recursively
		for (ListRepresentation operand : lr.operands) {
			evaluateExpressionHelper(finalList, operand);
		}

		char operator = lr.getNodeVal().getVariablesOrOperator().charAt(0);
		// Case for handling addition sub-expression
		if (operator == '+') {
			evaluateAddition(finalList, lr);
		}
		// Case for handling multiplication sub-expression
		// If multiplication's operands have sub-expressions, remove them from the finalList
		else if (operator == '*') {
			for (ListRepresentation listRep : lr.operands) {
				if (listRep.operands.size() > 0 && listRep.getNodeVal().getVariablesOrOperator().charAt(0) != '^') {
					for (int i = 0; i < listRep.operands.size(); i++) {
						finalList.operands.remove(finalList.operands.size() - 1);
					}
				}
			}
			evaluateMultiplication(finalList, lr);
		}
		// Case for handling exponentiation sub-expression
		// If exponentiation's operands have sub-expressions, remove them from the finalList
		else if (operator == '^') {
			if (lr.operands.get(0).operands.size() > 0) {
				for (int i = 0; i < lr.operands.size(); i++) {
					finalList.operands.remove(finalList.operands.size() - 1);
				}
			}
			evaluateExponentiation(finalList, lr);
		}
	}

	/**
	 * Helper method to evaluate addition sub-expressions.
	 * 
	 * @param finalList List to store the evaluated expression as a sum of products in ListRepresentation form.
	 * @param lr The sub-expression to evaluate.
	 */
	private void evaluateAddition(ListRepresentation finalList, ListRepresentation lr) {
		for (ListRepresentation operand : lr.operands) {
			finalList.operands.add(operand);
		}
	}

	/**
	 * Helper method to evaluate multiplication sub-expressions.
	 * 
	 * @param finalList List to store the evaluated expression as a sum of products in ListRepresentation form.
	 * @param lr The sub-expression to evaluate.
	 */
	private void evaluateMultiplication(ListRepresentation finalList, ListRepresentation lr) {
		// Set negative flag to help with operator distribution in result
		boolean isNeg = false;
		if (lr.isNegative()) {
			isNeg = true;
		}

		// Go through all operands in the expressions starting at the first index and multiply them by 
		// the expression at the 0th index, replacing the 0th index with the computed result after each 
		// newly computed expression
		for (int i = 1; i < lr.operands.size(); i++) {
			ListRepresentation temp = new ListRepresentation();
			temp.setNodeVal(new ExpressionAtom("+", AtomType.OPERATOR, 1));
			ListRepresentation op1 = lr.operands.get(0);
			
			// Case for single variable first operand multiplication
			if (op1.operands.size() == 0) {
				// Case for single variable second operand multiplication
				if (lr.operands.get(i).operands.size() == 0) {
					int coeff = op1.getNodeVal().getCoefficient() *
							lr.operands.get(i).getNodeVal().getCoefficient();
					String var = op1.getNodeVal().getVariablesOrOperator() +
							lr.operands.get(i).getNodeVal().getVariablesOrOperator();
					ExpressionAtom node = new ExpressionAtom(var, AtomType.OPERAND, coeff);
					temp.setNodeVal(node);
				}
				// Case for second operand being exponentiation
				else if (lr.operands.get(i).getNodeVal().getVariablesOrOperator().charAt(0) == '^') {
					evaluateExponentiation(finalList, lr.operands.get(i));
				}
				// Fall through for all other cases with first operand as single variable
				else {
					for (int k = 0; k < lr.operands.get(i).operands.size(); k++) {
						int coeff = 1;
						String var = "";
						
						// Compute the new coefficient and variable from the two sub-elements
						if (lr.operands.get(i).operands.get(k).getNodeVal().getAtomType().equals(AtomType.OPERAND)) {
							coeff = op1.getNodeVal().getCoefficient() * 
									lr.operands.get(i).operands.get(k).getNodeVal().getCoefficient();
							var = op1.getNodeVal().getVariablesOrOperator() +
									lr.operands.get(i).operands.get(k).getNodeVal().getVariablesOrOperator();
						}
						
						else {
							if (lr.operands.get(i).operands.get(k).getNodeVal().getVariablesOrOperator().charAt(0) == '*') {
								coeff = op1.getNodeVal().getCoefficient() *
										lr.operands.get(i).operands.get(k).operands.get(0).getNodeVal().getCoefficient();
								var = op1.getNodeVal().getVariablesOrOperator() +
										lr.operands.get(i).operands.get(k).operands.get(0).getNodeVal().getVariablesOrOperator();
							}
						}
						ListRepresentation tempElement = new ListRepresentation();
						ExpressionAtom node = new ExpressionAtom(var, AtomType.OPERAND, coeff);
						tempElement.setNodeVal(node);
						temp.operands.add(tempElement);
					}
				}
			}
			// Case for first operand as a multiple variable expression
			else {
				for (int j = 0; j < op1.operands.size(); j++) {
					for (int k = 0; k < lr.operands.get(i).operands.size(); k++) {
						// Compute the new coefficient and variable from the two sub-elements
						int coeff = op1.operands.get(j).getNodeVal().getCoefficient() * 
								lr.operands.get(i).operands.get(k).getNodeVal().getCoefficient();
						String var = op1.operands.get(j).getNodeVal().getVariablesOrOperator() +
								lr.operands.get(i).operands.get(k).getNodeVal().getVariablesOrOperator();
						ListRepresentation tempElement = new ListRepresentation();
						ExpressionAtom node = new ExpressionAtom(var, AtomType.OPERAND, coeff);
						tempElement.setNodeVal(node);
						temp.operands.add(tempElement);
					}
				}
			}
			lr.operands.remove(0);
			lr.operands.add(0, temp);
		}

		ListRepresentation lrTemp = lr.operands.get(0);
		lr = lrTemp;

		// Distribute negative sign throughout operands if expression had isNeg flag
		if (isNeg) {
			if (lr.operands.size() == 0) {
				lr.getNodeVal().setCoefficient(lr.getNodeVal().getCoefficient() * -1);
			} else {
				for (int i = 0; i < lr.operands.size(); i++) {
					lr.operands.get(i).getNodeVal().setCoefficient(lr.operands.get(i).getNodeVal().getCoefficient() * -1);
				}
			}
		}

		// Add computed sum of products list to the finalList
		if (lr.operands.size() == 0) {
			finalList.operands.add(lr);
		} else {
			for (int i = 0; i < lr.operands.size(); i++) {
				finalList.operands.add(lr.operands.get(i));
			}
		}
	}

	/**
	 * Helper method to evaluate exponentiation sub-expressions. Converts all exponentiation with 
	 * exponent greater than 0 to its corresponding multiplication expression equivalent and 
	 * exponentiation with exponent equal to 0 to 1.
	 * 
	 * @param finalList List to store the evaluated expression as a sum of products in ListRepresentation form.
	 * @param lr The sub-expression to evaluate.
	 */
	private void evaluateExponentiation(ListRepresentation finalList, ListRepresentation lr) {
		ListRepresentation temp = new ListRepresentation();
		int exponent = lr.operands.get(1).getNodeVal().getCoefficient();
		
		// Case for exponent as 0, sets the computed subexpression to 1 or -1 if isNegative flag was set
		if (exponent == 0) {
			temp.setNodeVal(new ExpressionAtom("", AtomType.OPERAND, 1));

			if (lr.isNegative()) {
				temp.getNodeVal().setCoefficient(temp.getNodeVal().getCoefficient() * -1);
				temp.setNegative(true);
			}

			lr = temp;
			finalList.operands.add(temp);
			return;
		}
		// Case for exponent > 0, converts the exponentiation into a series of products
		else {
			temp.setNodeVal(new ExpressionAtom("*", AtomType.OPERATOR, 1));
			ListRepresentation base = lr.operands.get(0);

			if (lr.isNegative()) {
				temp.setNegative(true);
			}

			for (int i = 0; i < exponent; i++) {
				temp.operands.add(base);
			}

			lr = temp;
			evaluateMultiplication(finalList, lr);
		}
	}

	/**
	 * Simplifies the polynomial obtained from the evaluateExpression() method by 
	 * combining like terms so the polynomial will be in its final, canonical form.
	 * 
	 * @param evaluatedExpression The evaluated polynomial List products.
	 * @return A List representing the final canonical form of the polynomial.
	 */
	private List<ExpressionAtom> simplifyAndNormalize(List<ExpressionAtom> evaluatedExpression) {
		List<ExpressionAtom> finalAtoms = new ArrayList<ExpressionAtom>();

		boolean found;

		// Go through all items in the evaluatedExpression and check for like terms, 
		// combining by summing their coefficients as needed.
		for (ExpressionAtom ea1 : evaluatedExpression) {
			found = false;
			ea1.setVariablesOrOperator(sortString(ea1.getVariablesOrOperator()));
			for (ExpressionAtom ea2 : finalAtoms) {
				if (ea2.getVariablesOrOperator().equals(ea1.getVariablesOrOperator())) {
					ea2.setCoefficient(ea2.getCoefficient() + ea1.getCoefficient());
					found = true;
					break;
				}
			}
			if (!found) {
				finalAtoms.add(ea1);
			}
		}

		return finalAtoms;
	}

	public Polynomial(String inputPolynomial) {
		this.infixExpression = parseInputPolynomial(inputPolynomial);

		this.listRepresentation = convertToListRepresentation();
	}

	private String sortString(String termVars) {
		char[] ar = termVars.toCharArray();
		Arrays.sort(ar);
		return String.valueOf(ar);
	}

	public void evaluate() {
		List<ExpressionAtom> evaluatedExpression = evaluateExpression();

		for(int i = 0; i < evaluatedExpression.size(); ++i) {
			evaluatedExpression.get(i).setVariablesOrOperator((sortString(
					evaluatedExpression.get(i).getVariablesOrOperator())));
		}

		this.finalExpression = simplifyAndNormalize(evaluatedExpression);
	}
}