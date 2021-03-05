%option noyywrap
%{
#include "SymbolNodes.h"
#include "Grammar.tab.h"
#include "SymbolTable.h"
#define YY_DECL int yylex(yy::parser::semantic_type *yylval)
typedef yy::parser::token token;
%}

%%

";" { return ';'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }
"=" { return '='; }
"(" { return '('; }
")" { return ')'; }

"0"|[1-9][0-9]* {   yylval->node = new CNUMBER(yytext);
					return token::NUMBER; }

[a-zA-Z][a-zA-Z0-9_]* {   yylval->node = new CIDENTIFIER(yytext);
						  GetInitValue(yytext);
					      return token::IDENTIFIER; }

[ \r\n\t]+ ;

%%