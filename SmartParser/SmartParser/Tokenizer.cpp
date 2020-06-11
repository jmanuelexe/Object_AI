#include "Tokenizer.h"
#include <string.h>

Reserved row2[] = {		//reserved words with lenth of two characters
	{"is", TK_IS},
	/*{ "if", TK_IF },
	{ "do", TK_DO },
	{ "or", TK_OR },
	{ "to", TK_TO },*/
	0
};
Reserved row3[] = {	//reserved words with lenth of three characters
	/*{ "for", TK_FOR },
	{ "end", TK_END },
	{ "not", TK_NOT },
	{ "and", TK_AND },*/
	0
};
Reserved row4[] = {
	{"show", TK_SHOW},
	/*{ "then", TK_THEN },
	{ "true", TK_TRUE },
	{ "case", TK_CASE },
	{ "else", TK_ELSE },
	{ "goto", TK_GOTO },*/
	0
};
Reserved row5[] = {
	{"props", TK_PROPS},
	/*{ "false", TK_FALSE },
	{ "while", TK_WHILE },
	{ "break", TK_BREAK },
	{ "label", TK_LABEL },
	{ "range", TK_RANGE },
	{ "class", TK_CLASS },
	{ "begin", TK_BEGIN },
	{ "endif", TK_ENDIF },*/
	0
};
Reserved row6[] = {
	{ "define", TK_DEFINE},	
	0
};

Reserved row7[] = {
	{ "objects", TK_OBJECTS},
	0
};

Reserved row8[] = {
	/*{ "continue", TK_CONTINUE },
	{ "function", TK_FUNCTION },
	{ "endwhile", TK_ENDWHILE },*/
	{ 0 }
};

Reserved *Keyboards[] = { NULL, NULL, row2, row3,row4, row5, row6, row7, row8 };//makesure the last keyboard is zero as string

void Tokenizer::init()
{
	int n;

	for (n = 0; n < 64; n++) chcomp[n] = ttError;
	for (n = 'A'; n <= 'Z'; n++) chcomp[n] = ttLeter;
	for (n = 'a'; n <= 'z'; n++) chcomp[n] = ttLeter;
	for (n = '0'; n <= '9'; n++) chcomp[n] = ttDigit;
	chcomp['_'] = ttLeter;

	chcomp['\n'] = chcomp['\t']= chcomp[' '] = ttWhitespace;

	chcomp['.'] = chcomp['{'] = chcomp['}'] =
	chcomp[')'] = chcomp['('] = chcomp[','] = chcomp['='] = 
	chcomp['>'] = chcomp['<'] = chcomp['\'']= chcomp['"'] = 
	chcomp['^'] = chcomp[';'] = chcomp['+'] = chcomp['-'] = 
	chcomp['['] = chcomp[']'] = chcomp['-'] = chcomp['!'] =
	chcomp['*'] = chcomp['/'] = ttSpecial;
}

Tokenizer::Tokenizer()
{
	lasttoken.text[0] = curToken.text[0] = nextToken.text[0]= 0;
	lasttoken.type = curToken.type = nextToken.type = TK_NULL;
	lasttoken.col=curToken.col = nextToken.col = TK_NULL;
	s = 0;
	col = line = 1;
	init();
}

TTOKEN Tokenizer::isKeyWord(const Token& token) const
{
	const char* word1;
	Reserved *prw;
	if (nextToken.length >= 2 && nextToken.length <= 8)
	{
		prw = Keyboards[nextToken.length];
		if (prw) {			
			for (; prw->name; ++prw)
			{
				word1 = prw->name;
				if (word1[0] == nextToken.text[0] &&
					!strcmp(word1, nextToken.text)) return prw->type;
			}
		}
	}
	return TK_IDENTIFIER;
}

void Tokenizer::getIdentifier()
{
	char* p = nextToken.text;
	while (*s != NULL) {
		*p++=*s++;
		if (chcomp[*s] < ttLeter)
			break;
	}

	*p = 0;
	nextToken.length = (int)(p - nextToken.text);
	nextToken.type = isKeyWord(nextToken);
}

void Tokenizer::getNumber(){
	char decilmalpoint = 0;
	char* p = nextToken.text;
	while (s != NULL) {
		*p++=*s++;
		if (*s == '.') decilmalpoint++;
		if (chcomp[*s] != ttDigit && *s != '.')	break;
	}

	*p = 0;
	nextToken.length = (int)(p - nextToken.text);
	if (decilmalpoint) nextToken.type = TK_FLOAT;
	else nextToken.type = TK_INTEGER;
}

void Tokenizer::parseString(char delimiter)
{ 
	char* p = nextToken.text;

	while (*s++ != delimiter)
	{
		*p++=*s++;
	}
	nextToken.length = (int)(p - nextToken.text);
}

//skip until finds a string specified as delimiter
void Tokenizer::skipUntil(const char *delimiter)
{
	const char *d = delimiter;
	
	do {
		nextChar();
		if (*d == *s)
			d++;
		else if (d != delimiter) d = delimiter;

	} while (*d!='\0');
	nextChar();
}

char Tokenizer::nextChar()
{
	switch (*s++)
	{
	case TK_NEWLINE: line++; col = 1; 	break;
	default:
		col++;
	}
	return *s;
}

void Tokenizer::newLine() 
{
	nextToken.col = col;
	nextToken.line = line;
	line++; col = 0;
}

void Tokenizer::parseSpecial()
{
	char ch, * p = nextToken.text;
	*p++ = ch = *s++;
	
	switch (ch)
	{	
	case '"' :
	case '\'':
		p--;
		while (*s && ch != *s)
		{
			if (*s == TK_NEWLINE) 
			{
				//newLine(); 
				s++; 
			}
			else {
				*p++ = *s++;
				if ((p - nextToken.text) >= MAX_TOKKEN_LENGTH) 
					break;
			}
		}
		if (*s==ch) s++;
		else { nextToken.type = TK_ERRORPARSE; return;
		}
		nextToken.length = int(p - nextToken.text);
		nextToken.type = TK_STRING;
		break;
	case '/' :
		if (*s == '*') {				//long comment
			skipUntil("*/");
			break;
		}else if (*s == '/') {
			skipUntil("\n");			//short comment
			break;
		} else 
			nextToken.type = TK_DIV;
		break;	
	case '(': nextToken.type = TK_OPENPARENTHESIS; break;
	case ')': nextToken.type = TK_CLOSEPARENTHESIS;break;
	case '=': 
		if (*s == '=') {
			*p++ = *s;
			nextToken.type = TK_EQEQ;
		}
		else {
			nextToken.type = TK_EQ;
		}
		break;
	case '!':
		if (*s == '=') {
			*p++=*s;
			nextToken.type = TK_DIF;
			nextChar();
		}
		else nextToken.type = TK_NOT;
		break;
	case '>':
		if (*s == '=') {
			*p++=*s;
			nextToken.type = TK_GE;
			nextChar();
		}
		else nextToken.type = TK_GT;
		break;
	case '<':
		if (*s == '=') {
			*p++ = *s;
			nextToken.type = TK_LE;
			nextChar();
		}
		else nextToken.type = TK_LT;
		break;
	case '-':
		if (*s == '=') {
			*p++ = *s;
			nextToken.type = TK_DEC;
			nextChar();
		}
		else if (*s == '-')
		{
			*p++ = *s;
			nextToken.type = TK_LESSLESS;
			nextChar();
		}
		else nextToken.type = TK_MINUS;
		break;
	case '+':
		if (*s == '=') {
			*p++ = *s;
			nextToken.type = TK_INC;
			nextChar();
		}
		else if (*s == '+')
		{
			*p++ = *s;
			nextToken.type = TK_PLUSPLUS;
			nextChar();
		}
		else nextToken.type = TK_PLUS;
		break;
	case '*':nextToken.type = TK_MULT; break;
	case '//':nextToken.type = TK_DIV; break;
	case '^':nextToken.type = TK_POWER; break;
	case ';': nextToken.type = TK_SEMICOLON; break;
	default:
		nextToken.type = (TTOKEN) ch;
		break;
	}
	
	nextToken.length = (int)(p - nextToken.text);
	*p = 0;
}
/*
Token Tokenizer::nextToken() 
{
	const char* p = s;
	Token ltoken = lasttoken;
	Token t = token;
	Token r;

	int c	= col;
	int l	= line;
	nextToken();
	r = token;
	col		= c;
	line	= l;
	token	= t;
	s = p;
	return r;
}*/

void Tokenizer::setSource(const char* source)
{
	s = source;
	next();
}

void Tokenizer::scan() 
{
	lasttoken = curToken;
	curToken = nextToken;
	next();
}

void Tokenizer::next()
{
	const char* cur;
SKIPCHAR:
		cur = s;
		if (!s || !*s) {
			nextToken.type = TK_EOF; return;
		}

		nextToken.length = 0;
		nextToken.col = col;
		nextToken.line = line;

		switch (chcomp[*s])
		{
		case ttDigit: getNumber();		break;
		case ttLeter: getIdentifier();  break;
		case ttSpecial: parseSpecial(); break;
		case ttWhitespace:
		default:nextChar(); goto SKIPCHAR;
		}
	
	col +=int( s-cur);
}