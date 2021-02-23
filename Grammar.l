%option noyywrap
%{
#include "SymbolNodes.h"
#include "Grammar.tab.h"
#define YY_DECL int yylex(yy::parser::semantic_type *yylval)
typedef yy::parser::token token;

%}

%%

";" { return ';'; }
"+" { return '+'; }
"-" { return '-'; }
"*" { return '*'; }
"/" { return '/'; }

"0"|[1-9][0-9]* {   yylval->node = new CNUMBER(yytext);
					return token::NUMBER; }

[ \r\n\t]+ ;

%%