#include "STNode.h"
#include <iostream>
#include <set>

STNode* g_root=nullptr; // fernei tin riza tou sintaktikou dentrou kai tin DILWNOUME STO STNode.h
int STNode::m_serialCounter = 0;
const char* g_nodeTypeLabels[] = {
	"COMPILEUNIT", "STATEMENTS", "STATEMENT",
	"NUMBER", "IDENTIFIER", "ADDITION", 
	"SUBTRACTION","MULTIPLICATION",
	"DIVISION", "ASSIGNMENT"
};

STNode::STNode(NodeType type) {
	// afirimeni goniki klasi

	m_nodeType = type;
	m_serial = m_serialCounter++;
	m_children = new list<STNode*>;
	m_parents = new list<STNode*>;

	// arxikopoioume to onoma tou kombou me to onoma toy tipou pou
	// exoume orisei ston pinaka me ta alfarithmitika
	m_graphvizLabel = _strdup(g_nodeTypeLabels[m_nodeType]);

	// episinaptoume etiketa kai ton seiriako arithmo tou kombou
	m_graphvizLabel.append("_" + std::to_string(m_serial));
}

STNode::~STNode() {
	// apeleutheronoyme opoia mnimi exoyme desmeusei sto constructor

	// delete apo filla stin riza
	list<STNode*>::iterator it;
	for (it = m_children->begin(); it != m_children->end(); it++) {
		delete* it;
	}
	delete m_children;
	delete m_parents;
}

void STNode::AddChild(STNode* node) {
	// prosthetoume sto pisw meros tis listas ton kombo
	m_children->push_back(node);

	//pame ston kombo pou prosthesame kai enimerwnoyme tin lista 
	// me tous goneis oti o trexontas kombos apotelei patera tou kombou node
	node->m_parents->push_back(this);
}

STNode* STNode::GetChild(int index) {
	list<STNode*>::iterator it = m_children->begin();

	// proothoume ton iterator ston index-th kataseira paidi
	advance(it, index);

	// * gia na epistrepsei to paidi
	return *it;
}

STNode* STNode::GetParent(int index){
	list<STNode*>::iterator it = m_parents->begin();

	// proothoume ton iterator ston index-th kataseira gonea
	advance(it, index);

	// * gia na epistrepsei ton gonea
	return *it;
}

void STNode::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	//diatrexei ta paidia tou trexon kombou pou tha episkeutoume
	list<STNode*>::iterator it; 

	//tiponoume tin akmi apo ton patera pros ton trexonta kombo
	// "\"" -> gia na egklioume tis akmes mesa se dipla eisagwgika giati dimioiurgountai parenergies otan iparxoyn mesa sta onoma twn kombwn
	// "\"  -> gia na periklisoume me dipla eisagwgika to onoma tou komboy patera
	// \""  -> gia to onoma tou trexonta kombou p akolouthei;
	
	// cout << parent->GetGraphvizLabel() << "\n";

	(*dotFile) << "\"" << parent->GetGraphvizLabel() << "\"->\"" << GetGraphvizLabel() << "\";\n";

	for (it = m_children->begin(); it != m_children->end(); it++) {
		//den orizoume postorder action ektos apo tin riza
		(*it)->Visit_SyntaxTreePrinter(dotFile, this);
		
	}
}

int STNode::Visit_Eval() {
	// eksasfalizei tin diasxisei tou sintaktikou dentroy gia opoion kombo efarmozetai i methodos

	list<STNode*>::iterator it;

	// diatrexei ta paidia tou komboy
	int nai;
	for (it = m_children->begin(); it != m_children->end(); it++) {
		nai = (*it)->Visit_Eval();
	}
	return nai;
}

list<STNode*> STNode::SearchingAddition(list<STNode*> a) {
	list<STNode*>::iterator it;
	list<STNode*> piip;

	// diatrexei ta paidia tou komboy
	for (it = m_children->begin(); it != m_children->end(); it++) {
		piip=(*it)->SearchingAddition(a);
	}
	return piip;
}

list<STNode*> STNode::SearchingMultiplication(list<STNode*> b) {
	list<STNode*>::iterator it;
	list<STNode*> piip;

	// diatrexei ta paidia tou komboy
	for (it = m_children->begin(); it != m_children->end(); it++) {
		piip = (*it)->SearchingMultiplication(b);
	}
	return piip;
}

list<int> STNode::CommonFactor(list<STNode*> a, list<STNode*> b) {
	list<STNode*>::iterator it;
	list<int> piip;

	// diatrexei ta paidia tou komboy
	for (it = m_children->begin(); it != m_children->end(); it++) {
		piip=  (*it)->CommonFactor(a,b) ;
	}
	return piip;
}

NodeType STNode::GetNodeType() {
	// epistrefoun tipo kombou kai etiketas tou

	return m_nodeType;
}

string STNode::GetGraphvizLabel() {
	return m_graphvizLabel;
}