#pragma once
#include "SymbolNodes.h"
#include "SymbolTable.h"
#include <iostream>
#include <set>
#include <algorithm>

// ======== IMPLEMENTATION OF METHODS ========

CCompileUnit::CCompileUnit(): STNode(NodeType::NT_COMPILEUNIT) {}
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
	// assigning nodes with name/values
	m_number = atoi(text);
	m_graphvizLabel.append("_" + std::to_string(m_number));
}
CNUMBER::~CNUMBER() {}

CIDENTIFIER::CIDENTIFIER(char* text) : STNode(NodeType::NT_EXPRESSION_IDENTIFIER){
	m_name = (string)text;
	m_graphvizLabel.append("_" + m_name);	// attaching name of variable
											// ( contructors original name is defined in STNode.cpp)
}
CIDENTIFIER::~CIDENTIFIER() {}

void CCompileUnit::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	list<STNode*>::iterator it;
	// creating "main" tree .dot file
	if (parent == nullptr) {
		(*dotFile) << "digraph G{ \n";
		for (it = m_children->begin(); it != m_children->end(); it++) {
			(*it)->Visit_SyntaxTreePrinter(dotFile, this);
		}
		(*dotFile) << "}";

		dotFile->close();
	}
	else {
		STNode::Visit_SyntaxTreePrinter(dotFile, this);
	}
}
void CCompileUnit::Visit_FactorizedTree(list<STNode*>* node, ofstream* factfile, STNode* parent, int common_factor, list<int> all_nums) {
	list<STNode*>::iterator it;
	// creating "factorized" tree .dot file
	if (parent == nullptr) {
		(*factfile) << "digraph G{ \n";
		for (it = m_children->begin(); it != m_children->end(); it++) {
			(*it)->Visit_FactorizedTree(node, factfile, this, common_factor, all_nums);
		}
		(*factfile) << "}";

		factfile->close();
	}
	else {
		STNode::Visit_FactorizedTree(node, factfile, this, common_factor, all_nums);
	}
}


int CStatement::Visit_Eval() {
	list<STNode*>::iterator it;
	int p=0;
	// has two alternatives but we get the full expression which is: " expression ';' "
	if (m_children->size() != 0) {
		it = m_children->begin(); 
		p = (*it)->Visit_Eval();
	}
	return p;
}
int CAddition::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	advance(it, 1);
	result += (*it)->Visit_Eval();

	return result;
}
int CSubtraction::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	advance(it, 1);
	result -= (*it)->Visit_Eval();

	return result;
}
int CMultiplication::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	advance(it, 1);
	result *= (*it)->Visit_Eval();

	return result;
}
int CDivision::Visit_Eval() {
	list<STNode*>::iterator it = m_children->begin();
	int result = (*it)->Visit_Eval();

	advance(it, 1);
	result /= (*it)->Visit_Eval();

	return result;
}
int CAssignment::Visit_Eval() {
	// the phrase to the left of the assignment must be calculated 
	// and the value get assigned to SymbolTable

	list<STNode*>::iterator it = m_children->begin();

	// we take the child and assign it to an identifier type variable
	// so that we can access the children of the class
	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	SetValue(id->m_name, (*it)->Visit_Eval());

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
	return a;
}
list<STNode*> CIDENTIFIER::SearchingAddition(list<STNode*> a) {
	return a; // just to return something
}


bool first_time = true;		// first time puts child to the list
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
	list<STNode*>temp;
	bool found = false;
	for (auto const& i : a) {	// checks if the addition node in which we are now, is already on our all_add list from main
		if (i->GetGraphvizLabel() == this->GetGraphvizLabel()) {
			found = true;
		}
	}
	if (found) {				// if we are, put children on list
		STNode* child_1 = (this)->GetChild(0)->GetChild(1), * child_2 = (this)->GetChild(1);
		if (first_time) {
			b.push_back(child_1);
			b.push_back(child_2);
			first_time = false;
		}
		else {
			b.push_back(child_1);
		}

		temp = (*it)->SearchingMultiplications(a, b);

		advance(it, 1);
		temp = (*it)->SearchingMultiplications(a, temp); 
		return temp;
	}
	else {						// if we are not, go to the next node
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
	return b;
}
list<STNode*> CIDENTIFIER::SearchingMultiplications(list<STNode*> a, list<STNode*> b) {
	return a; // just to return something
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
	list<int> result1, result2;
	int a_len = a.size();

	int cnt = 0; 
	for (auto const& i : a) {				 // checks if the addition node in which we are now, is already on our list with sum of products from main
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;		}
	}
	if (cnt == a_len) {						// if its not, we return
		advance(ed, 1);
		list<int> last_multi = (*ed)->CommonFactor(a, b);
		return last_multi;
	}
	else {									// if it is, we get the values from children, compare them and find the common factor
		result1 = (*it)->CommonFactor(a, b);
		advance(it, 1);
		result2 = (*it)->CommonFactor(a, b);

		std::list<int> result;
		std::set_intersection(result1.begin(), result1.end(), result2.begin(), result2.end(), std::back_inserter(result));

		if ( !result.empty()) {				// if the result of the intersection isn't empty, there is a common factor that we return
			return result;					
		}									
		else {								// if it's empty, we return the merge of the two previous numbers
			result1.merge(result2);
			return result1;
		}
	}
}
list<int> CSubtraction::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	result1= (*it)->CommonFactor(a, b);

	advance(it, 1);
	result2 = (*it)->CommonFactor(a, b);

	result1.merge(result2);
	return result1;
}
list<int> CMultiplication::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;
	list<STNode*>temp;

	int cnt = 0;
	int b_len = b.size();					// checks if the multiplication node in which we are now, is already 
	for (auto const& i : b) {				// on our multiplications list from main
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;
		}
	}
	if (cnt == b_len) {						// if its not we go to next multiplication
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

	advance(it, 1);
	result2 = (*it)->CommonFactor(a, b);
	result1.merge(result2);

	return result1;
}
list<int> CAssignment::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it = m_children->begin();

	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	cout << id->m_name << "=" << GetInitValue(id->m_name) << endl;
	list<int> yes;		// just to return something

	return yes;
}

list<int> CNUMBER::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<int> nai;
	nai.push_back(m_number);
	return nai;
}
list<int> CIDENTIFIER::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<int> val;	// just to return something
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
	list<int> result1, result2;
	int a_len = a.size();

	int cnt = 0;
	for (auto const& i : a) {
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;
		}
	}
	if (cnt == a_len) {
		advance(ed, 1);
		list<int> last_multi = (*ed)->FindingNums(a, b, c);
		return last_multi;
	}
	else {

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

	advance(it, 1);
	result2 = (*it)->FindingNums(a, b, c); 

	result1.merge(result2);

	return result1;
}
list<int> CMultiplication::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<STNode*>::iterator it = m_children->begin();
	list<int> result1, result2;

	int cnt = 0;
	int b_len = b.size();
	for (auto const& i : b) {	
		if (this->GetGraphvizLabel() != i->GetGraphvizLabel()) {
			cnt++;
		}
	}
	if (cnt == b_len) {	
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

	advance(it, 1);
	result2 = (*it)->FindingNums(a, b, c); 

	result1.merge(result2);

	return result1;
}
list<int> CAssignment::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<STNode*>::iterator it = m_children->begin();

	CIDENTIFIER* id = dynamic_cast<CIDENTIFIER*>(*it);

	advance(it, 1);

	cout << id->m_name << "=" << GetInitValue(id->m_name) << endl;
	list<int> yes;
	return yes;
}

list<int> CNUMBER::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<int> nai;
	nai.push_back(m_number);
	return nai;
}
list<int> CIDENTIFIER::FindingNums(list<STNode*> a, list<STNode*> b, int c) {
	list<int> val;
	return val;		// just to return something
}