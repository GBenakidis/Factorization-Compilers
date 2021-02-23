%verbose
%language "C++"

%{
#include "Grammar.tab.h"
#include "SymbolNodes.h"
extern int yylex(yy::parser::semantic_type *yylval);
%}

%code requires{ 
#include "STNode.h"
}
%union{
	STNode *node; 
}

%start compileUnit
%token <node> NUMBER
%left '+' '-'
%left '*' '/'
%type <node> expression compileUnit statements statement

%%

compileUnit : statements		 { g_root = $$ = new CCompileUnit(); $$->AddChild($1); }
		;

statements : statement			 { $$ = new CStatements(); $$->AddChild($1); }
		| statements statement	 { $$ = new CStatements(); $$->AddChild($1); $$->AddChild($2); }
		;

statement : expression ';'		 { $$ = new CStatement(); $$->AddChild($1); }
		| ';'					 { $$ = new CStatement(); }
		;

expression : NUMBER
			| expression '+' expression { $$ = new CAddition(); $$->AddChild($1); $$->AddChild($3); }
			| expression '-' expression { $$ = new CSubtraction(); $$->AddChild($1); $$->AddChild($3); }
			| expression '*' expression { $$ = new CMultiplication(); $$->AddChild($1); $$->AddChild($3); }
			| expression '/' expression { $$ = new CDivision(); $$->AddChild($1); $$->AddChild($3); }
			| '(' expression ')'		{ $$ = $2; }
			;

%%
 namespace yy{
	 void parser::error (const location_type& loc, const std::string& msg){
		std::cerr << "eroor at " << loc << ": " << msg << std::endl;
	 }
 }