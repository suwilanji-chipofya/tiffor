#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

enum Type {
    NONE,
    _EOF,
    INTEGER,
    PLUS,
    MINUS,
    DIV,
    MUL,
    SEMI_COLON
};

struct Token {
    Type type;
    int value;
};
class Interpreter {
    public:
        string source;
        char cur_char;
        int pos=-1;
        struct Token cur_token;
        Interpreter(string source) {
            this->source=source;
            next();
            get_cur_token();
        }
        void next() {
            pos+=1;
            if(pos>=source.length()) {
                cur_char='\0';
            } else {
                cur_char=source[pos];
            }
        }
        char peek() {
            if(pos+1>=source.length()) {
                return ('\0');
            } else {
                return (source[pos+1]);
            }
        }
        char return_cur_char() {
            return cur_char;
        }
        string str_type(Type type) {
            switch(type) {
                case 0:
                    return ("NONE");
                    break;
                case 1:
                    return ("EOF");
                    break;
                case 2:
                    return ("INTEGER");
                    break;
                case 3:
                    return ("PLUS");
                    break;
                case 4:
                    return ("MINUS");
                    break;
                case 5:
                    return ("DIV");
                    break;
                case 6:
                    return ("MUL");
                    break;
                case 7:
                    return ("SEMI_COLON");
                default:
                    return ("UNKNOWN");
                    break;
            }
        }
        void get_cur_token() {
            int start,end,num;
            string str_num;
            switch(cur_char) {
                case '+':
                    cur_token.type=PLUS;
                    cur_token.value=0;
                    break;
                case '-':
                    cur_token.type=MINUS;
                    cur_token.value=0;
                    break;
                case '/':
                    cur_token.type=DIV;
                    cur_token.value=0;
                    break;
                case '*':
                    cur_token.type=MUL;
                    cur_token.value=0;
                    break;
                case '\0':
                    cur_token.type=_EOF;
                    cur_token.value=0;
                    break;
                case ';':
                    cur_token.type=SEMI_COLON;
                    cur_token.value=0;
                    break;
                default:
                    if(isdigit(cur_char)) {
                        start=pos;
                        while(isdigit(peek())) {
                            next();
                        }
                        end=pos;
                        str_num=source.substr(start,end+1);
                        stringstream stream(str_num);
                        stream>>num;
                        cur_token.type=INTEGER;
                        cur_token.value=num;
                    } else {
                        cur_token.type=NONE;
                        cur_token.value=-1;
                    }
                    break;
            }
            next();
        }
        //Debugging function
        void print_cur_token() {
            cout<<str_type(cur_token.type)<<endl;
            cout<<"Value: "<<cur_token.value<<endl;
        }
        void check_token(Type type) {
            if(cur_token.type==type) {
                get_cur_token();
            } else {
                cout<<"Expected token "<<str_type(type)<<" got "<<str_type(cur_token.type)<<endl;
                exit(EXIT_FAILURE);
            }
        }
        void expr() {
            struct Token left=cur_token;
            check_token(INTEGER);
            struct Token op=cur_token;
            struct Token right;
            switch(op.type) {
                case PLUS:
                    get_cur_token();
                    right=cur_token;
                    check_token(INTEGER);
                    cout<<(double)left.value+right.value;
                    break;
                case MINUS:
                    get_cur_token();
                    right=cur_token;
                    check_token(INTEGER);
                    cout<<(double)left.value-right.value;
                    break;
                case DIV:
                    get_cur_token();
                    right=cur_token;
                    check_token(INTEGER);
                    cout<<(double)left.value/right.value;
                    break;
                case MUL:
                    get_cur_token();
                    right=cur_token;
                    check_token(INTEGER);
                    cout<<(double)left.value*right.value;
                    break;
                default:
                    break;
            }
            cout<<"\n";
            struct Token end=cur_token;
            check_token(SEMI_COLON);
            
        }
};

int main(int argc,char* argv[]) {
    if(argc<2) {
        cout<<"Usage: tiffor [source_file]\n";
        exit(EXIT_FAILURE);
    }
    string source,sp;
    ifstream fp(argv[1]);
    if(fp.is_open()) {
        while(getline(fp,sp)) {
            source.append(sp);
        }
        fp.close();
    } else {
        cout<<"Error opening file"<<endl;
        exit(EXIT_FAILURE);
    }
    source.append("\n");
    Interpreter in=Interpreter(source);
    while(in.peek()!='\0') {
        in.expr();
    }
    return 0;
}