#pragma once
#include <list>
#include <string>
#include <fstream>
using namespace std;

typedef enum nodeType {
	NT_COMPILEUNIT, NT_STATEMENTS, NT_STATEMENT,
	NT_EXPRESSION_NUMBER, NT_EXPRESSION_IDENTIFIER, NT_EXPRESSION_ADDITION,
	NT_EXPRESSION_SUBTRACTION, NT_EXPRESSION_MULTIPLICATION,
	NT_EXPRESSION_DIVISION, NT_EXPRESSION_ASSIGNMENT
} NodeType;

class STNode {
public:
	STNode(NodeType type);
	virtual ~STNode();

	NodeType GetNodeType();
	string GetGraphvizLabel();
	STNode* GetChild(int index);
	STNode* GetParent(int index = 0);

	void AddChild(STNode* node);

	virtual void Visit_SyntaxTreePrinter(ofstream* dotfile, STNode* parent);

	virtual list<STNode*> SearchingAddition(list<STNode*>);

	virtual int Visit_Eval();

protected:
	NodeType m_nodeType;
	string m_graphvizLabel;
	int m_serial;
	static int m_serialCounter;
	list<STNode*>* m_children;
	list<STNode*>* m_parents;

};


extern STNode* g_root; //dilwsi