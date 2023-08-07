#include "SYNTAXTREE.h"

SyntaxTree_T* init_SyntaxTree(std::string Node_name, std::vector<SyntaxTree_T*> children, SyntaxTree_T* neighbour, bool isStatement)
{
	SyntaxTree_T* syntaxTree = new SyntaxTree_T();

	syntaxTree->Node_name = Node_name;
	syntaxTree->children = children;
	syntaxTree->neighbour = neighbour;
	syntaxTree->isStatement = isStatement;

	return syntaxTree;
}