import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;

public class TreeParser {
	static int position;
	
	static String treeString;
	
	public static String getCleanedString(String text) {
		return text.replaceAll("\\s","");
	}
	
	private static Node parseAndCreateTree() {
		if (treeString.charAt(position) == '(') {
			if (treeString.charAt(position + 1) == ')') {
				position += 2;
				return null;
			}
			char val = treeString.charAt(position + 1);
			Node curNode = new Node(String.valueOf(val));
			position += 3;
			if (treeString.charAt(position - 1) == ')')
				return curNode;
			Node leftRoot = parseAndCreateTree();
			position += 1;
			Node rightRoot = parseAndCreateTree();
			position += 1;
			curNode.setLeftChild(leftRoot);
			curNode.setRightChild(rightRoot);
			return curNode;
		}
		return null;

	}
	
	public static Node createTree(String treeRepresentationText) {
		treeString = treeRepresentationText;
		position = 0;
		return parseAndCreateTree();
	}
	
	/*
	 * Checks for all matches of a branching tree pattern in the subject tree and returns
	 * a list containing the paths to the starting points of the matched patterns.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the branching tree pattern to be matched
	 * 
	 * @return a list of strings representing the path to matched patterns
	 */
	public static List<String> searchForNeedleInHaystack(Node haystack, Node needle) {
		List<String> positions = new ArrayList<String>();
		String path = "";
		searchForNeedleInHaystackHelper(haystack, needle, positions, path);
		
		// If no matches were found, add NONE to the list of paths
		if (positions.size() == 0) {
			positions.add("NONE");
		}
		
		return positions;
	}
	
	/*
	 * Recursive helper method for matching a branching tree. Checks all nodes in the 
	 * subject tree for possible matches of the pattern and stores the paths in a list.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the branching tree pattern to be matched
	 * @param list the list to store the string representations of paths to matched patterns
	 * @param path the string representation of the path to a matched pattern
	 */
	public static void searchForNeedleInHaystackHelper(Node haystack, Node needle, List<String> list, String path) {
		if (haystack == null) {
			return;
		}
		searchForNeedleInHaystackHelper(haystack.getLeftChild(), needle, list, path + "L");
		
		// If a match was found, add its path to the list or add ROOT if match is at the root
		if (checkPath(haystack, needle)) {
			if (path.equals("")) {
				list.add("ROOT");
			} else {
				list.add(path);
			}
		}
		
		searchForNeedleInHaystackHelper(haystack.getRightChild(), needle, list, path + "R");
	}
	
	/*
	 * Helper method to check if a branching tree pattern matches a subject tree at a given node.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the branching tree pattern to be matched
	 * 
	 * @return true if the pattern is matched and false otherwise
	 */
	public static boolean checkPath(Node haystack, Node needle) {
		// Condition when haystack and needle both reach a leaf node's child
		if (haystack == null && needle == null) {
			return true;
		}
		
		// Condition when haystack reaches a leaf node's child but needle is at a valid node
		if (haystack == null && needle != null) {
			return false;
		}
		
		// Condition where haystack is at a valid node but needle is at a leaf node's child
		if (haystack != null && needle == null) {
			return true;
		}
		
		// Recursively check both subject and pattern tree's left and right children
		// and perform a boolean AND to check for a match
		return checkPath(haystack.getLeftChild(), needle.getLeftChild()) && 
				checkPath(haystack.getRightChild(), needle.getRightChild());
	}
	
	/*
	 * Checks for all matches of a non-branching tree pattern in the subject tree and returns
	 * a list containing the paths to the starting points of the matched patterns.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the non-branching tree pattern to be matched
	 * 
	 * @return a list of strings representing the path to matched patterns
	 */
	public static List<String> searchForNeedleInHaystack(Node haystack, String needle) {
		List<String> positions = new ArrayList<String>();
		String path = "";
		searchForNeedleInHaystackHelper(haystack, needle, positions, path);
		
		// If no matches were found, add NONE to the list of paths
		if (positions.size() == 0) {
			positions.add("NONE");
		}
		
		return positions;
	}

	/*
	 * Recursive helper method for matching a non-branching tree. Checks all nodes in the 
	 * subject tree for possible matches of the pattern and stores the paths in a list.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the non-branching tree pattern to be matched
	 * @param list the list to store the string representations of paths to matched patterns
	 * @param path the string representation of the path to a matched pattern
	 */
	public static void searchForNeedleInHaystackHelper(Node haystack, String needle, List<String> list, String path) {
		if (haystack == null) {
			return;
		}
		searchForNeedleInHaystackHelper(haystack.getLeftChild(), needle, list, path + "L");
		
		// If a match was found, add its path to the list or add ROOT if match is at the root
		if (checkPath(haystack, needle)) {
			if (path.equals("")) {
				list.add("ROOT");
			} else {
				list.add(path);
			}
		}
		
		searchForNeedleInHaystackHelper(haystack.getRightChild(), needle, list, path + "R");
	}
	
	/*
	 * Helper method to check if a non-branching tree pattern matches a subject tree at a given node.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the non-branching tree pattern to be matched
	 * 
	 * @return true if the pattern is matched and false otherwise
	 */
	public static boolean checkPath(Node haystack, String needle) {
		Node current = haystack;
		
		// Check for full match of pattern and return false if a null node is reached before the end
		for (int i = 0; i < needle.length(); i++) {
			char next = needle.charAt(i);
			if (next == 'L') {
				current = current.getLeftChild();
				if (current == null) {
					return false;
				}
			}
			if (next == 'R') {
				current = current.getRightChild();
				if (current == null) {
					return false;
				}
			}
		}
		return true;
	}
	
	/*
	 * Checks all nodes in the subject tree for partial matches with the non-branching
	 * tree pattern, where the length of the partial match must be at least (3*M)/4 where
	 * M is an integer representing the number of nodes in the pattern.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the non-branching tree pattern to be matched
	 * 
	 * @return a list of strings representing the path to matched patterns
	 */
	public static List<String> approximateSearch(Node haystack, String needle) {
		List<String> positions = new ArrayList<String>();
		
		// Compute minimum length of accepted match
		int matchLength = (3 * needle.length()) / 4;
		String path = "";
		
		approximateSearchHelper(haystack, needle, positions, path, matchLength);
		
		// If no matches were found, add NONE to the list of paths
		if (positions.size() == 0) {
			positions.add("NONE");
		}
		
		return positions;
	}
	
	/*
	 * Recursive helper method for partial matching of a non-branching tree. Checks all nodes
	 * for partial matches and stores them in a list as strings representing the path to the 
	 * root of the partial match.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the non-branching tree pattern to be matched
	 * @param list the list to store the string representations of paths to matched patterns
	 * @param path the string representation of the path to a matched pattern
	 * @param matchLength the minimum length of an accepted partial path match
	 */
	public static void approximateSearchHelper(Node haystack, String needle, List<String> list, String path, int matchLength) {
		if (haystack == null) {
			return;
		}
		
		approximateSearchHelper(haystack.getLeftChild(), needle, list, path + "L", matchLength);
		
		// Check all possible substrings of valid length at the needle at the current node
		for (int i = 0; i < needle.length() - matchLength + 1; i++) {
			String subNeedle = needle.substring(i, needle.length());
			// If the length of the matched continuous path is greater than or equal to the minimum,
			// add its path to the list, or add ROOT if the path begins at the root node
			if (checkApproximatePath(haystack, subNeedle) >= matchLength) {
				if (path.equals("")) {
					if (!list.contains("ROOT")) {
						list.add("ROOT");
					}
				} else {
					if (!list.contains(path)) {
						list.add(path);
					}
				}
			}
		}
		
		approximateSearchHelper(haystack.getRightChild(), needle, list, path + "R", matchLength);
	}
	
	/*
	 * Helper method that finds the length of a continuous match of the pattern starting
	 * at the root of the subject tree.
	 * 
	 * @param haystack the subject tree to be searched
	 * @param needle the non-branching tree pattern to be matched
	 * 
	 * @return the number of nodes in a continuous matched path
	 */
	public static int checkApproximatePath(Node haystack, String needle) {
		int matchLength = 0;
		Node current = haystack;
		
		// Count the total number of nodes matched in the pattern and break on a null node
		for (int i = 0; i < needle.length(); i++) {
			char next = needle.charAt(i);
			if (next == 'L') {
				current = current.getLeftChild();
				if (current == null) {
					break;
				}
				matchLength++;
			}
			if (next == 'R') {
				current = current.getRightChild();
				if (current == null) {
					break;
				}
				matchLength++;
			}
		}
		
		return matchLength;
	}

	public static void main(String args[]) {
		Scanner in = new Scanner(System.in);

		int nCases = in.nextInt();
		in.nextLine();

		for(int i = 0; i < nCases; ++i) {
			String treeRepresentationText = in.nextLine();
			treeRepresentationText = getCleanedString(treeRepresentationText);
			System.out.println("Testcase " + (i + 1) + ": " + treeRepresentationText);
			
			Node rootSearchTree = createTree(treeRepresentationText);
			
			int nPaths = in.nextInt();
			in.nextLine();

			System.out.println("Output for testcase " + (i+1));
			for(int j = 0; j < nPaths; ++j) {
				String searchInput = in.nextLine();
				searchInput = searchInput.trim();
				
				String searchOption = searchInput.substring(0, 1);
				String searchPattern = searchInput.substring(2);
				
				searchPattern = getCleanedString(searchPattern);
				
				System.out.println("Query " + (j+1) + ": " + searchPattern);

				List<String> occurences;
				if(searchOption.equals("t")) { // Tree search
					Node rootPatternTree = createTree(searchPattern);
					occurences = searchForNeedleInHaystack(rootSearchTree, rootPatternTree);
				} else if(searchOption.equals("p")) { // Path search
					occurences = searchForNeedleInHaystack(rootSearchTree, searchPattern);
				} else { // Approximate Search
					occurences = approximateSearch(rootSearchTree, searchPattern);
				}
				Collections.sort(occurences);
				for(String position: occurences) {
					System.out.println(position);
				}
			}
		}
		in.close();
	}
}
