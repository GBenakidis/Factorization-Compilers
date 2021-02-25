#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "STNode.h"
#include "SymbolNodes.h"
#include "Grammar.tab.h"
using namespace std;
extern FILE* yyin;

int main(int argc, char* argv[]) {

	fopen_s(&yyin, "test.txt", "r");

	yy::parser* p = new yy::parser();
	p->parse();

	//===== dimiourgei dentro gia tin eisodo pou tou dwsame =====

	ofstream *f = new ofstream("stree.dot", ofstream::out);
	g_root->Visit_SyntaxTreePrinter(f, nullptr); // phgainei stin symbolnodes.cpp

	// -Tgif -> gia na paragei arxeio eikonas
	// -o -> output arxeio
	system("dot -Tgif stree.dot -o stree.dot.gif");

	//=============================================================

	//===== perasma tou dentrou pou kanei tis prakseis =====

	g_root->Visit_Eval();

	//======================================================
	
	return 0;
}