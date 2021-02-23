#include "STNode.h"

STNode* g_root=nullptr; // fernei tin riza tou sintaktikou dentrou kai tin dilwnoume STO STNode.h
int STNode::m_serialCounter = 0;
const char* g_nodeTypeLabels[] = {
	"COMPILEUNIT", "STATEMENTS", "STATEMENT",
	"NUMBER", "ADDITION", 
	"SUBTRACTION","MULTIPLICATION",
	"DIVISION"
};

//afirimeni goniki klasi
STNode::STNode(NodeType type) {
	m_nodeType = type;
	m_serial = m_serialCounter++;
	m_children = new list<STNode*>;
	m_parents = new list<STNode*>;

	// arxikopoioume to onoma tou komboy me to onoma toy tipou p 
	// exoyme orisei ston pinaka me ta alfarithmika
	m_graphvizLabel = _strdup(g_nodeTypeLabels[m_nodeType]);

	// episinaproume etiketa kai ton diariako arithmo tou kombou
	m_graphvizLabel.append("_" + std::to_string(m_serial));
}

// apeleytheronoyme opoia mnimi exoyme desmeusei sto constructor
STNode::~STNode() {

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
	// me tous goneis oti o trrexontas kombos apotelei patera tou kombou node
	node->m_parents->push_back(this);
}

STNode* STNode::GetChild(int index) {
	list<STNode*>::iterator it = m_children->begin();

	// proothoume ton iterator ston indexth kataseira paidi
	advance(it, index);

	// * gia na epistrepsei to paidi
	return *it;
}

STNode* STNode::GetParent(int index){
	list<STNode*>::iterator it = m_parents->begin();

	// proothoume ton iterator ston indexth kataseira gonea
	advance(it, index);

	// * gia na epistrepsei ton gonea
	return *it;
}

void STNode::Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) {
	//diatrexei ta paidia tou trexon kombou pou tha peiskeutoume
	list<STNode*>::iterator it; 

	//tiponoume tin akmi apo ton patera pros ton trexonta kombo
	// "\"" -> gia na egklioume tis akmes mesa se dipla eisagwgika giati dimioiurgountai parenergies otan iparxoyn mesa sta onoma twn kombwn
	// "\"  -> gia na periklisoume me dipla eisagwgika to onoma tou komboy patera
	// \""  -> gia to onoma tou trexonta kombou p akolouthei
	(*dotFile) << "\"" << parent->GetGraphvizLabel() << "\"->\"" << GetGraphvizLabel() << "\";\n";

	for (it = m_children->begin(); it != m_children->end(); it++) {
		//den orizoume postorder action ektos apo tin riza
		(*it)->Visit_SyntaxTreePrinter(dotFile, this);
	}
}

// epistrefoun tipo kombou kai etiketas toy
NodeType STNode::GetNodeType() {
	return m_nodeType;
}

string STNode::GetGraphvizLabel() {
	return m_graphvizLabel;
}