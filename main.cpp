#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "STNode.h"
#include "SymbolNodes.h"
#include "Grammar.tab.h"
#include <set>
using namespace std;
extern FILE* yyin;

int main(int argc, char* argv[]) {

	fopen_s(&yyin, "test.txt", "r");

	yy::parser* p = new yy::parser();
	p->parse();

	ofstream *f = new ofstream("stree.dot", ofstream::out);
	g_root->Visit_SyntaxTreePrinter(f, nullptr);
	system("dot -Tgif stree.dot -o stree.dot.gif");

	list<STNode*> additions;
	additions = g_root -> SearchingAddition(additions);	// saving all additions

	list<STNode*> to_delete;
	for (auto const& i : additions) {					// checkes which of them have multiplication
		STNode* child_1 = i->GetChild(0)->GetChild(1), * child_2 = i->GetChild(1);
		if ((child_1->GetNodeType() != NT_EXPRESSION_MULTIPLICATION) || (child_2->GetNodeType() != NT_EXPRESSION_MULTIPLICATION)) {
			to_delete.push_back(i);
		}
	}

	list<STNode*> all_add;
	int additions_size= additions.size();
	bool comm = false;
	for (auto const& i : additions) {					// push in a new list (all_add) additions sum of products
		for (auto const& j : to_delete) {
			if (i == j) { comm = true; }
		}
		if (!comm) {
			all_add.push_back(i);
		}
		comm = false;
	}

	cout << "ALL SUM OF PRODUCTS: " << endl;
	for (auto const& i : all_add) {
		cout << i->GetGraphvizLabel() << endl ;
	}

	list<STNode*> multiplications;
	multiplications = g_root->SearchingMultiplications(all_add, multiplications );	// saving multiplications from sum of products 
	cout << "\nMULTIPLIATIONS OF SUM PRODUCTS: "<< endl;
	for (auto const& i : multiplications) {
		cout  << i->GetGraphvizLabel() << endl;
	}

	list<int> common_factor_list;
	common_factor_list = g_root->CommonFactor(all_add, multiplications);		// returns common factor
	int common_factor = common_factor_list.front();		// because common factor "lands" on top of the list

	cout << "\nCOMMON FACTOR: " << common_factor << endl;


	list<int> all_nums;
	all_nums = g_root->FindingNums(all_add, multiplications, common_factor);		// returns list of numbers from sum of products
	cout << "\nALL NUMBERS THAT NEED TO PUT IN FACTORIZED TREE: " << endl;			// ( not common factor )
	for (auto const& i : all_nums) {
		cout << i << endl;
	}

	//ofstream* k = new ofstream("stree_factor.dot", ofstream::out);
	//g_root->Visit_FactorizedTree(&all_add, k, nullptr, common_factor, all_nums);	// printing factorized tree

	//system("dot -Tgif stree_factor.dot -o stree_factor.dot.gif");

	int res;
	res = g_root->Visit_Eval();														// prints the result of the equation
	cout << "\nResult of the operations: " << res << endl;

	return 0;
}