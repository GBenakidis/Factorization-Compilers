#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "STNode.h"
#include "SymbolNodes.h"

using namespace std;

#include "Grammar.tab.h"
extern FILE* yyin;

int main(int argc, char* argv[]) {

	fopen_s(&yyin, "test.txt", "r");

	yy::parser* p = new yy::parser();
	p->parse();

	ofstream *f = new ofstream("stree.dot", ofstream::out);
	g_root->Visit_SyntaxTreePrinter(f, nullptr);

	// -Tgif -> gia na paragei arxeio eikonas
	// -o -> output arxeio
	system("dot -Tgif stree.dot -o stree.dot.gif");
	
	return 0;
}