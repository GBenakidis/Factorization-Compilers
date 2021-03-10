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
	additions = g_root -> SearchingAddition(additions);

	list<STNode*> to_delete;
	for (auto const& i : additions) {
		STNode* paidi_1 = i->GetChild(0)->GetChild(1), * paidi_2 = i->GetChild(1);
		if ((paidi_1->GetNodeType() != NT_EXPRESSION_MULTIPLICATION) || (paidi_2->GetNodeType() != NT_EXPRESSION_MULTIPLICATION)) {
			to_delete.push_back(i);
		}
	}

	list<STNode*> all_add;
	int additions_size= additions.size();
	bool comm = false;
	for (auto const& i : additions) {
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
	multiplications = g_root->SearchingMultiplications(all_add, multiplications );
	cout << "\nMULTIPLIATIONS OF SUM PRODUCTS: "<< endl;
	for (auto const& i : multiplications) {
		cout  << i->GetGraphvizLabel() << endl;
	}

	list<int> lista_koinos_paragontas;
	lista_koinos_paragontas = g_root->CommonFactor(all_add, multiplications);
	int common_factor = lista_koinos_paragontas.front();

	cout << "\nCOMMON FACTOR: " << common_factor << endl;


	list<int> all_nums;
	all_nums = g_root->FindingNums(all_add, multiplications, common_factor);
	cout << "\nALL NUMBERS THAT NEED TO PUT IN FACTORIZED TREE: " << endl;
	for (auto const& i : all_nums) {
		cout << i << endl;
	}

	//ofstream* k = new ofstream("stree_factor.dot", ofstream::out);
	//g_root->Visit_FactorizedTree(&all_add, k, nullptr, common_factor);

	//system("dot -Tgif stree_factor.dot -o stree_factor.dot.gif");

	int nai;
	nai = g_root->Visit_Eval();
	cout << "\nResult of the operations: " << nai << endl;

	
	return 0;
}