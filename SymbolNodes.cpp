#pragma once
#include "SymbolNodes.h"
#include "SymbolTable.h"
#include <iostream>
#include <set>
#include <algorithm>

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

bool locked = false;
list<int> CStatement::CommonFactor(list<STNode*> a) {
	list<STNode*>::iterator it;
	list<int> arr;

	if (m_children->size() != 0) { 
		it = m_children->begin();
		arr= (*it)->CommonFactor(a);
	}
	return arr;
}
list<int> CAddition::CommonFactor(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator em = m_children->begin();
	list<STNode*> edw;
	list<int> result1, result2;

	int a_len = a.size();

	int cnt = 0;
	for (auto const& i : a) { // psaxnoume an o kombos pou briskomaste einai mesa stin lista me ta athroismata ginomenwn
		if (m_graphvizLabel != i->GetGraphvizLabel()) {
			cnt++;
		}
	}
	if (cnt == a_len) { // an den einai pame sto epomeno addition
		result1= (*it)->CommonFactor(a);
		return result1;
	}
	else {	// an einai pairnoume tis times twn paidiwn, tis siggrinoyme kai briskoume ton koino paragonta
		result1 = (*it)->CommonFactor(a);

		advance(it, 1);

		result2 = (*it)->CommonFactor(a);

		list<int> temp = result1; // apothikeuoyme prosorina ton koino paragonta
		std::list<int> result;
		std::set_intersection(result1.begin(), result1.end(), result2.begin(), result2.end(), std::back_inserter(result));

		if ( !result.empty()) { // an h tomi den emfanizei einai keni tote iparxei koinos paragontas opote epistrefoume me auton
			return result;
		}
		else { // den iparxei koinos paragontas epistrefoume ton koino paragonta twn prohgoumen athroismatwn ginomeonwn
			// kombos opou tha ksekinisoume tin paragontopoihsh tou dentroy
			if (locked == false) {
				locked = true;
				cout << " !!!PROSORINO SHMEIO!!! Kombos apo ton opoio tha ksekinisei i paragontopoihsh tou dentrou logo koinou paragonta: " << (*em)->GetGraphvizLabel() << endl;
			}
			return result1;
		}

	}

}
list<int> CSubtraction::CommonFactor(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	result1= (*it)->CommonFactor(a) ;

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result2 = (*it)->CommonFactor(a); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	result1.merge(result2);

	for (auto const& i : result1) {
		std::cout << "Num: " << i << "\n";
	}
	return result1; // epistrefoume tin timi ston gonea
}
list<int> CMultiplication::CommonFactor(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;
	list<STNode*>temp;

	result1 = (*it)->CommonFactor(a);

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result2 = (*it)->CommonFactor(a); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	result1.merge(result2);

	return result1; // epistrefoume tin timi ston gonea
}
list<int> CDivision::CommonFactor(list<STNode*> a) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	result1 = (*it)->CommonFactor(a);

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result2 = (*it)->CommonFactor(a); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	result1.merge(result2);
	cout << "ELAAAA ELAELAELELALAELALELAE: " << endl;

	for (auto const& i : result1) {
		std::cout << "Num: " << i << "\n";
	}
	return result1; // epistrefoume tin timi ston gonea
}
list<int> CAssignment::CommonFactor(list<STNode*> a) {
	// prepei na ipologistei h ekfrasi pou brisketai sta aristera tis anathesis
	// kai i timi tis na anatethei ston SymbolTable

	// deixnei sto prwto stoixeio tis listas(prwto paidi) pou einai to identifier
	list<STNode*>::iterator it = m_children->begin();

	// to pairnoume to paidi kai to anathetoume se mia metabliti 
	// tipou identifier wste na mporoume na prospelasoume ta pedia tis klasis
	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	// dinoume onoma metablitis kai ton arithmo pou prokiptei apo thigatrikous kombous

	// minima pou mas anaferei oti auti i metabliti tropopoihthike
	cout << id->m_name << "=" << GetInitValue(id->m_name) << endl;
	list<int> yes;
	// yes.insert(GetInitValue(id->m_name));

	return yes;
}

list<int> CNUMBER::CommonFactor(list<STNode*> a) {
	list<int> nai;
	nai.push_back(m_number);
	return nai;
}
list<int> CIDENTIFIER::CommonFactor(list<STNode*> a) {
	list<int> val;
	val.push_back (GetInitValue(m_name));
	return val;
}
