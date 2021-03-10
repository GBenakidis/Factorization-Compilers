#include "STNode.h"
#include <iostream>
#include <set>

STNode* g_root=nullptr;						// brings the root of the syntax tree and we declare it in STNode.h
int STNode::m_serialCounter = 0;
const char* g_nodeTypeLabels[] = {
	"COMPILEUNIT", "STATEMENTS", "STATEMENT",
	"NUMBER", "IDENTIFIER", "ADDITION", 
	"SUBTRACTION","MULTIPLICATION",
	"DIVISION", "ASSIGNMENT"
};

STNode::STNode(NodeType type) {
	m_nodeType = type;
	m_serial = m_serialCounter++;
	m_children = new list<STNode*>;
	m_parents = new list<STNode*>;

	m_graphvizLabel = _strdup(g_nodeTypeLabels[m_nodeType]);

	m_graphvizLabel.append("_" + std::to_string(m_serial));
}

STNode::~STNode() {
	list<STNode*>::iterator it;
	for (it = m_children->begin(); it != m_children->end(); it++) {
		delete* it;
	}
	delete m_children;
	delete m_parents;
}

void STNode::AddChild(STNode* node) {
	m_children->push_back(node);
	node->m_parents->push_back(this);
}

STNode* STNode::GetChild(int index) {
	list<STNode*>::iterator it = m_children->begin();
	advance(it, index);
	return *it;
}

STNode* STNode::GetParent(int index){
	list<STNode*>::iterator it = m_parents->begin();
	advance(it, index);
	return *it;
}

void STNode::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	list<STNode*>::iterator it; 

	// we print the link from father to the current node
	// "\"" -> to enclose links in double quotes because we create side effects when they exist in the names of the nodes
	// "\"  -> to request with double quotes the name of the parent node
	// \""  -> for the name of the current node that follows
	(*dotFile) << "\"" << parent->GetGraphvizLabel() << "\"->\"" << GetGraphvizLabel() << "\";\n";

	for (it = m_children->begin(); it != m_children->end(); it++) {
		//do not define postorder action except from the root
		(*it)->Visit_SyntaxTreePrinter(dotFile, this);
	}
}

bool flag_1 = true;
void STNode::Visit_FactorizedTree(list<STNode*>* node, ofstream* factfile, STNode* parent, int common_factor, list<int> all_nums) {
	// === !!! CAUTION !!! ==========================================
	// This function is a VERY beta version of printing the factorized tree.
	// In main.cpp, I've put it in comments cause it's not ready.
	// I was testing with WRONG inputs, just to check the output.
	// Didn't delete just to show the effort ..
	// ==============================================================
	list<STNode*>::iterator it;
	bool flag = true;
	int out = 0;
	(*factfile) << "\"" << parent->GetGraphvizLabel() << "\"->\"" << GetGraphvizLabel() << "\";\n";
	if (flag_1) {
		for (it = m_children->begin(); it != m_children->end(); it++) {
			if ((*it)->GetNodeType() != NT_EXPRESSION_ADDITION) {
				out++;
				(*it)->Visit_FactorizedTree(node, factfile, this, common_factor, all_nums);
				flag_1 = false;
			}
		}
	}
	list<STNode*>::iterator po = m_children->begin();

	if (out >= 1) {
		list<STNode*>::iterator ite = node->begin();
		(*factfile) << "\"" << (*po)->GetGraphvizLabel() << "\"->\"" << "MULTIPLICATION_X" << "\";\n";
		(*factfile) << "\"" << "MULTIPLICATION_X" << "\"->\"" << "NUMBER_0_" << common_factor << "\";\n";
		(*factfile) << "\"" << "MULTIPLICATION_X" << "\"->\"" << (*ite)->GetGraphvizLabel() << "\";\n";

		list<STNode*>::iterator child = node->begin();
		string str1, str2;
		int count = 14;
		for (auto const& i : all_nums) {
			(*factfile) << "\"" << "ADDITION_" << count << "\"->\"" << "NUMBER_" << count << "_" << i << "\";\n";
			count++;
		}
		out = 0;
	}

}

int STNode::Visit_Eval() {
	list<STNode*>::iterator it;
	int p;

	for (it = m_children->begin(); it != m_children->end(); it++) {
		p = (*it)->Visit_Eval();
	}
	return p;
}

list<STNode*> STNode::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it;
	list<STNode*> p;

	for (it = m_children->begin(); it != m_children->end(); it++) {
		p = (*it)->SearchingAddition(a);
	}
	return p;
}

list<STNode*> STNode::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it;
	list<STNode*> p;

	for (it = m_children->begin(); it != m_children->end(); it++) {
		p = (*it)->SearchingMultiplications(a,b);
	}
	return p;
}

list<int> STNode::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it;
	list<int> p;

	for (it = m_children->begin(); it != m_children->end(); it++) {
		p = (*it)->CommonFactor(a,b) ;
	}
	return p;
}

list<int> STNode::FindingNums(list<STNode*> a, list<STNode*> b , int c) {
	list<STNode*>::iterator it;
	list<int> p;

	for (it = m_children->begin(); it != m_children->end(); it++) {
		p = (*it)->FindingNums(a,b,c);
	}
	return p;
}

NodeType STNode::GetNodeType() {
	return m_nodeType;
}

string STNode::GetGraphvizLabel() {
	return m_graphvizLabel;
}