#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler profiler("OSTrees");

#define CONTOR 10
int asig = 0;
int comp = 0;

typedef struct node {
	int key;
	int index;
	struct node* left, * right;
}NodeT;


NodeT* newNode(int data, int data1)
{
	NodeT* node = new NodeT();
	node->key = data;
	node->index = data1;
	node->left = NULL;
	node->right = NULL;

	return node;
}

NodeT* build( int* a, int left, int right, int* k) {
	if (left > right) {
		return NULL;
	}
	*k = right - left + 1;
	double mid1 = (left + right) / 2.0;
	int mid = round(mid1);
	NodeT* root = newNode(a[mid], *k);
	root->left = build(a, left, mid - 1, k);
	root->right = build(a, mid + 1, right, k);
	return root;
}

NodeT* minArbore(NodeT* p)
{

	while (p->left != NULL)
	{
		p = p->left;
	}
	return p;
}


void inorder(NodeT* p)
{
	if (p == NULL)
		return;
	else{
		inorder(p->left);
		printf("%d:%d  ", p->key, p->index);
		inorder(p->right);
	}
}

void printArr(int* a, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d " , a[i]);
	}
}

void preetyPrint(NodeT* root, int space)
{
  
	if (root == NULL)
		return;
	space += CONTOR;
	preetyPrint(root->right, space);

	printf("\n");
	for (int i = CONTOR; i < space; i++)
		printf(" ");
	printf("%d \n", root->key);

	preetyPrint(root->left, space);
}


NodeT* osSelect(NodeT* root, int poz) {

	NodeT* aux = root->left;
	int r;
	comp++;
	if (aux == NULL) {
		r = 1;
	}
	else {
		r = aux->index + 1;
	}
		
	if (poz == r) {
		return root;
	}
		
	else {
		if (poz < r) {
			comp++;
			if (root->left != NULL)
				return osSelect(root->left, poz);
			else
				return root;
		}
		else
			return osSelect(root->right, poz - r);
	}
}


NodeT* osDelete(NodeT* root, int key)
{
	root->index--;
	comp++;
	if (root == NULL) return root;
	if (key < root->key) {
		asig++;
		root->left = osDelete(root->left, key);
	}

	else if (key > root->key) {
		asig++;
		root->right = osDelete(root->right, key);
	}

	else
	{
		comp++;
		if (root->left == NULL)
		{
			NodeT* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL)
		{
			comp++;
			NodeT* temp = root->left;
			free(root);
			return temp;
		}
		asig += 2;
		NodeT* temp =minArbore(root->right);
		root->key = temp->key;
		root->right = osDelete(root->right, temp->key);
	}
	return root;
}

void demo(int n, int* arr, int select, int sterge) {
	
	NodeT* tree;
	int k = n;
	tree = build(arr, 0, n - 1, &k);
	printf("Parcurgerea arborelui in inordine este : \n");
	inorder(tree);
	printf("\nAfisarea arborelui : \n");
	preetyPrint(tree, 0);
	NodeT* n1 = osSelect(tree, select);
	printf("\nNodul de pe pozitia %d este %d\n\n\n" , select, n1->key);
	osDelete(tree, sterge);
	printf("Dupa stegere, arborele arata asa: \n");
	preetyPrint(tree, 0);
	printf("\n\nIar parcurgerea lui in inordine este aceasta: ");
	inorder(tree);

}


int main(void) {
	
/*
	int n = 10;
	int arr[10] = {2,5,6,16,17,18,20,21,26,30};
	demo(n, arr);
*/
	

	int a[10000];
	NodeT* tree;
	int randnumber;
	for (int n = 100; n < 10000; n += 100) {
	int k = n;
	FillRandomArray(a, n, 0, 15000, true, 1);
		tree = build(a, 0, n - 1, &k);
		for (int i = n; i > 0; i--) {
			randnumber = (rand() % i) + 1;
			NodeT* aux = osSelect(tree,randnumber);
			osDelete(tree, aux->key);
		}
		profiler.countOperation("comp", n, comp);
		profiler.countOperation("asig", n, asig);
     } 
	profiler.addSeries("Efort", "comp", "asig");

	//inorder(tree);
	profiler.showReport();
	return 0;
}
