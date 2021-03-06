#pragma once
#include "SymbolNodes.h"
#include "SymbolTable.h"
#include <iostream>

// ======== ILOPOIHSEIS METHODWN ========

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

CAssignment::CAssignment() : STNode(NodeType::NT_EXPRESSION_ASSIGNMENT) {}
CAssignment::~CAssignment() {}

CNUMBER::CNUMBER(char *text): STNode(NodeType::NT_EXPRESSION_NUMBER) {

	// --- Edw pairnoun times/onoma oi komboi me noumera ---

	m_number = atoi(text);
	// kai episinaptoume ton arirhmo kanontas ton string
	// (text p fainetai panv ston kombo)
	m_graphvizLabel.append("_" + std::to_string(m_number));
}
CNUMBER::~CNUMBER() {}

CIDENTIFIER::CIDENTIFIER(char* text) : STNode(NodeType::NT_EXPRESSION_IDENTIFIER){
	m_name = (string)text;
	m_graphvizLabel.append("_" + m_name); // episinaptoume to onoma tis metablitis
	// (to arxiko onoma tou contructor orizetai mesa stin STNode.cpp)
}
CIDENTIFIER::~CIDENTIFIER() {

}

void CCompileUnit::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	list<STNode*>::iterator it;

	// --- Edw dimiourgoume ta .dot --
	// elegxoume an o kombos exei patera

	if (parent == nullptr) {
		(*dotFile) << "digraph G{ \n";
		for (it = m_children->begin(); it != m_children->end(); it++) {
			(*it)->Visit_SyntaxTreePrinter(dotFile, this); // edw mpainei stin STNode.cpp
		}
		(*dotFile) << "}";

		//kleinoume to arxeio
		dotFile->close();
	}
	else {
		// klisi tis methodo visit tis gonikis klasis
		STNode::Visit_SyntaxTreePrinter(dotFile, this);
	}
}

int CStatement::Visit_Eval() {
	list<STNode*>::iterator it;
	int nai;
	// exei 2 enalaktikes alla pairnoume tin pliri ekfrasi pou einai h: " expression ';' "
	if (m_children->size() != 0) { // elegxoume an i lista me ta paidia einai adeia
		// ton iterator ton arxikopoioume sto prwto stoixeio tis listas
		it = m_children->begin(); 
		nai = (*it)->Visit_Eval();
	}
	return nai;
}
int CAddition::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	// auksanoume ton iterator gia na deixnei sto epomeno paidi
	advance(it, 1);
	result += (*it)->Visit_Eval(); // prosthetoume ayto pou tha epistrepsei gia to DEUTERO paidi

	return result; // epistrefoume tin timi ston gonea
}
int CSubtraction::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result -= (*it)->Visit_Eval(); // afairoume ayto pou tha epistrepsei gia to DEUTERO paidi

	return result; // epistrefoume tin timi ston gonea
}
int CMultiplication::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result *= (*it)->Visit_Eval(); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	return result; // epistrefoume tin timi ston gonea
}
int CDivision::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result /= (*it)->Visit_Eval(); // diairoume ayto pou tha epistrepsei gia to DEUTERO paidi

	return result; // epistrefoume tin timi ston gonea
}
int CAssignment::Visit_Eval() {
	// prepei na ipologistei h ekfrasi pou brisketai sta aristera tis anathesis
	// kai i timi tis na anatethei ston SymbolTable

	// deixnei sto prwto stoixeio tis listas(prwto paidi) pou einai to identifier
	list<STNode*>::iterator it = m_children->begin();

	// to pairnoume to paidi kai to anathetoume se mia metabliti 
	// tipou identifier wste na mporoume na prospelasoume ta pedia tis klasis
	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	// dinoume onoma metablitis kai ton arithmo pou prokiptei apo thigatrikous kombous
	SetValue(id->m_name, (*it)->Visit_Eval());

	// minima pou mas anaferei oti auti i metabliti tropopoihthike
	cout << id->m_name << "=" << GetInitValue(id->m_name) << endl;

	return GetInitValue(id->m_name);
}

int CNUMBER::Visit_Eval() {
	return m_number;
}
int CIDENTIFIER::Visit_Eval() {
	return GetInitValue(m_name);
}


list<STNode*> CStatement::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it;
	list<STNode*> arr;
	if (m_children->size() != 0) {
		it = m_children->begin();
		arr = (*it)->SearchingAddition(a);
	}
	return arr;
}
list<STNode*> CAddition::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator par = m_parents ->begin();
	list<STNode*>temp;

	if((*par)->GetNodeType() != 2)
		a.push_back(*par);

	temp = (*it)->SearchingAddition(a);
	
	advance(it, 1);

	temp = (*it)->SearchingAddition(temp);
	
	return temp;
}
list<STNode*> CSubtraction::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	(*it)->SearchingAddition(a);

	advance(it, 1);
	(*it)->SearchingAddition(a); 

	return a;
}
list<STNode*> CMultiplication::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator par = m_parents->begin();
	list<STNode*>temp;

	bool brika_koino = false;
	for (auto const& i : a) {
		if (i->GetGraphvizLabel() == (*par)->GetGraphvizLabel()) {
			brika_koino = true;
		}
	}
	if(brika_koino==false)
		a.push_back(*par);

	temp = (*it)->SearchingAddition(a);

	advance(it, 1);
	temp = (*it)->SearchingAddition(a);

	return temp;
}
list<STNode*> CDivision::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	(*it)->SearchingAddition(a);

	advance(it, 1);
	(*it)->SearchingAddition(a);
	return a;
}
list<STNode*> CAssignment::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();

	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	cout << id->m_name << "=" << GetInitValue(id->m_name) << endl;

	return a;
}

list<STNode*> CNUMBER::SearchingAddition(list<STNode*> a) {
	// return m_number;
	return a;
}
list<STNode*> CIDENTIFIER::SearchingAddition(list<STNode*> a) {
	// return GetInitValue(m_name);
	return a;
}


list<STNode*> CStatement::SearchingMultiplication(list<STNode*> b) {
	list<STNode*>::iterator it;
	list<STNode*> arr;
	if (m_children->size() != 0) {
		it = m_children->begin();
		arr = (*it)->SearchingMultiplication(b);
	}
	return arr;
}
list<STNode*> CAddition::SearchingMultiplication(list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator par = m_parents->begin();
	list<STNode*>temp;

	if ((*it)->GetNodeType() == 7)
		b.push_back(*it);

	temp = (*it)->SearchingMultiplication(b);

	b = temp;
	advance(it, 1);

	if ((*it)->GetNodeType() == 7)
		b.push_back(*it);

	temp = b;

	temp = (*it)->SearchingMultiplication(temp);

	return temp;

}
list<STNode*> CSubtraction::SearchingMultiplication(list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	(*it)->SearchingMultiplication(b);

	advance(it, 1);
	(*it)->SearchingMultiplication(b);

	return b;
}
list<STNode*> CMultiplication::SearchingMultiplication(list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator par = m_parents->begin();
	list<STNode*>temp;

	temp = (*it)->SearchingAddition(b);

	advance(it, 1);

	temp = (*it)->SearchingAddition(temp);

	return temp;
}
list<STNode*> CDivision::SearchingMultiplication(list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	(*it)->SearchingMultiplication(b);

	advance(it, 1);
	(*it)->SearchingMultiplication(b);
	return b;
}
list<STNode*> CAssignment::SearchingMultiplication(list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();

	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	cout << id->m_name << "=" << GetInitValue(id->m_name) << endl;

	return b;
}

list<STNode*> CNUMBER::SearchingMultiplication(list<STNode*> b) {
	// return m_number;
	return b;
}
list<STNode*> CIDENTIFIER::SearchingMultiplication(list<STNode*> b) {
	// return GetInitValue(m_name);
	return b;
}
