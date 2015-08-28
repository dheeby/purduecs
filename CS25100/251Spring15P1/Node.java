public class Node {
	private int id;
	private Node leftChild;
	private Node rightChild;
	
	public Node() {
		this(0);
	}
	
	public Node(int id) {
		this.id = id;
	}
	
	public void setid(int id) {
		this.id = id;
	}

	public void setLeftChild(Node n) {
		leftChild = n;
	}
	
	public void setRightChild(Node n) {
		rightChild = n;
	}
	
	public Node getLeftChild() {
		return leftChild;
	}
	
	public Node getRightChild() {
		return rightChild;
	}
	
	public int getid() {
		return id;
	}
}