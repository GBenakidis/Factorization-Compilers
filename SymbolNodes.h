#pragma once
// periexei ton orismo tis gonikis klasis tou sintaktikou dentrou
#include "STNode.h"

//twra ftiaxnoume mia klasi gia kathe ksexwristo tipo kombou pou theloume na apikonistei sto sintaktiko dentro

class CCompileUnit : public STNode {
public:
	CCompileUnit();
	virtual ~CCompileUnit();

	// ektelei to perasma, episis tin eksidikeuoume gt o kombos CCCompU tha prepei na eksagoume prologo 
	// epilogo kai na kleisoyme to arxeio, gi atuo tin methodo V.. p exoume orisei stin afirimeni goniki 
	// klasi tin iposkelizoume me thn override
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
};

class CAddition : public STNode {
public:
	CAddition();
	virtual ~CAddition();
};

class CSubtraction : public STNode {
public:
	CSubtraction();
	virtual ~CSubtraction();
};

class CDivision : public STNode {
public:
	CDivision();
	virtual ~CDivision();
};

class CMultiplication : public STNode {
public:
	CMultiplication();
	virtual ~CMultiplication();
};

//kefalaio giati antistoixei se termatiko stoixeio tis grammatikis, enw ta alla se mi termatika
class CNUMBER : public STNode {
public:
	// arithmos pou antiprosopeuei i klasi
	int m_number;

	CNUMBER(char *text);
	virtual ~CNUMBER();
};


