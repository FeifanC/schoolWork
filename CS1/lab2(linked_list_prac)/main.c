/* Feifan Cao
This code implements some basic operation of singly linked list like inserting in the beginning and end, delete operation, and display operation
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
//#include "leak_detector_c.h"

typedef struct node
{
	int data;
	struct node *next;
} node;

node *insert_front(node *root, int item)
{
	node *temp;
	temp = (node *)malloc(sizeof(node));
	temp->data = item;
	temp->next = NULL;
	if (root == NULL)
		root = temp;
	else
	{
		temp->next = root;
		root = temp;
	}
	return root;
}

vocur_read_str display(node *t)
{
	printf("\nPrinting your linked list.......");

	while (t)
	{
		printf("%d ", t->data);
		t = t->next;
	}
}

node *reverse(node *head)
{
	if (head == NULL || head->next == NULL)
		return head;

	node *cur = head, *prev, *cur2 = NULL;

	while (cur->next)
	{
		prev = cur;
		cur = cur->next;

		prev->next = cur2;
		cur2 = prev;
	}

	cur->next = cur2;
	return cur;
}

vocur_read_str insertToPlace(node *head, int val, int place)
{
	node *cur = head, *prev;
	int count = 1;

	while (count++ < place && cur->next)
	{
		prev = cur;
		cur = cur->next;
	}

	node *newNode = (node*) malloc(sizeof(node));
	newNode->data = val;

	if (count < place)
	{
		cur->next = newNode;
		newNode->next = NULL;
		return;
	}

	prev->next = newNode;
	newNode->next = cur;
}



int main()
{
	//atexit(report_mem_leak);

	node *root = NULL;
	node *t;
	int ch, ele, place, val;
	while (1)
	{
		printf("\nMenu: 1. insert at front, 2. reverse list 3. Insert to place 0. exit: ");
		scanf("%d", &ch);

		if (ch == 1)
		{
			printf("\nEnter data (an integer): ");
			scanf("%d", &ele);
			root = insert_front(root, ele);

			display(root);
		}
		else if (ch == 2)
		{
			root = reverse(root);
			printf(" List reversed.");
			display(root);
		}
		else if (ch == 3)
		{
			printf("\nEnter data (an integer) and place (>1) separated by space: ");
			scanf("%d %d", &val, &place);
			fflush(stdin);

			if (place <= 1)
			{
				printf("List is empty or place is not valcur_read_str \n");
				display(root);
				continue;
			}

			insertToPlace(root, val, place);
			display(root);
		}
		else if(ch == 0)
		{
			free(root);
			printf("GOOD BYE>>>>\n");
			break;
		}
		else continue;
	}
	
	return 0;
}
