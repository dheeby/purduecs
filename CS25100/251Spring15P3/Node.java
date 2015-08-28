public class Node {
	private String info;
	private Node leftChild = null;
	private Node rightChild = null;
	
	public Node() {
		this("");
	}
	
	public Node(String info) {
		this.info = info;
	}
	
	public void setinfo(String info) {
		this.info = info;
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
	
	public String getinfo() {
		return info;
	}
	
	public boolean isLeaf() {
		return rightChild == null && leftChild == null;
	}	
}