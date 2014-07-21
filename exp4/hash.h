#ifndef __HASH_H_
#define __HASH_H_

#include "list.h"
#include "mconfig.h"
#include <stdio.h>
#include <assert.h>
#define TableSize 16384
struct list_head FuncListHead;

typedef struct IdNode
{
	char *name;
	char *type;
	char *reg;
	int father;
	bool init;	//是否初始化，1 已经初始化，0，未初始化
	union
 		{
			//int i;
			float f;
			//char * s;
		}val;
	struct list_head InSList;	//同作用域中的下一个
	struct list_head HashList;	//hash表中的下一个
};
typedef struct IdNode IdNode;
struct list_head IdTable[TableSize];

typedef struct FuncNode
{
	char *name;
	char *type;
	struct list_head ScopeListHead;
	struct list_head FuncList;
};
typedef struct FuncNode FuncNode;

typedef struct ScopeNode
{
	int ID;
	//char *type;	//
	struct list_head ScopeList;		//作用域链表
	struct list_head ScopeNodeList;	//同作用域内节点链表
};
typedef struct ScopeNode ScopeNode;


void init();	//初始化hashtable，链表等
unsigned int hash_pjw(char* name);	//取hash值
void AddIdNodeList(IdNode *node);	//将Id加到符号表中
void AddScopeNode();	//新增一个作用域，在遇到NewScope时调用
void DelScope();		//删除一个作用域，在EndScope时调用
bool ReDefine(IdNode *node);	//检查是否重复定义，重复定义返回true
IdNode* CheckDefine(char *name);	//检查是否定义，若已经定义返回true
void analysis(TreeNode *root);		//遍历AST分析
void analysisDefSeq(TreeNode *root);
void analysisParam(TreeNode *root);
void analysisDef(TreeNode *root);
void analysisFun(TreeNode *root);
TreeNode* analysisExp(TreeNode *root);
int CalcuNode(TreeNode *root);		//判断节点是否为运算符
bool CheckInit(char *name);		//检查是否初始化
void InitIdNode(char *name, float i);	//初始化或更新符号表节点
FuncNode *GetFirstFunNode();
struct list_head *GetScopeHead();
void analysisReturn(TreeNode *root);

int ID;

//reg func
char *Rstack[9];
char *ParamRstack[4];
int RstackTop; //point to a free reg
void GetReg(char **reg);
void FreeReg(char **reg);
void GenFuncCode(TreeNode *root);
int ParamRegFlag;

#endif
