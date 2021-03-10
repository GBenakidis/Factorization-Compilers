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

void CCompileUnit::Visit_FactorizedTree(list<STNode*> *node, ofstream* factfile, STNode* parent, STNode* common_factor) {
	list<STNode*>::iterator it;
	if (parent == nullptr) {
		(*factfile) << "digraph G{ \n";
		for (it = m_children->begin(); it != m_children->end(); it++) {
			(*it)->Visit_FactorizedTree(node, factfile, this, common_factor); // edw mpainei stin STNode.cpp
		}
		(*factfile) << "}";

		factfile->close();
	}
	else {
		STNode::Visit_FactorizedTree(node, factfile, this, common_factor);
	}
}


int CStatement::Visit_Eval() {
	list<STNode*>::iterator it;
	int nai=0;
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

	a.push_back(this);

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


bool first_time = true;
list<STNode*> CStatement::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it;

	list<STNode*> arr;
	if (m_children->size() != 0) {
		it = m_children->begin();
		arr = (*it)->SearchingMultiplications(a,b);
	}
	return arr;
}
list<STNode*> CAddition::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator alt = m_children->begin();
	list<STNode*>temp;
	bool found = false;
	for (auto const& i : a) {
		if (i->GetGraphvizLabel() == this->GetGraphvizLabel()) {
			found = true;
		}
	}
	if (found) {
		STNode* paidi_1 = (this)->GetChild(0)->GetChild(1), * paidi_2 = (this)->GetChild(1);
		if (first_time) {
			b.push_back(paidi_1);
			b.push_back(paidi_2);
			first_time = false;
		}
		else {
			b.push_back(paidi_1);
		}

		temp = (*it)->SearchingMultiplications(a, b);

		advance(it, 1);
		temp = (*it)->SearchingMultiplications(a, temp); 
		return temp;
	}
	else {
		temp = (*it)->SearchingMultiplications(a, b);
		return temp;
	}
}
list<STNode*> CSubtraction::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	(*it)->SearchingMultiplications(a, b);

	advance(it, 1);
	(*it)->SearchingMultiplications(a, b);

	return a;
}
list<STNode*> CMultiplication::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>temp;

	temp = (*it)->SearchingMultiplications(a, b);

	advance(it, 1);

	temp = (*it)->SearchingMultiplications(a, b);

	return temp;
}
list<STNode*> CDivision::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	(*it)->SearchingMultiplications(a, b);

	advance(it, 1);
	(*it)->SearchingMultiplications(a, b);
	return a;
}
list<STNode*> CAssignment::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();

	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	cout << id->m_name << "=" << GetInitValue(id->m_name) << endl;

	return a;
}

list<STNode*> CNUMBER::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	// return m_number;
	return b;
}
list<STNode*> CIDENTIFIER::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	// return GetInitValue(m_name);
	return a;
}


list<int> CStatement::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it;
	list<int> arr;

	if (m_children->size() != 0) { 
		it = m_children->begin();
		arr= (*it)->CommonFactor(a,b);
	}
	return arr;
}
list<int> CAddition::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator ed = m_children->begin();
	list<STNode*> edw;
	list<int> result1, result2;
	int a_len = a.size();

	int cnt = 0;
	for (auto const& i : a) {				 // psaxnoume an o kombos pou briskomaste einai mesa stin lista me ta athroismata ginomenwn
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;		}
	}
	if (cnt == a_len) {						// an den einai girname
		advance(ed, 1);
		list<int> last_multi = (*ed)->CommonFactor(a, b);
		return last_multi;
	}
	else {									// an einai pairnoume tis times twn paidiwn, tis siggrinoyme kai briskoume ton koino paragonta
		
		result1 = (*it)->CommonFactor(a, b);
		advance(it, 1);
		result2 = (*it)->CommonFactor(a, b);

		std::list<int> result;
		std::set_intersection(result1.begin(), result1.end(), result2.begin(), result2.end(), std::back_inserter(result));

		if ( !result.empty()) {				// an h tomi den emfanizei einai keni tote iparxei koinos paragontas opote epistrefoume me auton
			return result;
		}
		else {								// den iparxei koinos paragontas epistrefoume ton koino paragonta twn prohgoumen athroismatwn ginomeonwn
											// kombos opou tha ksekinisoume tin paragontopoihsh tou dentroy
			result1.merge(result2);
			return result1;
		}
	}
}
list<int> CSubtraction::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	result1= (*it)->CommonFactor(a, b);
	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result2 = (*it)->CommonFactor(a, b); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	result1.merge(result2);
	return result1; // epistrefoume tin timi ston gonea
}
list<int> CMultiplication::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;
	list<STNode*>temp;

	int cnt = 0;
	int b_len = b.size();
	for (auto const& i : b) {				 // psaxnoume an o kombos pou briskomaste einai mesa stin lista me ta athroismata ginomenwn
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;
		}
	}
	if (cnt == b_len) {						// an den einai pame sto epomeno mult
		return result1;
	}
	else {

		result1 = (*it)->CommonFactor(a, b);
		advance(it, 1);
		result2 = (*it)->CommonFactor(a, b);

		result1.merge(result2);
		return result1;
	}	
}
list<int> CDivision::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	result1 = (*it)->CommonFactor(a, b);

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result2 = (*it)->CommonFactor(a, b); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	result1.merge(result2);
	cout << "ELAAAA ELAELAELELALAELALELAE: " << endl;

	for (auto const& i : result1) {
		std::cout << "Num: " << i << "\n";
	}
	return result1; // epistrefoume tin timi ston gonea
}
list<int> CAssignment::CommonFactor(list<STNode*> a, list<STNode*> b) {
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

list<int> CNUMBER::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<int> nai;
	nai.push_back(m_number);
	return nai;
}
list<int> CIDENTIFIER::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<int> val;
	return val;
}


list<int> CStatement::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<STNode*>::iterator it;
	list<int> arr;

	if (m_children->size() != 0) {
		it = m_children->begin();
		arr = (*it)->FindingNums(a,b,c);
	}
	return arr;
}
list<int> CAddition::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<STNode*>::iterator it = m_children->begin();
	list<STNode*>::iterator ed = m_children->begin();
	list<STNode*> edw;
	list<int> result1, result2;
	int a_len = a.size();

	int cnt = 0;
	for (auto const& i : a) {				 // psaxnoume an o kombos pou briskomaste einai mesa stin lista me ta athroismata ginomenwn
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;
		}
	}
	if (cnt == a_len) {						// an den einai girname
		advance(ed, 1);
		list<int> last_multi = (*ed)->FindingNums(a, b, c);
		return last_multi;
	}
	else {									// an einai pairnoume tis times twn paidiwn, tis siggrinoyme kai briskoume ton koino paragonta

		result1 = (*it)->FindingNums(a, b, c);
		advance(it, 1);
		result2 = (*it)->FindingNums(a, b, c);

		result1.merge(result2);
		return result1;

	}
}
list<int> CSubtraction::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	result1 = (*it)->FindingNums(a, b, c);

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result2 = (*it)->FindingNums(a, b, c); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	result1.merge(result2);

	for (auto const& i : result1) {
		std::cout << "Num: " << i << "\n";
	}
	return result1; // epistrefoume tin timi ston gonea
}
list<int> CMultiplication::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;
	list<STNode*>temp;

	int cnt = 0;
	int b_len = b.size();
	for (auto const& i : b) {				 // psaxnoume an o kombos pou briskomaste einai mesa stin lista me ta athroismata ginomenwn
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;
		}
	}
	if (cnt == b_len) {	
		cout << " O NAI "<< this->GetGraphvizLabel() << endl;
		return result1;
	}
	else {

		result1 = (*it)->FindingNums(a, b,c);
		advance(it, 1);
		result2 = (*it)->FindingNums(a, b,c);
		bool found1 = (std::find(result1.begin(), result1.end(), c) != result1.end());
		bool found2 = (std::find(result2.begin(), result2.end(), c) != result2.end());
		if (!found1 && !found2) {
			result1.clear();
			return result1;
		}
		result1.remove(c);
		result2.remove(c);
		result1.merge(result2);
		return result1;
	}
}
list<int> CDivision::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	result1 = (*it)->FindingNums(a, b, c);

	// auksanoume ton iterator gia na deicnei sto epomeno paidi
	advance(it, 1);
	result2 = (*it)->FindingNums(a, b, c); // pollaplasiazoume ayto pou tha epistrepsei gia to DEUTERO paidi

	result1.merge(result2);

	for (auto const& i : result1) {
		std::cout << "Num: " << i << "\n";
	}
	return result1; // epistrefoume tin timi ston gonea
}
list<int> CAssignment::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
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

list<int> CNUMBER::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<int> nai;

		nai.push_back(m_number);

	return nai;
}
list<int> CIDENTIFIER::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<int> val;
	return val;
}