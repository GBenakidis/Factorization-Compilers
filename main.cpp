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

	// KALITERI EKDOSI
	// ===== 3o perasma: sinartisi pou briskei ta athroismata ===

	list<STNode*> all_add; // periexei oles tis prostheseis poy exoume sto dentro
	all_add = g_root -> SearchingAddition(all_add);

	//================================================================

	// ===== krataei sthn lista all_add mono tous komvous add pou exoyn paidi pollaplasiasmo ====

	int num_of_pops = 0;
	for (auto const& i : all_add) {
		STNode* kappe = i->GetChild(0), * kappos = i->GetChild(1);
		if ((kappe->GetNodeType() != 7) && (kappos->GetNodeType() != 7)) {
			num_of_pops++;
		}
	}
	for (int i = 0; i < num_of_pops; i++) {
		all_add.pop_front();
	}

	cout << "To plithos twn athrismatvn ginomenwn einai " << all_add.size() << endl;
	//===================================================================================
	
	//=== 4o perasma: euresi koinou paragonta, ekmetaleuontas tous pinakes all_add kai all_multiple ===
	list<int> koinos_paragontas;
	koinos_paragontas = g_root->CommonFactor(all_add);
	cout << "O koinos paragontas: " ;
	for (auto const& i : koinos_paragontas) {
		std::cout << i << "\n";
	}
	cout << "\n";

	//=================================================================================================

	//===== teleutaio perasma: ektelesi praksewn panw sto dentro =====

	int nai;
	nai = g_root->Visit_Eval();
	cout << "Result of the operations: " << nai << endl;

	//================================================================

	
	return 0;
}