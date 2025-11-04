/*
 * Cael Link
 * CS 1450 
 * Assignment 10
 * Due April 24, 2024
 * Description: Implement a binary search tree to descramble the song the parrots are singing. 
 */

import java.io.File;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;

public class LinkCaelAssignment10 {

	public static void main(String[] args) throws FileNotFoundException {

		BinaryTree binaryTree = new BinaryTree();
		
		File parrots = new File("parrots.txt");
		Scanner parrotsScanner = new Scanner(parrots);
		
		while (parrotsScanner.hasNext()) {
			
			// Read id
			int id = parrotsScanner.nextInt(); 
			// Read name
			String name = parrotsScanner.next(); 
			// Read song word
			String songWord = parrotsScanner.next();
		    
			
		    Parrot parrot = new Parrot(id, name, songWord);
		    binaryTree.insert(parrot);
		}
		parrotsScanner.close();
		//print the song based on level order
		System.out.println("Parrot's song");
		System.out.println("-------------");
		binaryTree.levelOrder();
		
		System.out.println("");
		
		//print out the leaf nodes
		System.out.println("\nParrots on Leaf Nodes");
		System.out.println("-------------");
		binaryTree.visitLeaves();
		
	}

}

class Parrot implements Comparable<Parrot> {
	
	//Private data fields
	private int id;
	private String name;
	private String songWord;
	
	//Constructor
	public Parrot(int id, String name, String songWord) {
		this.id = id;
		this.name = name;
		this.songWord = songWord;
	}
	
	//Getter
	public String getName() {
		return name;
	}
	
	public String sing() {
		return songWord;
	}
	
	//Compare id
	@Override
	public int compareTo(Parrot otherParrot) {
		if (this.id < otherParrot.id) {
			return -1;
		}
		else if (this.id > otherParrot.id) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

class BinaryTree {
	
	private TreeNode root;
	
	//Constructor
	public BinaryTree() {
		root = null;
	}
	
	public boolean insert(Parrot parrotToAdd) {
		
		//start the root
		if (root == null) {
			root = new TreeNode(parrotToAdd);
			
		}
		else {
			TreeNode parent = null;
			TreeNode current = root;
			
			while (current != null) {
				
				//if less that zero in compare, so less than, go to the left. 
				if (parrotToAdd.compareTo(current.parrot) < 0) {
					parent = current;
					current = current.left;
				}
				//returns 1, greater than so put to the right. 
				else if (parrotToAdd.compareTo(current.parrot) > 0) {
					parent = current;
					current = current.right;
				}
				//if zero, compare the same, found duplicate
				else {
					//Found duplicate
					return false;
				}//else
			}//while
			
			if(parrotToAdd.compareTo(parent.parrot) < 0) {
				parent.left = new TreeNode(parrotToAdd);
			}
			else {
				parent.right = new TreeNode(parrotToAdd);
			}
			
		}//else
		
		return true;
	}//insert 
	
	public void levelOrder() {
		
		//push onto Q
		if (root != null) {
			
			Queue<TreeNode> q = new LinkedList<>();
			q.offer(root);
		
			while (!q.isEmpty()) {
				
				TreeNode tempNode = q.remove();
				System.out.print(tempNode.parrot.sing());
				System.out.print(" ");
				
				if (tempNode.left != null) {
					q.offer(tempNode.left);
				}//if
				
				if (tempNode.right != null) {
					q.offer(tempNode.right);
				}//if
				
			}//while
			
		}//if
		
	}//level
	
	public void visitLeaves() {
		visitLeaves(root);
	}
	
	private void visitLeaves(TreeNode aNode) {
		
		if (aNode != null) {
			//resurse through to visit left to right 
			if(aNode.left != null && aNode.right != null) {
				visitLeaves(aNode.left);
				visitLeaves(aNode.right);
			}//if
			else {
				System.out.println(aNode.parrot.getName());
			}//else
			
		}//if
		
	}//visitLeaves
	
	private class TreeNode {
		
		//Private data fields
		private Parrot parrot;
		private TreeNode left;
		private TreeNode right;
		
		//Constructor 
		public TreeNode(Parrot parrot) {
			this.parrot = parrot;
			this.left = null;
			this.right= null;
		}
	}
}




