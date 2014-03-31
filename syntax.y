%{
 #include	<stdio.h>
 #include	"lex.yy.c"    
%}

/*declared tokens*/
%token INT Float
%token ADD SUB MUL DIV

%%
Calc	:
		|	E