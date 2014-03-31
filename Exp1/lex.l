%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "mconfig.h"
%}

%option yylineno
delim		[ \t\n]
ws 		{delim}+
digit		[0-9]
int 		[+-]?[1-9]{digit}*
float		[+-]?([1-9]{digit}*(\.)?{digit}*)|({digit}*\.{digit}+)
hex			0(x|X)[0-9a-fA-F]+
oct			0[0-7]+
letter		[_A-Za-z]
id			{letter}({letter}|{digit})*
reserved_word	("int"|"float"|"bool"|"if"|"else"|"do"|"while"|"return")
op 			("+"|"-"|"*"|"/"|"&"|"&&"|"|"|"||"|"%"|"^"|"<="|">="|"="|"=="|"+="|"("|")"|",")
bool		("true"|"false")
%%

{ws}		{/* do nothing */}
"/*"([*]*(([^*/])+([/])*)*)*"*/"	{/* do nothing */}
"//"		{char c = input(); while(c != '\n'){if(c == EOF) break; c=input();}}
{reserved_word}	{if(look_fwd(NO_SIMI)) printf("<%s>\n",up_char(yytext));}
{bool}		{if(look_fwd(SIMI)) printf("<%s>\n",up_char(yytext));}
{op}		{if(look_fwd(NO_SIMI)) printf("<%s>\n",yytext); else err_handle();}
";"			{if(look_fwd(NO_SIMI)) printf("<%s>\n",yytext); else err_handle();}
{oct}		{if (look_fwd(SIMI)) 	printf("<oct,%s>\n",yytext);	else err_handle();}
{int}		{if (look_fwd(SIMI)) 	printf("<int,%s>\n",yytext);	else err_handle();}
{float}		{if (look_fwd(SIMI))	printf("<float,%s>\n",yytext); else err_handle();}
{hex}		{if (look_fwd(SIMI)) 	printf("<hex,%s>\n",yytext);	else err_handle();}

{id}		{if (look_fwd(SIMI))	printf("<id,%s>\n",yytext); else err_handle();}
.			{err_handle();}
%%

bool look_fwd(int flag)
{
	//printf("before input yytext %s yyleng %d\n",yytext,yyleng);	//debug
	char tmp = input();
	//printf("after yytext %s yyleng %d\n",yytext,yyleng);		//debug
	unput(tmp);	
	//printf("then yytext %s yyleng %d\n",yytext,yyleng);	//debug
	yytext[yyleng] = '\0';
	//printf("final yytext %s yyleng %d\n",yytext,yyleng);	//debug
	//printf("in look_fwd %c\n",tmp);	//debug
	if (flag == NO_SIMI)
		if(strcmp(yytext,"return")!=0)
			if(tmp == ';')
			{
				//printf("return false\n");
				err_handle();
				return false;
			}
	if(in_follow_set(tmp))
	{
		//printf("true in look_fwd\n");		//debug
		return true;
	}
	else
	{
		//yymore();
		//printf("false yytext %s yyleng %d\n",yytext,tmp);
		return false;
	}
}

bool in_follow_set(const char c)
{
	/*
	if((c >=0x25 && c <=0x2f) || c == ' ' || \
		c == 0x3b || c == '\n' || c == '!' || c == '\t')
		if(c != 0x27 && c != 0x2e)
			return true;
	*/

	if(c == ' ' || c == '\t' || c==';' ||\
	   c == '\n')
		return true;

	return false;
}

void err_handle()
{
	printf("err in line %d: '%s",yylineno,yytext);
	char c = input();
	//if (c == EOF)	return;
	while(c!=' ' && c!='\t' && c!='\n')
	{
		if (c == EOF)	break;
		printf("%c",c);
		c = input();
	}
	printf("'\n");
	
	return;
}

char * up_char(char *buf)
{
	char *tmp = buf;
	while(*tmp !='\0')
	{
		*tmp = 'A' - 'a' + (*tmp++);
	}
	return buf;
}


int main(int argc, char* argv[])
{
	if(argc == 1)
		yyin = fopen("./example.txt","r");
	else
	{
		assert(argc >= 2);
		if(!(yyin = fopen(argv[1],"r")))
		{
			printf("err in open\n");
			exit(1);
		}
	}
	yyrestart(yyin);
	yylex();
	fclose(yyin);
	return 0;
	
};
