%{

#include"node.h"
#include"yacchw3.tab.h"
//#include"check.h"
extern "C"
{
	void yyerror(const char *);
	int yywrap(void);
	int yylex(void);
}

%}

%%
[&|()~,]  return *yytext;
"=>" return IMP;
[a-z] {yylval.name = yytext; return VAR;}
[A-Z][A-Za-z]* {yylval.name = yytext;
		return POC;}//predicate or constant
. ;//ignore others

%%

int yywrap(void)
{
	return 1;


}

void read_to_buffer(const char*s)

{
	yy_switch_to_buffer(yy_scan_string(s));

}
