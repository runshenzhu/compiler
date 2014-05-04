%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mconfig.h"
#include "syntax.tab.h"
//#define YYSTYPE TreeNode*


int yycolumn = 1;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
    yylloc.first_column = yycolumn;yylloc.last_column = yycolumn + yyleng -1;\
    yycolumn += yyleng;

%}

%option yylineno
delim		[ \t]
ws 		{delim}+
digit		[0-9]
int 		0|([1-9]{digit}*)
float		([1-9]{digit}*(\.)?{digit}*)|({digit}*\.{digit}+)
hex			0(x|X)[0-9a-fA-F]+
oct			0[0-7]+
letter		[_A-Za-z]
id			{letter}({letter}|{digit})*
reserved_word	("int"|"float"|"bool"|"if"|"else"|"do"|"while"|"return")
op 			("+"|"-"|"*"|"/"|"&"|"&&"|"|"|"||"|"%"|"^"|"<="|">="|"="|"=="|"+="|"("|")"|",")
bool		("true"|"false")
%%

\n 			{ yycolumn = 1; }
{ws}		{/* do nothing */}
"/*"([*]*(([^*/])+([/])*)*)*"*/"	{/* do nothing */}
"//"		{char c = input(); while(c != '\n'){if(c == EOF) break; c=input();}}
"int"		  {yylval=CreatNode("INT", NULL, "TYPE");return TYPE;}
"float" 	  {yylval=CreatNode("FLOAT", NULL, "TYPE");return TYPE;}
"void" 	  {yylval=CreatNode(yytext, NULL, "TYPE");return TYPE;}
"return"      {yylval=CreatNode(yytext, NULL, "RETURN");return(RETURN);}
"if"          {yylval=CreatNode(yytext, NULL, "IF");return(IF);}
"else"        {yylval=CreatNode(yytext, NULL, "ELSE");return(ELSE);}
"while"       {yylval=CreatNode(yytext, NULL, "WHILE");return(WHILE);}
"struct"	{yylval=CreatNode(yytext, NULL, "STRUCT");return(STRUCT);}
";"         {yylval=CreatNode(yytext, NULL, "SEMI");return(SEMI);}
","         {yylval=CreatNode(yytext, NULL, "COMMA");return(COMMA);}
"="         {yylval=CreatNode(yytext, NULL, "ASSIGNOP");return(ASSIGNOP);}
">"         {yylval=CreatNode(yytext, NULL, "RELOP");return(RELOP);}
"<"         {yylval=CreatNode(yytext, NULL, "RELOP");return(RELOP);}
">="        {yylval=CreatNode(yytext, NULL, "RELOP");return(RELOP);}
"<="        {yylval=CreatNode(yytext, NULL, "RELOP");return(RELOP);}
"=="        {yylval=CreatNode(yytext, NULL, "RELOP");return(RELOP);}
"!="        {yylval=CreatNode(yytext, NULL, "RELOP");return(RELOP);}
"+"         {yylval=CreatNode(yytext, NULL, "PLUS");return(PLUS);}
"-"         {yylval=CreatNode(yytext, NULL, "MINUS");return(MINUS);}
"*"         {yylval=CreatNode(yytext, NULL, "STAR");return(STAR);}
"/"         {yylval=CreatNode(yytext, NULL, "DIV");return(DIV);}
"&&"        {yylval=CreatNode(yytext, NULL, "AND");return(AND);}
"||"        {yylval=CreatNode(yytext, NULL, "OR");return(OR);}
"."         {yylval=CreatNode(yytext, NULL, "DOT");return(DOT);}
"!"         {yylval=CreatNode(yytext, NULL, "NOT");return(NOT);}
"("         {yylval=CreatNode(yytext, NULL, "LP");return(LP);}
")"         {yylval=CreatNode(yytext, NULL, "RP");return(RP);}
"["         {yylval=CreatNode(yytext, NULL, "LB");return(LB);}
"]"         {yylval=CreatNode(yytext, NULL, "RB");return(RB);}
"{"         {yylval=CreatNode(yytext, NULL, "LC");return(LC);}
"}"         {yylval=CreatNode(yytext, NULL, "RC");return(RC);}
{oct}		 {yylval=CreatNode(yytext, yytext, "INT");return INT;}
{int}		 {yylval=CreatNode(yytext, yytext, "INT");return INT;}
{float}		 {yylval=CreatNode(yytext, yytext, "FLOAT");return FLOAT;}
{hex}		 {yylval=CreatNode(yytext, yytext, "INT");return INT;}
{id}		 {yylval=CreatNode(yytext, NULL, "ID");return ID;}
.			{err_handle();}

%%
void err_handle()
{
		  printf("err in lex line %d: '%s",yylineno,yytext);
		  char c = input();
		  //if (c == EOF) return;
		  while(c!=' ' && c!='\t' && c!='\n')
		  {
		    if (c == EOF) break;
		    printf("%c",c);
		    c = input();
		  }
		  printf("'\n");
  
  return;
}
