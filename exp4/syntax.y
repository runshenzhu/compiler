%code requires
  {
    #include "mconfig.h"
    #define YYSTYPE TreeNode *
    /*
    typedef struct YYLTYPE
      {
            int first_line;
            int first_column;
            int last_line;
            int last_column;
      } YYLTYPE;
      
      extern YYLTYPE yylloc;
      */
  }

%{
#include "lex.yy.c"
#include "mconfig.h"
%}

%locations
/* decplared tokens */
%token INT FLOAT TYPE ID SEMI COMMA
%token ASSIGNOP
%token AND OR NOT
%token RELOP
%token PLUS MINUS STAR DIV
%token DOT LP RP LB RB LC RC
%token RETURN IF ELSE WHILE STRUCT


/*handle conflicts*/
%right ASSIGN 
%left ADD SUB 
%left MUL DIV 
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%
/*****************************High-Level Definitions********************/
Program     :   ExtDefList	{$$=$1; root = $$;} //root should be here!!!	          
            ;
ExtDefList  :   ExtDef ExtDefList	
			{
				if($2->child!=NULL) 
					$1->sibling=$2->child; 
				$2->child=$1;
				$$=$2; 
				/*AddNode($$, $1, NULL);*/ }     
            |   /*Empty*/	        {$$=CreatNonTreeNode("DefSeq");}           
            ;
ExtDef      :   Specifier ExtDecList SEMI  {$$=CreatNonTreeNode("Def"); AddNode($$,$1, $2, NULL);}
            |   Specifier SEMI	{$$=$1;}	      
            |   Specifier FunDec CompSt	
            {
                /*
                $$=$1; TreeNode *ptr = $3->child;
                $3->child = ptr->sibling;
                ptr->sibling=$2->child->sibling;
                $2->child->sibling = ptr;
                AddSibling($1, $2, $3, NULL);
                */
                $$=CreatNonTreeNode("FunNode"); TreeNode *ptr = CreatNonTreeNode("NewScope");
                ptr->sibling=$2->child->sibling;
                $2->child->sibling = ptr;
                ptr = CreatNonTreeNode("EndScope");
                AddNode($$, $1, $2, $3, ptr,NULL);
            }
            |   error SEMI
            ;
ExtDecList  :   VarDec		{$$=$1;}                    
            |   VarDec COMMA ExtDecList	{$$=$1; AddSibling($1,$3,NULL);}       	
            ;


/*****************************Specifiers********************/
Specifier   :   TYPE		  {$$=$1;}               
            |   StructSpecifier {$$=$1;}    		       
            ;

StructSpecifier :   STRUCT OptTag LC DefList RC	{$$ = CreatNonTreeNode("StructDef"); AddNode($$, $1, $2, $4, NULL);}	
                |   STRUCT Tag	{$$=$1; AddNode($1,$2,NULL);}	      
                ;

OptTag      :   ID	{$$=$1;}	                    
            |   /*Empty*/	{$$=guard;}                    
            ;

Tag         :   ID	{$$=$1;}	                   
            ;
/*****************************Declarators********************/            
VarDec      :   ID		      {$$ = $1;}         //变量       
            |   VarDec LB INT RB	{$$=CreatNonTreeNode("arrary"); AddNode($$,$1,$3, NULL);}	   //数组？？ 
            |   VarDec LB error INT RB
            ;
FunDec      :   ID LP VarList RP	{$$=CreatNonTreeNode("FunDec"); TreeNode *p = CreatNonTreeNode("ParamS");AddNode(p,$3,NULL);AddNode($$,$1,p,NULL);}	    
            |   ID LP RP		{$$=CreatNonTreeNode("FunDec"); AddNode($$,$1,NULL);} 
                           
            ;
VarList     :   ParamDec COMMA VarList	{$$=$1; AddSibling($1, $3,NULL);}	
            |   ParamDec		      {$$=$1;}        
            ;
ParamDec    :   Specifier VarDec	{$$=$1; AddSibling($1, $2, NULL);}	
             
            ;
/*****************************Statements********************/       
CompSt      :   LC DefList StmtList RC	{$$=CreatNonTreeNode("FucBody"); AddNode($$,CreatNonTreeNode("NewScope"), $2,$3,CreatNonTreeNode("EndScope"),NULL);} //函数体	
            ;
StmtList    :   Stmt StmtList		{$$=$1; AddSibling($1, $2, NULL);}        
            |   /*Empty*/		     {$$=guard;}              
            ;
Stmt        :   Exp SEMI		{$$=$1;}           
            |   CompSt		      {$$=$1;}         
            |   RETURN Exp SEMI	{$$=$1; AddNode($1, $2, NULL);}	        
            |   IF LP Exp RP Stmt ELSE Stmt	{$$ = CreatNonTreeNode("IFSeq");AddNode($$, $3, $5, $7, NULL);}
            |   WHILE LP Exp RP Stmt		{$$=$1; AddNode($1, $3, $5, NULL);}
            |   error Exp SEMI
            ;
/*****************************Local Definition********************/       

DefList     :   Def DefList		    {$$=$2; AddNode($$, $1, NULL); }            
            |   /*Empty*/		    {$$=CreatNonTreeNode("DefSeq");}           
            ;

Def         :   Specifier DecList SEMI	{$$=CreatNonTreeNode("Def"); AddNode($$,$1, $2, NULL);}
               
            ;

DecList     :   Dec		      {$$=$1;}               
            |   Dec COMMA DecList	{$$=$1; AddSibling($1, $3, NULL);} 	    
            ;
Dec         :   VarDec		      {$$=$1;}     //VarDec：变量或数组   
            |   VarDec ASSIGNOP Exp	{$$=$1; TreeNode *ptr = CreatNonTreeNode("Exp");AddNode(ptr,$3,NULL);AddNode($1, $2, ptr, NULL);} 	   
            ;

Exp         :   Exp ASSIGNOP Exp	{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}   	    
            |   Exp AND Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}            
            |   Exp OR Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}          
            |   Exp RELOP Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}      
            |   Exp PLUS Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}    
            |   Exp MINUS Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}           
            |   Exp STAR Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}         
            |   Exp DIV Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$2,NULL); AddNode($2, $1, $3, NULL);}              
            |   LP Exp RP		{$$=$2;}             
            |   MINUS Exp		{$$=CreatNonTreeNode("Exp");AddNode($$,$1,NULL); AddNode($1, $2, NULL);}                  
            |   NOT Exp		      {$$=CreatNonTreeNode("Exp");AddNode($$,$1,NULL); AddNode($1, $2, NULL);}           
            |   ID LP Args RP		{$$=$1; AddNode($1,$3, NULL);}          
            |   ID LP RP		{$$=$1;}             
            |   Exp LB Exp RB		{$$=$1; AddSibling($1,$3, NULL);}      
            |   Exp DOT ID	{$$ = $1; if($1->child) AddSibling($1->child, $2, $3, NULL); else AddSibling($1, $2, $3, NULL);}	 //pay attention           
            |   ID		{$$=CreatNonTreeNode("Exp");AddNode($$,$1,NULL);}                   
            |   INT		{$$=CreatNonTreeNode("Exp");AddNode($$,$1,NULL);}                  
            |   FLOAT		{$$=CreatNonTreeNode("Exp");AddNode($$,$1,NULL);}  
            |   Exp LB error Exp RB              
            ;
Args        :   Exp COMMA Args	{$$=$1; AddSibling($1, $3, NULL);}
            |   Exp		{$$=$1;}                    
            ;

%%
yyerror(char *msg) {
    wrong = true;
	printf("bison err in line %d %s\n",yylineno, msg);
}




