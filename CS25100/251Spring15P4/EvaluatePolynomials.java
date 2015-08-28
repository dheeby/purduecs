import java.util.Collections;
import java.util.Comparator;
import java.util.Scanner;

public class EvaluatePolynomials {
		
	public static void main(String args[]) {

		Scanner in = new Scanner(System.in);
		
		int nCases = in.nextInt(); /*
		String[] inputs = {"(a+b)*(c+d)", "x^2 -y^32", "(x+y)^2", "x^2 - y^3", 
				"(x+y)*(x-y)", "(x+y)^2 - (x-y)^2", "(x+y)^2", "(a+b-c)^2", "x^2 - y^3", 
				"a - (x^2 - (k+6n)^3 - (z^2 - j^3))", "(x+z)^2 - (x+y)^2", "(x-y)^2", 
				"(x+y)*(x-y)", "(a+b)", "a+a", "(a+b)*(a+b)", "(a+b-c)^2", "a-(b+c*(d-(e+f-g)))", 
				"(2*x + y)*(2*x-y)", "a-b", "44*x-(a - (x^2 - y^3))", "x^2 - y^3", 
				"a - (b-((x+y)^2 - (44- 55)^5))", "a - (x^2 - y^3 - (z^8 - j^9))", 
				"a - (b-((x+y)^2 - (55- 44)^5))", "a", "5", "6 + 6", "6 * 7 * (a + h)*(1 - 1)", 
				"6 * (6-5)", "(2*x-3*y)*(2*x+3*y)", "(2x-3y)*(x + 3y)", "(abc + 11)^4", 
				"(a-b)  * (a^2 + ab + b^2)", "(a+b)  * (a^2 - ab + b^2)", 
				"3 * a * 6 * (a+a+4*a) * b * a", "a^2 - (bc)^3", "(a+b)", "(a-b)", 
				"(x+y)*(x+5)*(2*x - yz)", "x^2 - y^3", 
				"(1 - x) * (1 + x) * (1 + x^2) * (1 + x^4) * (1 + x^8)", 
				"2 - 1", "0 - 1", "1 - 1", "(1+x)^9", "a - a - a - a", 
				"a -a -a -a-a-a+a+a+100*a", "-x^2 + (-2x + 1)*(-2x-1)", "-2 + 3 - 4 -4 +10", 
				"3 + (-x^2) + (-2) + (-(-2x^2))", "2x^2", "((1 + x)^9 + 2)^0", "1 + 3 + 5+7+   9", 
				"10*(10*(1 - 1 + 2 - 2 + 3 -3 + 4 - (1 + 2 +3 -(-2) - 4 +1))) + 1 + 2 + 3 + 9 + 21 + 19", 
				"1999+(1-a)*(1+a)*(1+a^2)*(1+a^4)*(1+a^8)*(1+a^16)+2001a^32"}; //REMOVE */
		in.nextLine();
		
		for(int i = 0; i < nCases; ++i) {
	//	for(int i = 0; i < inputs.length; ++i) {//REMOVE
			 String input = in.nextLine();
			//String input = inputs[i];
			
			if(i > 0) {
				System.out.println("");
			}
			System.out.println("Test Case " + (i + 1) + ": " + Utils.getCleanedString(input));

			Polynomial inputPolynomial = new Polynomial(Utils.getCleanedString(input));
			
			System.out.print("\t");
			Utils.printListRepresentation(inputPolynomial.listRepresentation);
			System.out.println("");
			
			inputPolynomial.evaluate();
			
			

			Collections.sort(inputPolynomial.finalExpression, new Comparator<ExpressionAtom>(){
				private int getDegree(String operand) {
					try {
						Integer.valueOf(operand);
						return 0;
					} catch(Exception e) {
						return operand.length();
					}
				}

				public int compare(ExpressionAtom o1, ExpressionAtom o2){
			    	String var1 = o1.getVariablesOrOperator();
			    	String var2 = o2.getVariablesOrOperator();
			    	int degree1 = getDegree(var1);
			    	int degree2 = getDegree(var2);
			    	
			    	if(degree1 < degree2 || 
			    			(degree1 == degree2 && (var1.compareTo(var2) > 0)))
			    		return 1;
			    	else if(degree1 == degree2 && var1.equals(var2))
			    		return 0;
			    	return -1;
			    }
			});

			System.out.print("\t");
			Utils.printOutputExpression(inputPolynomial.finalExpression);	
		}
		
		in.close();
	}
}
