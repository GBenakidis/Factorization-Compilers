#pragma once
#include "SymbolNodes.h"

CCompileUnit::CCompileUnit(): STNode(NodeType::NT_COMPILEUNIT) {} //tetrimeni ilopoiisi
CCompileUnit::~CCompileUnit(){}

CStatements::CStatements(): STNode(NodeType::NT_STATEMENTS) {} 
CStatements::~CStatements() {}

CStatement::CStatement(): STNode(NodeType::NT_STATEMENT) {}
CStatement::~CStatement() {}

CAddition::CAddition(): STNode(NodeType::NT_EXPRESSION_ADDITION) {} 
CAddition::~CAddition() {}

CSubtraction::CSubtraction(): STNode(NodeType::NT_EXPRESSION_SUBTRACTION) {}
CSubtraction::~CSubtraction() {}

CMultiplication::CMultiplication(): STNode(NodeType::NT_EXPRESSION_MULTIPLICATION) {}
CMultiplication::~CMultiplication() {}

CDivision::CDivision(): STNode(NodeType::NT_EXPRESSION_DIVISION) {}
CDivision::~CDivision() {}

CNUMBER::CNUMBER(char *text): STNode(NodeType::NT_EXPRESSION_NUMBER) {
	// metatrepoume text->akeraio
	m_number = atoi(text);
	// kai episinaptoume ton arirhmo kanontas ton string
	m_graphvizLabel.append("_" + std::to_string(m_number));
}
CNUMBER::~CNUMBER() {}

void CCompileUnit::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	list<STNode*>::iterator it;

	//elegxoume an o kombos exei patera
	if (parent == nullptr) {
		(*dotFile) << "digraph G{ \n";
		for (it = m_children->begin(); it != m_children->end(); it++) {
			(*it)->Visit_SyntaxTreePrinter(dotFile,this);
		}
		// epilogos arxeiou
		(*dotFile) << "}";
		
		//kleinoume to arxeio
		dotFile->close();
	}
	else {
		// klisi tis methodo visit tis gonikis klasis
		STNode::Visit_SyntaxTreePrinter(dotFile, this);
	}
}