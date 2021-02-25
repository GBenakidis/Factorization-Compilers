#include "SymbolTable.h"

// dilwnoumr metabliti tou pianak simbolwn
map<string, int> g_symbolTable;

int GetInitValue(string name) {
	map<string, int>::iterator it;
	int value = 0;

	it = g_symbolTable.find(name);
	if (it != g_symbolTable.end()) { // se periptwsi pou i metabliti toy simbolou den iparxei ston pinaka simbolwn
		value = g_symbolTable[name];
	}
	else {
		g_symbolTable[name] = 0;
	}
	return value;
}

void SetValue(string name, int value) {
	g_symbolTable[name] = value;
}