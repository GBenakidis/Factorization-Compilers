#pragma once
#include "STNode.h"

class CCompileUnit : public STNode {
public:
	CCompileUnit();
	virtual ~CCompileUnit();

	void Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) override;
	void Visit_FactorizedTree(list<STNode*>* node, ofstream* factfile, STNode* parent, int common_factor, list<int> all_nums) override;

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
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
};

class CAddition : public STNode {
public:
	CAddition();
	virtual ~CAddition();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
};

class CSubtraction : public STNode {
public:
	CSubtraction();
	virtual ~CSubtraction();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
};

class CDivision : public STNode {
public:
	CDivision();
	virtual ~CDivision();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
};

class CMultiplication : public STNode {
public:
	CMultiplication();
	virtual ~CMultiplication();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
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
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
};

class CIDENTIFIER : public STNode {
public:
	string m_name;

	CIDENTIFIER(char* text);
	virtual ~CIDENTIFIER();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
};

class CAssignment : public STNode {
public:
	CAssignment();
	virtual ~CAssignment();

	int Visit_Eval() override;
	list<STNode*> SearchingAddition(list<STNode*>) override;
	list<STNode*> SearchingMultiplications(list<STNode*>, list<STNode*>) override;
	list<int> CommonFactor(list<STNode*>, list<STNode*>) override;
	list<int> FindingNums(list<STNode*>, list<STNode*>, int) override;
};
