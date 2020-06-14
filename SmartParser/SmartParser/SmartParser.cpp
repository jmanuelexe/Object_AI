// SmartParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <String>
#include <list>
#include <map>
#include "Tokenizer.h"

struct Object
{
    std::map<std::string, Object*> list;
    Object() 
    {
    }
    ~Object()
    {
    }

 };
//
//struct Synonym
//{
//    std::list<Element*> list;
//    Synonym() {
//    }
//    void addElement(Element* element)
//    {
//        list.push_back(element);
//    }
//};
//
//std::map<std::string, Synonym> synonyms;
//std::map<std::string, Adjective> adjetives;
std::map<std::string, Object> objects;


void printAdjective(Object& o)
{
    printf("(");

    for (auto p = o.list.begin(); p!=o.list.end(); p++)
    {
        std::cout<< p->first << ", ";
    }
    printf(")\n");
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

#define ERROR(str) std::cout << "I dont know " << str << " please define\n"; tk.scan();
//parse definitions
void parseDefine(Tokenizer& tk)
{
    tk.scan();
    objects[tk.curToken.text] = Object();
    std::cout << "ok.\n";
}

int main()
{
    std::map<std::string, Object>::iterator o;
    std::string s;
    /*const char* s = R"(
define pedro
define jose
pedro is tall
is pedro tall
show objects
show pedri props
)";*/
    /*
define synonyms rich, wealthy, afluent
define synonyms beautiful, attractive
pedro is tall
pedro is rich
rich is like walthy
pedro is white
*/
    Tokenizer tk;
    std::cout <<"Usage'\n";
    std::cout <<"   create object: define [object name]\n";
    std::cout <<"   create object propertie: [object name] is [object name]'\n";
    std::cout <<"   ask about propertines: is [object name] [object name]\n";
    std::cout <<"   show propertines of and object: show [object name] props\n";
    std::cout <<"   show all objects: show objects\n";
    std::cout <<"type bye,quit exit to exit the application\n";
    for(;;)
    {
       again:
        std::getline(std::cin, s);
        tk.setSource(s.c_str());
        tk.scan();
        if (tk.curToken.type == TK_EOF) break;

        switch (tk.curToken.type) 
        {
        case TK_EXIT:
        case TK_QUIT:
        case TK_BYE: return 0;
        case TK_IS:
            tk.scan();
            if (tk.nextToken.type==TK_IDENTIFIER) {
                o = objects.find(tk.curToken.text);
                if (o == objects.end()) {
                    ERROR(tk.curToken.text);
                    break;
                }
                auto ii=o->second.list.find(tk.nextToken.text);
                if (ii == o->second.list.end()) {
                    std::cout << "no\n";
                    break;
                }
                std::cout << "yes\n";
                tk.scan();
            }
            break;
        case TK_IDENTIFIER:
            tk.scan();
            switch (tk.curToken.type) {
            case TK_IS:
                o = objects.find(tk.lasttoken.text);
                if (o == objects.end()) {
                    ERROR(tk.lasttoken.text);
                    break;
                }
                o->second.list[tk.nextToken.text] = &objects[tk.nextToken.text];
                tk.scan();
                std::cout <<"ok.\n";
                break;
            default:std::cout << "\nI don't understand.\n"; break;
            }
            break;
        case TK_DEFINE:
            parseDefine(tk);            
            break;
        case TK_SHOW:
            switch (tk.nextToken.type) {
            case TK_OBJECTS: 
                printObject(); tk.scan(); 
                std::cout << "ok.\n"; 
                break;
            case TK_IDENTIFIER:
                tk.scan();
                switch (tk.nextToken.type) {
                case TK_PROPS:
                    o = objects.find(tk.curToken.text);
                    if (o!= objects.end()) {
                        printAdjective(o->second);
                    }
                    else {
                        ERROR(tk.curToken.text);
                        break;
                    }
                    tk.scan();
                    std::cout <<"ok.\n";
                default:std::cout << "\nI don't understand.\n"; goto again; break;
                }
                break;
            default:std::cout <<"\nI don't understand.\n"; goto again; break;
            }
            break;
        default:std::cout <<"\nI don't understand.\n"; break;
        }
    }
}
