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

	//===== 1o/2o perasma: dimiourgei dentro gia tin eisodo pou tou dwsame =====

	ofstream *f = new ofstream("stree.dot", ofstream::out);
	g_root->Visit_SyntaxTreePrinter(f, nullptr); // phgainei stin symbolnodes.cpp


	// -Tgif -> gia na paragei arxeio eikonas
	// -o -> output arxeio
	system("dot -Tgif stree.dot -o stree.dot.gif");

	// ===== 3o perasma: sinartisi pou briskei ta athroismata ===

	list<STNode*> additions; // periexei oles tis prostheseis poy exoume sto dentro
	additions = g_root -> SearchingAddition(additions);

	//================================================================

	// ===== krataei sthn lista all_add mono tous komvous add twn athroismatwn ginomenwn ====

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

	/*
	cout << "ALL ADD LIST _  __ :"<< endl;
	for (auto const& i : all_add) {
		STNode* paidi_1 = i->GetChild(0)->GetChild(1), * paidi_2 = i->GetChild(1);
		cout << "EIMAI TO ADDITION --> " <<i->GetGraphvizLabel() << endl;
		cout << "TA PAIDIA MOU EINAI --> PAIDI 1 " << paidi_1->GetGraphvizLabel() << " KAI PAIDI 2 " << paidi_2->GetGraphvizLabel() << endl;
		cout << "TA PAIDIA TOU 1 EINAII -..>->>> " << paidi_1->GetChild(0)->GetGraphvizLabel()<< " KAI 2 " << paidi_1->GetChild(1)->GetGraphvizLabel() <<  endl;
		cout << "TA PAIDIA TOU 1 EINAII -..>->>> " << paidi_2->GetChild(0)->GetGraphvizLabel() << " KAI 2 " << paidi_2->GetChild(1)->GetGraphvizLabel() << endl;
	}
	*/
	//cout << "To plithos twn athrismatvn ginomenwn einai " << all_add.size() << endl;

	cout << "OLA TA ATHROISMATA GINOMENWN: " << endl;
	for (auto const& i : all_add) {
		cout << i->GetGraphvizLabel() << endl ;
	}

	//===================================================================================
	
	//======= 4o perasma: epistrefw tous pollaplasiasmous pou einai mesa sta addition =========

	list<STNode*> multiplications; // periexei oles tis prostheseis poy exoume sto dentro
	multiplications = g_root->SearchingMultiplications(all_add, multiplications );
	cout << "\nTA MULTIPLICATIONS TWN ATHROISMATWN GINOMENWN: "<< endl;
	for (auto const& i : multiplications) {
		cout  << i->GetGraphvizLabel() << endl;
	}

	//=========================================================================================

	//=== 5o perasma: euresi koinou paragonta ========================================================

	list<int> lista_koinos_paragontas;
	lista_koinos_paragontas = g_root->CommonFactor(all_add);
	int common_factor = lista_koinos_paragontas.front();

	cout << "O KOINOS PARAGONTAS: " << common_factor << endl;

	//===================================================================================

	//======= 6o perasma: briskoume ta noumera twn athroismatwn ginomenwn kai ta apothikeuoume se lista me thn seira ==

	/*
	list<int> all_nums;
	all_nums = g_root->FindingNums(all_add);
	cout << "OLOI OI ARITHMOI EINAI: " << endl;
	for (auto const& i : all_nums) {
		cout << i << endl;
	}
	*/

	//=================================================================================================================

	//======== 6o perasma: dimiourgia tou paragontopoihmenou dentroy ==================

	//ofstream* k = new ofstream("stree_factor.dot", ofstream::out);
	//g_root->Visit_FactorizedTree(&all_add, k, nullptr, common_factor);

	//system("dot -Tgif stree_factor.dot -o stree_factor.dot.gif");

	//=================================================================================


	//===== teleutaio perasma: ektelesi praksewn panw sto dentro =====

	int nai;
	nai = g_root->Visit_Eval();
	cout << "Result of the operations: " << nai << endl;

	//================================================================

	
	return 0;
}