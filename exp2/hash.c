#include "hash.h"
#include <malloc.h>
#include <math.h>

unsigned int hash_pjw(char* name)
{
    unsigned int val = 0,i;
    for(; *name; ++name){
        val = (val << 2) + *name;
        if(i = val & ~0x3fff)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

void init()
{
	ID = 0;
	INIT_LIST_HEAD(&ScopeListHead);
	//AddScopeNode();		//全局变量或函数变量
	int i;
	for(i = 0; i < TableSize; i++)
	{
		INIT_LIST_HEAD(&IdTable[i]);
	}
}

void AddIdNodeList(IdNode *node)	
//将节点插入到hash表中，同时放入对应的函数作用域(链表)中
{
	DEBUG printf("in AddIdNodeList node is %s\n", node->name);
	assert(!list_empty(&ScopeListHead));
	struct list_head *plist = ScopeListHead.next;
	ScopeNode *ptr;
	ptr = list_entry(plist, ScopeNode, ScopeList);
	node->father = ptr->ID;
	node->init = 0;
	if(ReDefine(node))		//重复定义，退出
		return;
	int val = hash_pjw(node->name);
	list_add(&node->HashList,&IdTable[val]);
	list_add_tail(&node->InSList,&ptr->ScopeNodeList);
}

void AddScopeNode()	//新加入一个作用域节点
{
	DEBUG printf("in AddScopeNode\n");
	ScopeNode *ptr = malloc(sizeof(ScopeNode));
	ptr->ID = ID++;
	INIT_LIST_HEAD(&ptr->ScopeList);
	INIT_LIST_HEAD(&ptr->ScopeNodeList);
	list_add(&ptr->ScopeList, &ScopeListHead);
	DEBUG	printf("after AddScopeNode\n");
}

void DelScope()
{
	DEBUG printf("in DelScopeNode\n");
	assert(!list_empty(&ScopeListHead));
	struct list_head *plist = ScopeListHead.next;
	ScopeNode *ptr;
	ptr = list_entry(plist, ScopeNode, ScopeList);
	list_for_each(plist, &ptr->ScopeNodeList)
	{
		IdNode *p = list_entry(plist,IdNode,InSList);
		list_del(&p->InSList);
		list_del(&p->HashList);
		free(p);
	}
	assert(list_empty(&ptr->ScopeNodeList));
	list_del(&ptr->ScopeList);
	free(ptr);
}

bool ReDefine(IdNode *node)		//检查在同一作用域，是否重复定义
{
	DEBUG printf("in ReDefine\n");
	int val = hash_pjw(node->name);
	if(list_empty(&IdTable[val]))
		return false;
	struct list_head *plist = IdTable[val].next;
	IdNode *ptr = list_entry(plist, IdNode, HashList);
	if(ptr->father==node->father && strcmp(ptr->name,node->name)==0)
	{
		printf("ID %s redefine!!!\n",ptr->name);
		return true;
	}
	return false;
}

bool CheckInit(char *name)
{
	IdNode *ptr=CheckDefine(name);
	if(ptr==NULL)
		return false;
	else
	{
		if(ptr->init)
			return true;
		else 
		{
			printf("Use un-init ID %s!!!\n",name);
			return false;
		}
	}
}

IdNode* CheckDefine(char *name)	//如果定义，返回该IdNode，未定义返回NULL
{
	int val = hash_pjw(name);
	if(list_empty(&IdTable[val]))
	{
		printf("Undefined ID %s!!!\n",name);
		return NULL;
	}
	struct list_head *plist = IdTable[val].next;
	IdNode *ptr = list_entry(plist, IdNode, HashList);
	assert(strcmp(ptr->name,name)==0);
	return ptr;
}

void InitIdNode(char *name,float i)
{
	int val = hash_pjw(name);
	IdNode *ptr=CheckDefine(name);
	ptr->init = 1;
	ptr->val.f = i;
}
/*

DecList     :   Dec		      {$$=$1;}               
            |   Dec COMMA DecList	{$$=$1; AddSibling($1, $3, NULL);} 	    
            ;

Dec         :   VarDec		      {$$=$1;}     //VarDec：变量或数组   
            |   VarDec ASSIGNOP Exp	{$$=$1; TreeNode *ptr = CreatNonTreeNode("Exp");AddNode(ptr,$3,NULL);AddNode($1, $2, ptr, NULL);} 	 
            ;

ExtDefList  :   ExtDef ExtDefList	{$$=$2; AddNode($$, $1, NULL); }     
            | 	        {$$=CreatNonTreeNode("DefSeq");}           
            ;
ExtDef      :   Specifier ExtDecList SEMI  {$$=CreatNonTreeNode("Def"); AddNode($$,$1, $2, NULL);}
            |   Specifier SEMI	{$$=$1;}	      
            |   Specifier FunDec CompSt	{$$=$1; AddSibling($1, $2, $3, NULL);}	
            |   error SEMI 
            ;
ExtDecList  :   VarDec		{$$=$1;}                    
            |   VarDec COMMA ExtDecList	{$$=$1; AddSibling($1,$3,NULL);}       	
            ;
VarDec      :   ID		      {$$ = $1;}         //变量       
            |   VarDec LB INT RB	{$$=CreatNonTreeNode("arrary"); AddNode($$,$1,$3, NULL);}	   //数组？？ 

*/
void analysisDefSeq(TreeNode *root)
{
	
	TreeNode *p = root;
	while(p!= NULL)
	{
		DEBUG printf("in analysisDefSeq while loop name is %s\n",p->name);
		if(strcmp(p->name,"DefSeq")==0)
		{
			DEBUG printf("in analysisDefSeq ");
			//printf("in analysisDefSeq");
			p = p->child;
			if(p!=NULL&&strcmp(p->name,"Def")!=0)		//定义函数
			{
				DEBUG printf("func branch\n");	
				analysis(p);
				return;
				//return;
			}
			else
			{
				if(p==NULL)		//函数体内未定义变量
					return;
				assert(p!=NULL);
				DEBUG printf("Def branch %s\n",p->name);	
				assert(strcmp(p->name,"Def")==0);
				analysisDefSeq(p->child);
				p = p->sibling;
			}
		}
		
		else	if(strcmp(p->name,"Def")==0)
		{
			DEBUG printf("in analysisDefSeq Def branch\n");
			analysisDefSeq(p->child);
			p = p->sibling;
			//assert(p!=NULL);
		}
		else	if(strcmp(p->name,"INT")==0||strcmp(p->name,"FLOAT")==0)
		{
			analysisDef(p);
			//p = p->sibling;
			return;
		}
		else
		{
			printf("err in analysisDefSeq %s\n",p->name);
			while(1);
		}
	}
}

void analysisDef(TreeNode *root)
{
	char *name = root->name;
	int flag;	//int 0 and float 1
	if(strcmp(name,"INT")==0)
		flag = 0;
	else if (strcmp(name,"FLOAT")==0)
		flag = 1;
	else
	{	
		printf("err in analysisDef name is %s\n",name);
		while(1);
	}
	TreeNode *p = root->sibling;
	assert(p!=NULL);
	while(p!=NULL)
	{
		if(strcmp(p->name,"INT")==0||strcmp(p->name,"FLOAT")==0)
		{
			analysisDef(p);
			return;
		}
		IdNode *node = malloc(sizeof(IdNode));
		node->name=MyStrCpy(p->name);
		if(flag)
			node->type=MyStrCpy("FLOAT");
		else	
			node->type=MyStrCpy("INT");
		AddIdNodeList(node);
		p = p->sibling;

	}
}

TreeNode* analysisExp(TreeNode *root)
{
	DEBUG printf("in analysisExp\n");
	TreeNode *p = root;
	if(strcmp(p->name,"Exp")==0)
	{
		DEBUG printf("analysisExp child %s\n", p->child->name);
		return analysisExp(p->child);
	}
	if(strcmp(p->type,"ID")==0)
	{
		DEBUG printf("analysisExp ID branch %s\n", p->name);
		if(!CheckDefine(p->name))	
			return NULL;
		else 
			return	p;

	}
	if(strcmp(p->type,"INT")==0||strcmp(p->type,"FLOAT")==0)
	{
		DEBUG printf("analysisExp number branch %s\n", p->name);
		return p;
	}
	if(CalcuNode(p)!=-1)
	{
		DEBUG printf("in CalcuNode branch type is %s %s\n",p->name,p->type);
		TreeNode *first = analysisExp(p->child);
		TreeNode *second = analysisExp(p->child->sibling);
		if(second==NULL||first==NULL)
			return NULL;
		if(strcmp(first->type,"ID")==0)
			if(!CheckInit(first->name))
				return NULL;
		else if(strcmp(second->type,"ID")==0)
			if(!CheckInit(second->name))
				return NULL;
		TreeNode *tmp = malloc(sizeof(TreeNode));
		assert(p->child->sibling->sibling==NULL);
		float a,b;
		a = first->val.f;
		b = second->val.f;
		DEBUG printf("first and second is %f %f\n", a,b);
		if(strcmp(first->type,"FLOAT")==0||strcmp(second->type,"FLOAT")==0)
		{
			tmp->type=MyStrCpy("FLOAT");
		}
		else
		{
			tmp->type=MyStrCpy("INT");
		}
		switch(CalcuNode(p))
		{
			case 1:tmp->val.f = a+b; break;
			case 2:tmp->val.f = a-b; break;
			case 3:tmp->val.f = a*b; break;
			case 4:DEBUG printf("in div\n"); if(b==0){printf("Div Zero err!!!\n");break;} else {tmp->val.f = a/b; break;}
			default:printf("err in switch CalcuNode\n"); while(1);
		}
		return tmp;
	}
	if(strcmp(p->type,"ASSIGNOP")==0)
	{
		DEBUG printf("in = branch\n");
		TreeNode *first = analysisExp(p->child);
		if(first==NULL)
			return NULL;
		assert(strcmp(first->type,"ID")==0);
		TreeNode *second = analysisExp(p->child->sibling);
		if(second==NULL)
			return NULL;
		if(strcmp(CheckDefine(first->name)->type,"INT")==0&&strcmp(second->type,"FLOAT")==0)
		{
			printf("assign float number %s to int (ID %s) err!!!\n",second->name,first->name);
			return NULL;
		}
		if(strcmp(CheckDefine(first->name)->type,"INT")==0&&strcmp(second->type,"ID")==0)
		{
			if(strcmp(CheckDefine(second->name)->type,"FLOAT")==0)
			{	
				printf("assign float ID %s to int (ID %s) err!!!\n",second->name,first->name);
				return NULL;
			}
		}
		if(strcmp(second->type,"ID")==0)
		{
			//DEBUG	printf("in = branch checkinit\n");
			if(!CheckInit(second->name))
				return NULL;
		}
		//DEBUG printf("first second type %s %s\n", first->type, second->type);
		InitIdNode(first->name,second->val.f);
	}
}
int CalcuNode(TreeNode *root)
{
	if(strcmp(root->type,"PLUS")==0)
		return 1;
	else if(strcmp(root->type,"MINUS")==0)
		return 2;
	else if(strcmp(root->type,"STAR")==0)
		return 3;
	else if(strcmp(root->type,"DIV")==0)
		return 4;
	else return -1;
}
void analysis(TreeNode *root)
{
	if(wrong)
		return;
	DEBUG	printf("in analysis name is %s\n",root->name);
	if(root==NULL)	return;
	TreeNode *p = root;
	while(p != NULL)
	{
		DEBUG printf("in analysis while loop name is %s\n",p->name);
		if(strcmp(p->name,"NewScope")==0)
		{
			AddScopeNode();
		}
		else	if(strcmp(p->name,"EndScope")==0)
		{
			DelScope();
			assert(p->child == NULL);
			assert(p->sibling==NULL);
		}
		else if (strcmp(p->name,"DefSeq")==0)
		{
			analysisDefSeq(p);		//不用分析child,analysisDefSeq已经分析
			p = p->sibling;
			//assert(strcmp(p->name,"EndScope")==0||p==NULL);
			continue;
		}
		else if(strcmp(p->name,"ParamS")==0)		//定义变量
		{
			assert(strcmp(p->child->name,"INT")==0||strcmp(p->child->name,"FLOAT")==0);
			analysisDef(p->child);
			p = p->sibling;
			continue;
		}
		else if(strcmp(p->name,"Exp")==0)
		{
			analysisExp(p);
			p = p->sibling;
			continue;
		}
		if(p->child!=NULL)
		{
			DEBUG printf("before analysis child\n");
			analysis(p->child);
		}
		p = p->sibling;
	}

}