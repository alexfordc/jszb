#ifndef TG_INDICATOR_LEXER_H
#define TG_INDICATOR_LEXER_H

namespace tg {

enum Token {
	TK_ID, /* ��ʶ�� [a-zA-Z]([a-zA-Z0-9])* */
	TK_INT, /* ���� [0-9]+ */
	TK_DECIMAL, /* С�� [0-9]*\.[0-9]+ */
	TK_COLON, /* : */
	TK_COLON_EQ, /* := */
	TK_SEMICOLON, /* ; */
	TK_LP, /* ( */
	TK_RP, /* ) */
	TK_COMMA, /* , */
	TK_ADD, /* + */
	TK_SUB, /* - */
	TK_MUL, /* * */
	TK_DIV, /* / */
	TK_EOF, /* ��ʾ���� */
	TK_ERR, /* ��ʾ���� */
	TK_NONE, /* ��ʾʲô������ */
	TK_ALL
};

struct Lexer {
	char *code; /* ȫ�������ڴ�Ĵ��� */
	int capacity;
	int size;
	int cursor;
	int lineno; /* �к� */
	int charpos; /* һ��֮�ڵ��ַ�λ�� */
};

typedef struct Lexer Lexer;

Lexer *lexerNew();
void lexerFree(Lexer *l);

int lexerReadFile(Lexer *l, const char *filename);
int lexerRead(Lexer *l, const char *str, int len);

enum Token lexerGetToken(Lexer *l, char **value, int *len);

}

#endif