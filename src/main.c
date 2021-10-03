#include <iostream>
#include <fstream>
#include <ctype.h>

typedef enum {
    _EOF,
    NEWLINE,
    NUMBER,
    IDENT,
    STRING,
    //Keywords
    LABEL,
	GOTO,
	PRINT,
	INPUT,
	LET,
	IF,
	THEN,
	ENDIF,
	WHILE,
	REPEAT,
	ENDWHILE,
	//Operators.
	EQ,
	PLUS,
	MINUS,
	ASTERISK,
	SLASH,
	EQEQ,
	NOTEQ,
	LT,
	LTEQ,
	GT,
	GTEQ,
	SEMI_COLON
} Type;

typedef struct {
    Type type;
    std::string text;
} token;

class Lexer {
    char curChar;
    int curPos=-1;
    std::string source;
    public:
        Lexer() {
            
        }
        Lexer(std::string source) {
            this->source=source;
            next();
        }
        void next() {
            curPos+=1;
            if(curPos>=source.length()) {
                curChar='\0';
            } else {
                curChar=source[curPos];
            }
        }
        char peek() {
            if(curPos+1>=source.length()) {
                return '\0';
            } else {
                return source[curPos+1];
            }
        }
        void skip_whitespace() {
            while(isspace(curChar)) {
                next();
            }
        }
        void skip_comment() {
            if(curChar=='#') {
                while(curChar!='\n') {
                    next();
                }
            }
        }
        Type check_type(std::string text) {
            if(text=="if") {
                return IF;
            } else if(text=="label") {
                return LABEL;
            } else if(text=="goto") {
                return GOTO;
            } else if(text=="print") {
                return PRINT;
            } else if(text=="let") {
                return LET;
            } else if(text=="input") {
                return INPUT;
            } else if(text=="if") {
                return IF;
            } else if(text=="then") {
                return THEN;
            } else if(text=="endif") {
                return ENDIF;
            } else if(text=="while") {
                return WHILE;
            } else if(text=="repeat") {
                return REPEAT;
            } else if(text=="endwhile") {
                return ENDWHILE;
            } else {
                return IDENT;
            }
        }
        token get_next_token() {
            token tk;
            skip_whitespace();
            skip_comment();
            int start,end;
            std::string str;
            switch(curChar) {
                case '+':
                    tk.type=PLUS;
                    break;
                case '-':
                tk.type=MINUS;
                    break;
                case '*':
                    tk.type=ASTERISK;
                    break;
                case '/':
                    tk.type=SLASH;
                    break;
                case '=':
                    if(peek()=='=') {
                        tk.type=EQEQ;
                    } else {
                        tk.type=EQ;
                    }
                    break;
                case '!':
                    if(peek()=='=') {
                        tk.type=NOTEQ;
                    } else {
                        std::cout << "Error expected != got "<< curChar << std::endl;
                    }
                    break;
                case '>':
                    if(peek()=='=') {
                        tk.type=GTEQ;
                    } else{
                        tk.type=GT;
                    }
                    break;
                case '<':
                    if(peek()=='=') {
                        tk.type=LT;
                    } else {
                        tk.type=LTEQ;
                    }
                    break;
                //Logical operators (not yet implemented)
                /*case '&':
                    break;
                case '|':
                    break;*/
                case '\"':
                    next();
                    start=curPos;
                    while(curChar!='\"') {
                        next();
                    }
                    str=source.substr(start,curPos-start);
                    tk.type=STRING;
                    tk.text=str;
                    break;
                case '\0':
                    tk.type=_EOF;
                    break;
                case ';':
                    tk.type=SEMI_COLON;
                default:
                    if(isdigit(curChar)) {
                        start=curPos;
                        while(isdigit(peek())) {
                            next();
                        }
                        if(peek()=='.') {
                            next();
                            if(!isdigit(peek())) {
                                std::cout << "Illegal character on number." << std::endl;
                            }
                            while(isdigit(peek())) {
                                next();
                            }
                        }
                        str=source.substr(start,curPos-start+1);
                        tk.type=NUMBER;
                        tk.text=str;
                    } else if(isalpha(curChar)) {
                        start=curPos;
                        while(isalnum(peek())) {
                            next();
                        }
                        str=source.substr(start,curPos-start+1);
                        tk.type=check_type(str);
                        tk.text=str;
                    }
                    break;
            }
            next();
            return tk;
        }
        //Debug functions
        char retCurChar() {
            return curChar;
        }
};


class Parser {
        Lexer lexer;
        token curToken;
        token peek_token;
    public:
        Parser(Lexer lexer) {
            this->lexer=lexer;
            nextToken();
            nextToken();
        }
        std::string type_to_string_(Type type) {
    switch(type) {
        case LABEL:
            return "LABEL";
            break;
        case PRINT:
            return "PRINT";
            break;
        case SEMI_COLON:
            return "SEMI_COLON";
            break;
        case IF:
            return "IF";
            break;
        case WHILE:
            return "WHILE";
            break;
        case NUMBER:
            return "NUMBER";
            break;
        case STRING:
            return "STRING";
            break;
        default:
            return "Unknown";
            break;
    }
}
        int check_token(Type type) {
            return type==curToken.type;
        }
        
        int check_peek(Type type) {
            return type==peek_token.type;
        }
        void nextToken() {
            curToken=peek_token;
            peek_token=lexer.get_next_token();
        }
        void match(Type type) {
            if(!check_token(type)) {
                std::cout << "Error expected "<<type_to_string_(type) <<", got "<<type_to_string_(curToken.type)<< std::endl;
                exit(EXIT_FAILURE);
            } else {
                nextToken();
            }
        }
        void close() {
            std::cout << "Close" << std::endl;
            match(SEMI_COLON);
        }
        void expression() {
            std::cout << "expression" << std::endl;
        }
        void statement() {
            if(check_token(PRINT)) {
                std::cout << "Statement Print" << std::endl;
                nextToken();
                if(check_token(STRING)) {
                    nextToken();
                } else {
                    expression();
                }
            }
            
            close();
        }
        void program() {
            std::cout << "Program" << std::endl;
            while(!check_token(_EOF)) {
                statement();
            }
        }
};

int main(int argc,char* argv[]) {
    std::string input,sp;
    std::ifstream fp(argv[1]);
    if(argc<2) {
        std::cout << "Usage: [program] [source_file]" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(fp.is_open()) {
        while(getline(fp,sp)) {
            input.append(sp);
        }
        fp.close();
    } else {
        std::cout << "Error reading file" << std::endl;
    }
    input.append("\n");
    Lexer lex=Lexer(input);
    Parser parser=Parser(lex);
    //token tk=lex.get_next_token();
    /*while(tk.type!=_EOF) {
        if(tk.type==STRING||tk.type==NUMBER||tk.type==IDENT) {
            std::cout << tk.text << std::endl;
        } else {
            std::cout << tk.type << std::endl;
        }
        tk=lex.get_next_token();
    }*/
    parser.program();
    return 0;
}
