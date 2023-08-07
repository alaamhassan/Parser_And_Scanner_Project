#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H
#include<string>
#include<vector>

typedef struct SYNTAXTREE_STRUCT
{
	std::string Node_name;
	std::vector<SYNTAXTREE_STRUCT*> children;
	SYNTAXTREE_STRUCT* neighbour;
	bool isStatement; //1 if node is statement, 0 if expression
}SyntaxTree_T;

SyntaxTree_T* init_SyntaxTree(std::string Node_name, std::vector<SyntaxTree_T*> children, SyntaxTree_T* neighbour, bool isStatement);

#endif