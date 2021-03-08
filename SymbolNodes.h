#pragma once
// periexei ton orismo tis gonikis klasis tou sintaktikou dentrou
#include "STNode.h"

// twra ftiaxnoume mia klasi gia kathe ksexwristo tipo kombou 
// pou theloume na apikonistei sto sintaktiko dentro

class CCompileUnit : public STNode {
public:
	CCompileUnit();
	virtual ~CCompileUnit();

	// ektelei to perasma, episis tin eksidikeuoume giati o kombos CCompileUnit tha prepei na eksagei prologo 
	// epilogo kai na kleisei to arxeio, gi auto tin methodo Visit_SyntaxTreePrinter pou exoume orisei 
	// stin afirimeni goniki klasi tin iposkelizoume me thn override
	void Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) override;

};

class CStatements : public STNode {
public:
	CStatements();
	virtual ~CStatements();
};

class CStatement : public STNode {
public:
	CStatement();
	virtual ~CStatement();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};

class CAddition : public STNode {
public:
	CAddition();
	virtual ~CAddition();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};

class CSubtraction : public STNode {
public:
	CSubtraction();
	virtual ~CSubtraction();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};

class CDivision : public STNode {
public:
	CDivision();
	virtual ~CDivision();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};

class CMultiplication : public STNode {
public:
	CMultiplication();
	virtual ~CMultiplication();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};

class CNUMBER : public STNode {
// me kefalaia to NUMBER, giati antistoixei se termatiko stoixeio tis grammatikis, enw ta alla se mi termatika
public:
	// arithmos pou antiprosopeuei i klasi
	int m_number;

	CNUMBER(char *text);
	virtual ~CNUMBER();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};

class CIDENTIFIER : public STNode {
public:
	string m_name;

	CIDENTIFIER(char* text);
	virtual ~CIDENTIFIER();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};

class CAssignment : public STNode {
public:
	CAssignment();
	virtual ~CAssignment();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>) override;
};
