// SmartParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <String>
#include <list>
#include <map>
#include "Tokenizer.h"

struct Element {
};

struct Object;

struct Adjective: public Element
{
    int val;
};

struct Object: public Element
{
    std::map<std::string, Adjective*> list;
    Object() 
    {
    }
    ~Object()
    {
    }

 };

struct Synonym
{
    std::list<Element*> list;
    Synonym() {
    }
    void addElement(Element* element)
    {
        list.push_back(element);
    }
};

std::map<std::string, Synonym> synonyms;
std::map<std::string, Adjective> adjetives;
std::map<std::string, Object> objects;


void printAdjective(Object* o)
{
    printf("(");

    for (auto p = o->list.begin(); p!=o->list.end(); p++)
    {
        std::cout<< p->first << ", ";
    }
    printf(")");
}

void printObject()
{
    for (auto o = objects.begin(); o!=objects.end(); o++)
    {
        std::cout << o->first<< " ";
        printf(", ");
    }
    printf(".\n");
}

struct synonyms{
    synonyms *list;
    synonyms* next;
};

int main()
{
    Object* o;
    const char* s = R"(
define pedro
define jose
pedro is tall
is pedro tall
show objects
show pedro props
)";

    /*
define synonyms rich, wealthy, afluent
define synonyms beautiful, attractive
pedro is tall
pedro is rich
rich is like walthy
pedro is white
show pedro props
*/
    int n, p;
    Token l;
    //char s[255];
    Tokenizer tk;
    tk.setSource(s);

    for(;;)
    {
        tk.scan();
        if (tk.curToken.type == TK_EOF) break;

        switch (tk.curToken.type) 
        {
        case 
        case TK_IDENTIFIER:
            l = tk.curToken;
            tk.scan();
            switch (tk.curToken.type) {
            case TK_IS:
                adjetives[tk.nextToken.text] = Adjective();
                objects[l.text].list[tk.nextToken.text] = &adjetives[tk.nextToken.text];
                tk.scan();
                break;
            }       
            break;
        case TK_DEFINE:
            tk.scan();
            objects[tk.curToken.text]=Object();
            break;
        case TK_SHOW:
            switch (tk.nextToken.type) {
            case TK_OBJECTS: printObject(); tk.scan(); break;
            case TK_IDENTIFIER:
                tk.scan();
                switch (tk.nextToken.type) {
                case TK_PROPS:
                    o = &objects[tk.curToken.text];
                    if (o) {
                        printAdjective(o);
                    }
                    else {
                        printf("Object not found");
                    }
                }
                break;
            }
            break;
        }
    }

    //std::cin >> s;
}
