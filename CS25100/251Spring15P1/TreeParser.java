import java.util.Scanner;

public class TreeParser {
	static int position;
	
	static String treeString;
	
	public static String getCleanedString(String text) {
		/* TODO: strip blanks in 'text' */
		String stripped = "";
		for (int i = 0; i < text.length(); i++) {
			if (text.charAt(i) != ' ') {
				stripped += text.charAt(i);
			}
		}
		return stripped;
	}
	
	public static Node createTree(String treeRepresentationText) {
		/* TODO: build the tree by parsing 'treeRepresentationText' */
		Node root = new Node();

		int commacount = 0;

		int strptr = 1;
		while (Character.isDigit(treeRepresentationText.charAt(strptr))) strptr++;

		int value = Integer.parseInt(treeRepresentationText.substring(1, strptr));
		root.setid(value);

		// After first comma, either have:
		//		Immediately another comma: no left child
		//		Left parentheses: has left child
		if (treeRepresentationText.charAt(strptr + 1) == ',') {
			root.setLeftChild(null);
			strptr++;
		} else {
			root.setLeftChild(createTree(treeRepresentationText.substring(strptr)));
		}

		// After second comma, either have:
		//		Immediately another comma: no right child
		//		Left parentheses: has left child
		if (treeRepresentationText.charAt(strptr + 1) == ')') {
			root.setRightChild(null);
			strptr++;
		} else {
			root.setRightChild(createTree(treeRepresentationText.substring(strptr)));
		}

		return root;
	}
	
	public static String traversePath(Node root, String direction) {
		/* TODO: traverse path in tree denoted by 'direction' and print node labels */
		/* print a '*' when path would leave the tree */
		return null;
	}
	
	public static void main(String args[]) {
		Scanner in = new Scanner(System.in);

		int nCases = in.nextInt();
		in.nextLine();

		for(int i = 0; i < nCases; ++i) {
			String treeRepresentationText = in.nextLine();
			treeRepresentationText = getCleanedString(treeRepresentationText);
			System.out.println("Testcase " + (i + 1) + ": " + treeRepresentationText);
			
			Node root = createTree(treeRepresentationText);
			
			int nPaths = in.nextInt();
			in.nextLine();

			System.out.println("Output for testcase " + (i+1));
			for(int j = 0; j < nPaths; ++j) {
				String path = in.nextLine();
				path = getCleanedString(path);

				String output = traversePath(root, path);
				
				System.out.println(path.trim() + ": " + output.trim());
			}
		}
		in.close();
	}
}
