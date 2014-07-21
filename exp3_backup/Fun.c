#include "mconfig.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


TreeNode* CreatNode(char *name, char *context, char* type)
{
  if(debug)
    printf("CreateNode %s\n",context);
  TreeNode *tmp = (TreeNode *) malloc(sizeof(TreeNode));
  tmp->terminal = 1;
  tmp->sibling = NULL;
  tmp->child = NULL;
  tmp->name = NULL;
  tmp->type=NULL;
  tmp->name = MyStrCpy(name);
  
  tmp->type = MyStrCpy(type);
  //tmp->type = type;
  assert(strcmp(tmp->name,name)==0);
  

  if(strcmp(type,"INT")==0)
    tmp->val.f = atoi(context);
  else  if(strcmp(type,"FLOAT")==0)
    tmp->val.f = atof(context);
  else  if(context != NULL)
    tmp->val.s = MyStrCpy(context);
    /*
  switch(type)
  {
    case INT: tmp->val.i = atoi(context); break;
    case FLOAT: tmp->val.f = atof(context); break;
    default:  if(context != NULL) MyStrCpy(tmp->val.s=NULL, context);
  }*/

  return tmp;
}

TreeNode *CreatNonTreeNode(char *name)
{
    if(debug)
      printf("CreateNonTreeNode %s\n",name);
    TreeNode *tmp = (TreeNode *) malloc(sizeof(TreeNode));
    if(tmp == NULL)
      printf("err in malloc\n");
    tmp->terminal = 0;
    tmp->sibling = NULL;
    tmp->child = NULL;
    //tmp->buf = NULL;
    //tmp->name = NULL;
    //tmp->buf = malloc((strlen(context)+1)*sizeof(char));
    //strcpy(tmp->buf,context);
    tmp->name = MyStrCpy(name);
    DEBUG printf("name is %s\n", tmp->name);
    assert(strcmp(tmp->name,name)==0);
    return tmp;
}

TreeNode *LastSibling(TreeNode *p)
{
      TreeNode *q = p;
      while(q->sibling!=NULL)
            q = q->sibling;
      return q;
}

void AddNode(TreeNode *parent, ...)
{
      if(debug)
        printf("in AddNode parent is %s\t",parent->name);
      //DEBUG("in AddNode parent is %s\t",parent->buf);
      va_list VarList;
      va_start(VarList, parent);
      TreeNode *tmp = va_arg(VarList,TreeNode*);
      while(tmp == guard)
            tmp = va_arg(VarList,TreeNode*);
      if(!parent->child)
      {
            parent->child = tmp;
            tmp = va_arg(VarList,TreeNode*);
      }
      TreeNode *p = parent->child;
      if(debug)
        printf("child is %s\t",p->name);
      //DEBUG("child is %s\t",p->buf);
      while(tmp != NULL)
      {
              if(tmp != guard)
              {
                 TreeNode *q = LastSibling(p);
                 assert(q->sibling == NULL);
                 q->sibling = tmp;
                 p = tmp;
                 if(debug)
                    printf("sibling is %s\t",q->sibling->name);
                 //DEBUG("sibling is %s\t",q->sibling->buf);
              }
              tmp = va_arg(VarList,TreeNode*);
              
      }
      va_end(VarList);
      DEBUG printf("\n");
}

void AddSibling(TreeNode* s1, ...)
{
      if(debug)
        printf("in AddSibling s1 is %s\t",s1->name);
      //DEBUG("in AddSibling s1 is %s\t",s1->buf);
      va_list VarList;
      va_start(VarList, s1); 
      TreeNode *tmp = va_arg(VarList,TreeNode*);
      TreeNode *p = s1;
      while(tmp)
      {
             if(tmp != guard)
              {
                 TreeNode *q = LastSibling(p);
                 assert(q->sibling == NULL);
                 q->sibling = tmp;
                 p = tmp;
                 if(debug)
                    printf("sibling is %s\t",q->sibling->name);
              }
              tmp = va_arg(VarList,TreeNode*);
             
      }
      DEBUG printf("\n");
      va_end(VarList);
} 

void GetAttributes(TreeNode* s1, TreeNode* s2)
{
    assert(strcmp(s1->type,"INT")||strcmp(s1->type,"FLOAT"));
    s2->val.s = MyStrCpy(s1->type);
}

void TravelTree(TreeNode* root, int lev)
{
      if(wrong) return;
      TreeNode* p = root;
      //printf("root in %x\n",p);
      while(p != NULL)
      {     
            if(strcmp(p->name,"Exp")==0)
            {
              TravelTree(p->child,lev);
              p=p->sibling;
              continue;
            }
            
          
            else if(strcmp(p->name,"NewScope")==0||strcmp(p->name,"EndScope")==0)
            {
                p = p->sibling;
                continue;
            }

            else if(strcmp(p->name,"FunNode")==0)
            {
              TravelTree(p->child,lev);
              p=p->sibling;
              continue;
            }

            else if(strcmp(p->name,"FunDec")==0)
            {
              TravelTree(p->child,lev);
              p=p->sibling;
              continue;
            }
			
            else if(strcmp(p->name,"DefSeq")==0)
            {
              TravelTree(p->child,lev);
              p=p->sibling;
              continue;
            }
			
            
            //printf("in while\n");   //debug
            int count = lev;
            while(count--)
                  printf("  ");
            printf("%s\n",p->name);
            if(p->child);
                  TravelTree(p->child,lev+1);
            p = p->sibling;
      }
}

char* MyStrCpy(char *src)
{
    //assert(dst == NULL);
    assert(src != NULL);
    DEBUG printf("in MyStrCpy src is %s\n",src);
    char* dst = malloc((strlen(src)+1)*sizeof(char));
    return strcpy(dst,src);
    //DEBUG printf("size is %d dst is %s\n",(strlen(src)+1),dst);
}

/*
bool look_fwd(int flag)
{
  //printf("before input yytext %s yyleng %d\n",yytext,yyleng); //debug
  char tmp = input();
  //printf("after yytext %s yyleng %d\n",yytext,yyleng);    //debug
  unput(tmp); 
  //printf("then yytext %s yyleng %d\n",yytext,yyleng); //debug
  yytext[yyleng] = '\0';
  //printf("final yytext %s yyleng %d\n",yytext,yyleng);  //debug
  //printf("in look_fwd %c\n",tmp); //debug
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
    //printf("true in look_fwd\n");   //debug
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
  

  if(c == ' ' || c == '\t' || c==';' ||\
     c == '\n')
    return true;

  return false;
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
*/



