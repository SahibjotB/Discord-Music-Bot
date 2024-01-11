#include <iostream>
#include "header.h"

/*
#include "node.h"
#include "queue.h"
*/

int main(){
	queue q;

	node *node1 = new node();
	node1 = (node *) malloc (sizeof(node));
	node1->setData("Test1");
	node1->setID(1);
	
	q.addfront(node1);

	node *node2 = new node();
	node2 = (node *) malloc (sizeof(node));
	node2->setData("Test2");
	node2->setID(2);

	q.addfront(node2);

	node *node3 = new node();
	node3 = (node *) malloc (sizeof(node));
	node3->setData("Test3");
	node3->setID(3);

	q.addback(node3);

	node *node4 = new node();
	node4 = (node *) malloc (sizeof(node));
	node4->setData("Test4");
	node4->setID(4);

	q.addfront(node4);

	node *node5 = new node();
        node5 = (node *) malloc (sizeof(node));
        node5->setData("Test5");
        node5->setID(5);

	q.addback(node5);

	std::cout << "\nPRINT TEST 1" << std::endl;
	std::cout << q.display() << std::endl;

	bool tri = q.remove(3);

	std::cout << "\nPRINT TEST 2 | TRY = " << tri << std::endl;
	std::cout << q.display() << std::endl;

	tri = q.remove(4);

        std::cout << "\nPRINT TEST 3 | TRY = " << tri << std::endl;
	std::cout << q.display() << std::endl;

	tri = q.pop();

	std::cout << "\nPOP TEST 4 | TRY = " << tri << std::endl;
	std::cout << q.display() << std::endl;
	tri = q.pop();
	std::cout << "\nPOP TEST 4 | TRY = " << tri << std::endl;
	        std::cout << q.display() << std::endl;
	tri = q.pop();
	std::cout << "\nPOP TEST 4 | TRY = " << tri << std::endl;
	        std::cout << q.display() << std::endl;
	tri = q.pop();

	 std::cout << "\nPOP TEST 4 | TRY = " << tri << std::endl;
	                 std::cout << q.display() << std::endl;
			        

	return 0;
}
