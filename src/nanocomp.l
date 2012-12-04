/* Flex file for lexical analysis of the nanocomp files */

%{
#include "flexDefines.hpp"
using namespace std;
int line = 1;
%}

NUMBER	[0-9]+
LOWERCASE  [a-z]
UPPERCASE   [A-Z]
ID  [a-zA-Z][a-zA-Z0-9]*

%option c++
%option noyywrap

%%

[ \t]+

#.*$	{cout << "Line " << line << "; " << "Coment" << endl;}

". "	{cout << "Line " << line << "; " << "Point" << endl; return POINT;}

"."[ ]*$	{cout << "Line " << line << "; " << "Point" << endl; return POINT;}

"- "	{cout << "Line " << line << "; " << "Slash" << endl; return SLASH;}

"-"[ ]*$	{cout << "Line " << line << "; " << "Slash" << endl; return SLASH;}

"! "	{cout << "Line " << line << "; " << "Negation" << endl; return NEGATION;}

"!"[ ]*$	{cout << "Line " << line << "; " << "Negation" << endl; return NEGATION;}

{NUMBER}[ ]	{cout << "Line " << line << "; " << "Number " << YYText() << endl; return NUMBER;}

{NUMBER}[ ]*$	{cout << "Line " << line << "; " << "Number " << YYText() << endl; return NUMBER;}

{LOWERCASE}[ ]	{cout << "Line " << line << "; " << "Char " << YYText() << endl; return LOWER;}

{LOWERCASE}[ ]*$	{cout << "Line " << line << "; " << "Char " << YYText() << endl; return LOWER;}

{UPPERCASE}[ ]	{cout << "Line " << line << "; " << "Char " << YYText() << endl; return UPPER;}

{UPPERCASE}[ ]*$	{cout << "Line " << line << "; " << "Char " << YYText() << endl; return UPPER;}

{ID} {cout << "Line " << line << "; " << "Identifier " << YYText() << endl; return IDENTIFIER;}

\n	{line++;}

.	{cout << "Line " << line << "; " << "Unknown token" << endl; return 1;}

%%
