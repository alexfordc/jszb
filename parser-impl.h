#ifndef TG_INDICATOR_PARSER_IMPL_H
#define TG_INDICATOR_PARSER_IMPL_H

namespace tg {

/* ------ AST��ʼ ------ */

enum NodeType {
	NT_NONE = TK_ALL + 1, 
	NT_FORMULA,
	NT_STMT,
	NT_EXPR,
	NT_INT,
	NT_DECIMAL,
	NT_ID,
	NT_FUNC_CALL,
	NT_BINARY_EXPR,
	NT_ALL
	
};

struct Node {
	void (*clean)(Node *node);
};

struct Stmt;
struct Expr;

struct Formula {
	struct Node node;
	Array stmts;
};

struct Stmt {
	struct Node node;
	String id;
	enum Token op; /* TK_COLON_EQ/TK_COLON */
	Node *expr;
};

struct IntExpr {
	struct Node node;
	int64_t val;
};

struct DecimalExpr {
	struct Node node;
	double val;
};

struct IdExpr {
	struct Node node;
	String val;
};

struct ExprList {
	struct Node node;
	Array exprs;
};

struct FuncCall {
	struct Node node;
	String id;
	ExprList *args;
};

struct BinaryExpr {
	struct Node node;
	Node *lhs;
	enum Token op;
	Node *rhs;
};

/* ------ AST���� ------ */

class Parser {
public:
	Lexer *lex;
	
	enum Token tok;
	char *tokval; /* ָ��ǰtoken��ֵ(���ڻ�������lex��,û�����ַ�'\0'��β) */
	int toklen; /* ��ǰtoken�ĳ��� */
	
	void *userdata;
	int (*handleError)(int lineno, int charpos, int error, const char *errmsg, void *);
	int errcount;
	bool isquit;
	
	Formula *ast;
};

}

#endif
