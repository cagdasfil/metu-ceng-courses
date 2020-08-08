import java.util.ArrayList;
public class LibraryTree {
	
	public LibraryNode primaryRoot;		//root of the primary B+ tree
	public LibraryNode secondaryRoot;	//root of the secondary B+ tree
	public LibraryTree(Integer order)
	{
		LibraryNode.order = order;
		primaryRoot = new LibraryNodeLeaf(null);
		primaryRoot.level = 0;
		secondaryRoot = new LibraryNodeLeaf(null);
		secondaryRoot.level = 0;
	}
	
	public void addBook(CengBook book) {
		
		// add methods to fill both primary and secondary tree
		addBookPrimary(book);
	}
	
	
	
	public CengBook searchBook(Integer key) {
		
		// add methods to find the book with the searched key in primary B+ tree
		// return value will not be tested, just print as the specicifications say
		searchBookPrimary(primaryRoot, key);
		return null;
	}
	
	
	public void printPrimaryLibrary() {
		
		// add methods to print the primary B+ tree in Depth-first order
		if(primaryRoot.getType() == LibraryNodeType.Leaf) {
			printLeaf((LibraryNodeLeaf)primaryRoot);
			return;
		}
		printPrimaryLibraryHelper((LibraryNodePrimaryIndex)primaryRoot);
		
	}
	
	public void printSecondaryLibrary() {
		
		// add methods to print the secondary B+ tree in Depth-first order
		
	}
	
	// Extra functions if needed
	
	private void addBookPrimary(CengBook book) {
		
		if(primaryRoot.type == LibraryNodeType.Leaf) {
			int i;
			LibraryNodeLeaf leaf = (LibraryNodeLeaf)primaryRoot;
			if(leaf.bookCount() == 0) {
				leaf.addBook(0, book);
			}
			else {
				for(i=0; i < leaf.bookCount(); i++) {
					if(book.key() < leaf.bookKeyAtIndex(i)) {
						leaf.addBook(i, book);
						break;
					}
				}
				if(leaf.bookCount() == i) {
					leaf.addBook(i, book);
				}
			}
			
			if(leaf.bookCount() > LibraryNode.order*2){
				LibraryNodePrimaryIndex root = new LibraryNodePrimaryIndex(null);
				LibraryNodeLeaf newleaf1 = new LibraryNodeLeaf(root);
				LibraryNodeLeaf newleaf2 = new LibraryNodeLeaf(root);
				for(int j=0; j < LibraryNode.order ;j++) {
					newleaf1.addBook(j, leaf.bookAtIndex(j));
				}
				for(int j=LibraryNode.order; j < LibraryNode.order*2 + 1 ; j++) {
					newleaf2.addBook(j-LibraryNode.order, leaf.bookAtIndex(j));
				}
				root.addKey(newleaf2.bookKeyAtIndex(0));
				root.addChild(0, newleaf1);
				root.addChild(1, newleaf2);
				primaryRoot = root;
			}
		}
		
		else{
			LibraryNodeLeaf insertionleaf = (LibraryNodeLeaf)findInsertionLeaf((LibraryNodePrimaryIndex)primaryRoot, book.key());
			for(int i=0; i < insertionleaf.bookCount(); i++) {
				if(book.key() < insertionleaf.bookKeyAtIndex(i)) {
					insertionleaf.addBook(i, book);
					break;
				}
				else if(i == insertionleaf.bookCount()-1) {
					insertionleaf.addBook(i+1, book);
					break;
				}
			}
			
			if(insertionleaf.bookCount() > 2*LibraryNode.order) {
				LibraryNodePrimaryIndex root = (LibraryNodePrimaryIndex)insertionleaf.getParent();
				LibraryNodeLeaf newleaf1 = new LibraryNodeLeaf(root);
				LibraryNodeLeaf newleaf2 = new LibraryNodeLeaf(root);
				int insertindex=0;
				for(int j=0; j < LibraryNode.order ;j++) {
					newleaf1.addBook(j, insertionleaf.bookAtIndex(j));
				}
				for(int j=LibraryNode.order; j < LibraryNode.order*2 + 1 ; j++) {
					newleaf2.addBook(j-LibraryNode.order, insertionleaf.bookAtIndex(j));
				}
				//printLeaf(newleaf1);
				//printLeaf(newleaf2);
				root.addKey(newleaf2.bookKeyAtIndex(0));
				for(int i=0; i<root.keyCount(); i++) {
					if(root.keyAtIndex(i) == newleaf2.bookKeyAtIndex(0)) {
						insertindex = i;
						break;
					}
				}
				
				for(int i=0; i<root.keyCount(); i++) {
					if(root.getChildrenAt(i) == insertionleaf) {
						root.setChild(i, newleaf2);
					}
				}
				
				root.addChild(root.keyCount(), root.getChildrenAt(root.keyCount()-1));
				for(int i=root.keyCount(); i>insertindex; i--) {
					root.setChild(i, root.getChildrenAt(i-1));
				}
				root.setChild(insertindex, newleaf1);
				//printPrimaryLibraryHelper((LibraryNodePrimaryIndex)primaryRoot);
				splitUpPrimary(root);
			}
			else {
				
			}
		}
		
		
	}
	
	public void printPrimaryLibraryHelper(LibraryNodePrimaryIndex root) {
		System.out.println("<index>");
		for(int i=0; i<root.keyCount();i++) {
			System.out.println(root.keyAtIndex(i));
		}
		System.out.println("</index>");
		
		if(root.getChildrenAt(0).getType() == LibraryNodeType.Leaf) {
			ArrayList<LibraryNode> children = root.getAllChildren();
			for(int i=0; i < children.size() ;i++) {
				printLeaf((LibraryNodeLeaf)children.get(i));
			}
		}
		else {
			ArrayList<LibraryNode> children = root.getAllChildren();
			for(int i=0; i < children.size() ;i++) {
				printPrimaryLibraryHelper((LibraryNodePrimaryIndex)children.get(i));
			}
		}
	}

	private void printLeaf(LibraryNodeLeaf root) {
		System.out.println("<data>");
		for(int i=0; i < root.bookCount(); i++) {
			System.out.println("<record>" + root.bookAtIndex(i).fullName() + "</record>");
		}
		System.out.println("</data>");
	}
	
	private LibraryNode findInsertionLeaf(LibraryNodePrimaryIndex root, int key) {
		int i;
		for(i=0 ; i < root.keyCount(); i++) {
			if(key < root.keyAtIndex(i)) {
				if(root.getChildrenAt(i).getType() == LibraryNodeType.Leaf) {
					return root.getChildrenAt(i);
				}
				else {
					return findInsertionLeaf((LibraryNodePrimaryIndex)root.getChildrenAt(i), key);
				}
			}
		}
		if(root.getChildrenAt(i).getType() == LibraryNodeType.Leaf) {
			return root.getChildrenAt(i);
		}
		else {
			return findInsertionLeaf((LibraryNodePrimaryIndex)root.getChildrenAt(i), key);
		}
	}
	
	private void splitUpPrimary(LibraryNodePrimaryIndex root) {
		if(root.keyCount() > LibraryNode.order*2) {
			LibraryNodePrimaryIndex newroot = (LibraryNodePrimaryIndex)root.getParent();
			if(newroot == null) {
				LibraryNodePrimaryIndex creatednewroot = new LibraryNodePrimaryIndex(null);
				LibraryNodePrimaryIndex newprim1 = new LibraryNodePrimaryIndex(creatednewroot);
				LibraryNodePrimaryIndex newprim2 = new LibraryNodePrimaryIndex(creatednewroot);
				int insertindex;
				for(int j=0; j < LibraryNode.order ;j++) {
					newprim1.addKey(root.keyAtIndex(j));
					newprim1.addChild(j, root.getChildrenAt(j));
					root.getChildrenAt(j).setParent(newprim1);
				}
				newprim1.addChild(LibraryNode.order, root.getChildrenAt(LibraryNode.order));
				root.getChildrenAt(LibraryNode.order).setParent(newprim1);
				for(int j=LibraryNode.order+1; j < LibraryNode.order*2+1 ; j++) {
					newprim2.addKey(root.keyAtIndex(j));
					newprim2.addChild(j-LibraryNode.order-1, root.getChildrenAt(j));
					root.getChildrenAt(j).setParent(newprim2);
				}
				newprim2.addChild(LibraryNode.order, root.getChildrenAt(LibraryNode.order*2+1));
				root.getChildrenAt(LibraryNode.order*2+1).setParent(newprim2);
				insertindex = creatednewroot.addKey(root.keyAtIndex(LibraryNode.order));
				
				creatednewroot.addChild(0, newprim2);
				creatednewroot.addChild(0, newprim1);
				
				primaryRoot = creatednewroot;
			}
			else {
				LibraryNodePrimaryIndex newprim1 = new LibraryNodePrimaryIndex(newroot);
				LibraryNodePrimaryIndex newprim2 = new LibraryNodePrimaryIndex(newroot);
				int insertindex;
				for(int j=0; j < LibraryNode.order ;j++) {
					newprim1.addKey(root.keyAtIndex(j));
					newprim1.addChild(j, root.getChildrenAt(j));
					root.getChildrenAt(j).setParent(newprim1);
				}
				newprim1.addChild(LibraryNode.order, root.getChildrenAt(LibraryNode.order));
				root.getChildrenAt(LibraryNode.order).setParent(newprim1);
				for(int j=LibraryNode.order+1; j < LibraryNode.order*2+1 ; j++) {
					newprim2.addKey(root.keyAtIndex(j));
					newprim2.addChild(j-LibraryNode.order-1, root.getChildrenAt(j));
					root.getChildrenAt(j).setParent(newprim2);
				}
				newprim2.addChild(LibraryNode.order, root.getChildrenAt(LibraryNode.order*2+1));
				root.getChildrenAt(LibraryNode.order*2+1).setParent(newprim2);
				insertindex = newroot.addKey(root.keyAtIndex(LibraryNode.order));
				
				for(int i=0; i<newroot.keyCount(); i++) {
					if(newroot.getChildrenAt(i) == root) {
						newroot.setChild(i, newprim2);
					}
				}
			
				newroot.addChild(newroot.keyCount(), newroot.getChildrenAt(newroot.keyCount()-1));
				for(int i=newroot.keyCount(); i>insertindex; i--) {
					newroot.setChild(i, newroot.getChildrenAt(i-1));
				}
				newroot.setChild(insertindex, newprim1);
				splitUpPrimary(newroot);
			}
		}
	}
	private void searchBookPrimary(LibraryNode root, int key) {
		if(root.type == LibraryNodeType.Leaf) {
			searchBookLeaf((LibraryNodeLeaf)root,key);
			return;
		}
		else {
			LibraryNodePrimaryIndex t = (LibraryNodePrimaryIndex)root;
			System.out.println("<index>");
			for(int i=0; i<t.keyCount(); i++) {
				System.out.println(t.keyAtIndex(i));
			}
			System.out.println("</index>");
			for(int i=0; i < t.keyCount(); i++) {
				if(key < t.keyAtIndex(i)) {
					searchBookPrimary(t.getChildrenAt(i),key);
					break;
				}
				else if(t.keyCount()-1 == i) {
					searchBookPrimary(t.getChildrenAt(i+1),key);
				}
			}
			
		}
	}
	private void searchBookLeaf(LibraryNodeLeaf root, int key) {
		for(int i=0; i<root.bookCount(); i++) {
			if(root.bookKeyAtIndex(i) == key) {
				System.out.println("<data>");
				System.out.println("<record>" + root.bookAtIndex(i).fullName() + "</record>");
				System.out.println("</data>");
			}
		}
	}
}
