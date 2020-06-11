#ifndef PARSER_H
#define PARSER_H

typedef enum TTOKEN :char
{
	//general tokens
	TK_HALT = -1, TK_NEWLINE = '\n', TK_IDENTIFIER, TK_EOF, TK_NULL,
	//data types
	TK_DEFINE,
	//types
	TK_FLOAT, TK_INTEGER, TK_ERRORPARSE, TK_STRING,
	//
	TK_DIV, TK_OPENPARENTHESIS, TK_CLOSEPARENTHESIS, TK_EQEQ, TK_EQ, TK_DIF, TK_NOT,
	TK_GE, TK_GT, TK_LE, TK_LT, TK_DEC, TK_LESSLESS, TK_MINUS, TK_INC, TK_PLUSPLUS,
	TK_PLUS, TK_MULT, TK_POWER, TK_SEMICOLON, TK_SHOW, TK_OBJECTS, TK_PROPS, TK_IS, TK_BYE, TK_QUIT, TK_EXIT
} TTOKEN;

//character type. The possible classification for the characters found on the source string
enum CTYPE : unsigned char{
	ttError,
	ttWhitespace,
	ttSpecial,
	ttLeter,
	ttDigit
};

//Usefull way to store all the reserved words of the language
struct Reserved {
	const char* name;
	TTOKEN type;
};

const int MAX_TOKKEN_LENGTH = 35;
//store a token. The token text and type
struct Token
{
	char text[MAX_TOKKEN_LENGTH];//store the text found in the sourse
	int  length;			//lenth
	TTOKEN type;			//the type of token it is
	int col, line;
};

class Tokenizer
{
	CTYPE chcomp[127];
	const char* s;		//pointer to the zero terminate string that contains the code
public:
	Token	curToken,
			nextToken,		//current token read
			lasttoken;	//previews token read
	int		col, line;	//column and line number
public:
	void init();
	Tokenizer();
	void setSource(const char *sourse);
	TTOKEN isKeyWord(const Token& toke) const;
	void getNumber();
	void getIdentifier();
	void parseString(const char delimiter);
	void parseSpecial();
	void next();
	void skipUntil(const char *delimiter);
	char nextChar();
	void scan();
	void newLine();
};

#endif
