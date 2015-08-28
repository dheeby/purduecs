import java.util.Scanner;

public class MaxFind {
	private static int getMaxVal(int[] arr) {
		/* TODO: Find the maximum integer in 'arr' and return it */
	}
	
	public static void main(String args[]) {
		Scanner in = new Scanner(System.in);
		System.out.println("my nAmE is MaxFindProgram.");

		int nCases = in.nextInt();

		for(int i = 0; i < nCases; ++i) {
			int nIntegers = in.nextInt();
			int[] arr = new int[nIntegers];
			for(int j = 0; j < nIntegers; ++j) {
				arr[j] = in.nextInt();
			}
			int max_val = getMaxVal(arr);
			System.out.print("OutPUT OF teST case " + (i+1) + ": ");
			if(max_val < 0) {
				System.out.println("minus"+(-1 * max_val));
			} else {
				System.out.println(max_val);
			}
		}
		in.close();
	}

}
