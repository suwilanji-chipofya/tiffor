#include <iostream>
#include <fstream>
#include <ctype.h>
#include <cstdint>
//Tiffor compiler
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
        std::string symbols[UINT16_MAX];
        std::string labelsDeclared[UINT16_MAX];
        std::string labelsVisited[UINT16_MAX];
        //points to top of labels array
        int lp=-1;
        int lv=-1;
        int sp=-1;
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
        
        int isComparisonOperator() {
            return (check_token(GT)||check_token(GTEQ)||check_token(LT)||check_token(LTEQ)||check_token(NOTEQ)||check_token(EQEQ));
        }
        
        void primary() {
            std::cout << "Primary( "<<curToken.text<<" )" << std::endl;
            if(check_token(NUMBER)) {
                nextToken();
            } else if(check_token(IDENT)) {
                for(int i=0;i<=UINT16_MAX;i++) {
                    if(curToken.text==symbols[i]) {
                        break;
                    } else if(i==UINT16_MAX&&curToken.text!=symbols[i]) {
                        std::cout << "Referencing undefined variable "<<curToken.text << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                nextToken();
            } else {
                std::cout << "Unexpected token at "<<curToken.text << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        void unary() {
            std::cout << "unary" << std::endl;
            if(check_token(PLUS)||check_token(MINUS)) {
                nextToken();
                
            }
            primary();
        }
        void term() {
            std::cout << "term" << std::endl;
            unary();
            while(check_token(ASTERISK)||check_token(SLASH)) {
                nextToken();
                unary();
            }
        }
        void comparison() {
            std::cout << "comparison" << std::endl;
            expression();
            if(isComparisonOperator()) {
                nextToken();
                expression();
            } else {
                std::cout << "Expected comparison operation at "<<curToken.text << std::endl;
                exit(EXIT_FAILURE);
            }
            while(isComparisonOperator()) {
                nextToken();
                expression();
            }
        }
        void expression() {
            std::cout << "expression" << std::endl;
            if(check_token(STRING)) {
                std::cout << curToken.text << std::endl;
                nextToken();
            } else {
                term();
            while(check_token(PLUS)||check_token(MINUS)){
                nextToken();
                term();
            }
            }
            
            
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
            } else if(check_token(IF)) {
                std::cout << "Statement If" << std::endl;
                nextToken();
                comparison();
                match(THEN);
                close();
                while(!check_token(ENDIF)) {
                    statement();
                }
                match(ENDIF);
            } else if(check_token(WHILE)) {
                std::cout << "statement while" << std::endl;
                nextToken();
                comparison();
                match(REPEAT);
                close();
                while(!check_token(ENDWHILE)) {
                    statement();
                }
                match(ENDWHILE);
            } else if(check_token(LABEL)) {
                std::cout << "statement label" << std::endl;
                nextToken();
                for(int i=0;i<=UINT16_MAX;i++) {
                    if(curToken.text==labelsDeclared[i]) {
                        std::cout << "Error label already exists" <<curToken.text<< std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                lp+=1;
                labelsDeclared[lp]=curToken.text;
                std::cout <<labelsDeclared[lp]<< std::endl;
                match(IDENT);
            } else if(check_token(GOTO)) {
                std::cout << "statement goto" << std::endl;
                nextToken();
                for(int i=0;i<=UINT16_MAX;i++) {
                    if(curToken.text==labelsDeclared[i]) {
                        break;
                    } else if(i==UINT16_MAX&&curToken.text!=labelsDeclared[i]) {
                        std::cout << "Attempting to goto undeclared label "<<curToken.text << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }
                lv+=1;
                labelsVisited[lv]=curToken.text;
                match(IDENT);
            } else if(check_token(LET)) {
                std::cout << "statement let" << std::endl;
                nextToken();
                for(int i=0;i<=UINT16_MAX;i++) {
                    if(curToken.text==symbols[i]) {
                        break;
                    } else if(i==UINT16_MAX&&curToken.text!=symbols[i]) {
                        sp+=1;
                        symbols[sp]=curToken.text;
                    }
                }
                match(IDENT);
                match(EQ);
                expression();
            } else if(check_token(INPUT)) {
                std::cout << "statement input" << std::endl;
                nextToken();
                for(int i=0;i<=UINT16_MAX;i++) {
                    if(curToken.text==symbols[i]) {
                        break;
                    } else if(i==UINT16_MAX&&curToken.text!=symbols[i]) {
                        sp+=1;
                        symbols[sp]=curToken.text;
                    }
                }
                match(IDENT);
            } else {
                std::cout << "Invalid statement at "<<curToken.text <<" "<<type_to_string_(curToken.type)<< std::endl;
                exit(EXIT_FAILURE);
            }
            
            close();
        }
        void program() {
            std::cout << "Program" << std::endl;
            lexer.skip_comment();
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
    parser.program();
    return 0;
}
