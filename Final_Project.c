#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#define Buffer_Size 10000
#define BUFFER 1000000
#define buffer 1000

//Assignment 1 functions begin
void removeEmptyLines(FILE *p1, FILE *p2);
int isEmpty(char *str);
void removeComments(FILE *p1, FILE *p2);
void singleLineComments(FILE *p1);
void multiLineComments(FILE *p1);
void removeNewLines(FILE *p1,FILE *p2);
//Assignment 1 functions end


//Assignment 2 functions begin
bool isDelimiter(char ch);
bool isOperator(char ch);
bool isSeparator(char ch);
bool isParanthesis(char ch);
bool isKeyword(char* str);
bool isInteger(char* str);
bool isFloat(char* str);
bool validIdentifier(char* str);
char* substring(char*str, int left, int right);
void iterate(char* str);
//Assignment 2 functions end

//Assignment 3 functions begin
char tokens[100][100];
char fileInput[10000];
struct Table
{
    int sl;
    char name[100];
    char datatype[100];
    char idtype[100];
    char scope[100];
    char value[100];
} symbolTable[100];

void display(FILE *f2);
bool searchId(char str[],int i);
char *findDatatype(int i);
int findScope(int i);
void implementing_Symbol_Table(FILE *f1,FILE *f2);
void removing_Lexemes_Except_Id(FILE *f1,FILE *f2);
void final_Output(FILE *f1,FILE *f2);
//Assignment 3 functions end

//Assignment 4 functions start
char addline[buffer][buffer];
char temp[buffer];
int arraylen;

struct Stack
{
    int top;
    int parentheses[buffer];
    int line[buffer];
} st;

void initialize();
bool isFull();
bool isEmptyy();
void push(int par,int line);
int pop1(int line);
int pop2();
void printLineNumbers(FILE *f1,FILE *f2);
void checkSemicolon();
void checkParantheses();
void checkIfElse();
void checkKeyword();
//Assignment 4 functions end

//Assignment 1 start
int isEmpty(char *str)
{
    char ch;

    do
    {
        ch = *(str++);
        //printf("%c ",ch);

        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;
    }
    while(ch != '\0');

    return 1;//returns 1 if string is empty
}

void removeEmptyLines(FILE *p1, FILE *p2)
{
    char str[Buffer_Size];

    while((fgets(str,Buffer_Size,p1))!= NULL)
    {
        if(!isEmpty(str))
        {
            fputs(str,p2);
        }
    }
}

void removeComments(FILE *p1,FILE *p2)
{
    char ch,d,e;

    while((ch=fgetc(p1))!=EOF)
    {
        if(ch == '/')
        {
            if((d=fgetc(p1)) == '/')
            {
                singleLineComments(p1);
            }
            else if(d == '*')
            {
                multiLineComments(p1);
            }
            else
            {
                fputc(ch,p2);
                fputc(d,p2);
            }
        }
        else if(ch == '"')
        {
            fputc(ch,p2);
            while((e=fgetc(p1))!='"')
            {
                fputc(e,p2);
            }
            fputc(e,p2);
        }
        else
        {
            fputc(ch,p2);
        }
    }
}

void singleLineComments(FILE *p1)
{
    char s;

    while((s=fgetc(p1))!=EOF)
    {
        if(s == '\n')
        {
            return;
        }
    }
}

void multiLineComments(FILE *p1)
{
    char m,e;

    while((m=fgetc(p1))!=EOF)
    {
        if(m == '*')
        {
            e=fgetc(p1);
            if(e == '/')
            {
                return;
            }
        }
    }
}

void removeNewLines(FILE *p1,FILE *p2)
{
    char ch;

    while((ch=fgetc(p1))!=EOF)
    {
        if(ch!='\n')
        {
            fputc(ch,p2);
        }
    }
}
//Assignment 1 end


//Assignment 2 start
bool isDelimiter(char ch)
{
    if(ch == ' ' || ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == ',' || ch == ';' || ch == '>' ||
            ch == '<' || ch == '=' || ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' || ch == '[' || ch == ']')
    {
        return true;
    }
    return false;
}

bool isOperator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '/' || ch == '*' ||
            ch == '<' || ch == '>' || ch == '=')
    {
        return true;
    }
    return false;
}

bool isSeparator(char ch)
{
    if(ch == ';' || ch == ',' || ch == ':' || ch == '\'' || ch == '\"')
    {
        return true;
    }
    return false;
}

bool isParanthesis(char ch)
{
    if(ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']')
    {
        return true;
    }
    return false;
}

bool isKeyword(char* str)
{
    if(!strcmp(str,"if") || !strcmp(str,"else") || !strcmp(str,"while") ||
       !strcmp(str,"do") || !strcmp(str,"break") || !strcmp(str,"continue") ||
       !strcmp(str,"int") || !strcmp(str,"double") || !strcmp(str,"float") ||
       !strcmp(str,"return") || !strcmp(str,"char") || !strcmp(str,"case") ||
       !strcmp(str,"sizeof") || !strcmp(str,"long") || !strcmp(str,"short") ||
       !strcmp(str,"typedef") || !strcmp(str,"switch") || !strcmp(str,"unsigned") ||
       !strcmp(str,"void") || !strcmp(str,"static") || !strcmp(str,"struct") || !strcmp(str,"goto"))
    {
        return true;
    }
    return false;
}

bool isInteger(char* str)
{
    int i,len=strlen(str);

    if(len==0)
    {
        return false;
    }
    for(i=0; i<len; i++)
    {
        if(str[i]!='0' && str[i]!='1' && str[i]!='2'
                && str[i]!='3' && str[i]!='4' && str[i]!='5'
                && str[i]!='6' && str[i]!='7' && str[i]!='8'
                && str[i]!='9' || (str[i]=='-' && i>0))
        {
            return false;
        }
    }
    return true;

}

bool isFloat(char* str)
{
    int i,len=strlen(str);
    bool point=false;

    if(len==0)
    {
        return false;
    }
    for(i=0; i<len; i++)
    {
        if(str[i]!='0' && str[i]!='1' && str[i]!='2'
                && str[i]!='3' && str[i]!='4' && str[i]!='5'
                && str[i]!='6' && str[i]!='7' && str[i]!='8'
                && str[i]!='9' && str[i]!='.' || (str[i]=='-' && i>0))
        {
            return false;
        }
        if(str[i]=='.')
        {
            point=true;
        }
    }
    return point;
}

bool validIdentifier(char* str)
{
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
            str[0] == '3' || str[0] == '4' || str[0] == '5' ||
            str[0] == '6' || str[0] == '7' || str[0] == '8' ||
            str[0] == '9' || isDelimiter(str[0]) == true)
    {
        return (false);
    }
    return (true);
}

char* substring(char*str, int left, int right)
{
    int i;
    char* substr = (char*)malloc(sizeof(char) *(right-left+2));

    for(i=left; i<=right; i++)
    {
        substr[i-left] = str[i];
    }
    substr[right-left+1]='\0';

    return substr;
}

void iterate(char* str)
{
    int left = 0,right = 0;
    int len = strlen(str);
    char opening_bracket = '[';
    FILE *file;
    file = fopen("Output_Assignment2_2.txt","w");
    char ch;

    while(right<=len && left<=right)
    {
        if(isDelimiter(str[right])==false)
        {
            right++;
        }

        if(isDelimiter(str[right])==true && left==right)
        {
            if(isOperator(str[right])==true)
            {
                printf("[%c operator]  ",str[right]);
                fputc(opening_bracket,file);
                fputc(str[right],file);
                fputs(" operator]  ",file);
            }
            else if(isSeparator(str[right])==true)
            {
                printf("[%c separator]  ",str[right]);
                fputc(opening_bracket,file);
                fputc(str[right],file);
                fputs(" separator]  ",file);
            }
            else if(isParanthesis(str[right])==true)
            {
                printf("[%c paranthesis]  ",str[right]);
                fputc(opening_bracket,file);
                fputc(str[right],file);
                fputs(" paranthesis]  ",file);
            }

            right++;
            left=right;
        }
        else if(isDelimiter(str[right])==true && left!=right || (right==len && left!=right))
        {
            char* sub = substring(str, left, right-1);

            if(isKeyword(sub)==true)
            {
                printf("[%s keyword]  ",sub);
                fputc(opening_bracket,file);
                fputs(sub,file);
                fputs(" keyword]  ",file);
            }
            else if(isInteger(sub)==true)
            {
                printf("[%s number]  ",sub);
                fputc(opening_bracket,file);
                fputs(sub,file);
                fputs(" number]  ",file);
            }
            else if(isFloat(sub)==true)
            {
                printf("[%s number]  ",sub);
                fputc(opening_bracket,file);
                fputs(sub,file);
                fputs(" number]  ",file);
            }
            else if(validIdentifier(sub)==true && isDelimiter(str[right-1])==false)
            {
                printf("[%s identifier]  ",sub);
                fputc(opening_bracket,file);
                fputs(sub,file);
                fputs(" identifier]  ",file);
            }
            else if(validIdentifier(sub)==false && isDelimiter(str[right-1])==false)
            {
                printf("[%s unknown]  ",sub);
                fputc(opening_bracket,file);
                fputs(sub,file);
                fputs(" unknown]  ",file);
            }
            left=right;
        }
    }
    fclose(file);
    return;
}
//Assignment 2 end


//Assignment 3 begin
void display(FILE *f2)
{
    char ch;
    while((ch=fgetc(f2))!=EOF)
    {
        printf("%c",ch);
    }
    printf("\n\n");
}

bool searchId(char str[],int i)
{
    for(int cnt=0; cnt<i; cnt++)
    {
        if(!strcmp(str,tokens[cnt]))
        {
            return false;
        }
    }
    return true;
}

char *findDatatype(int i)
{
    char *dt;
    while(i>=0)
    {
        if((!strcmp(tokens[i],"float")))// || (!strcmp(tokens[i],"double")) || (!strcmp(tokens[i],"float")) (!strcmp(tokens[i],"long")) || (!strcmp(tokens[i],"char")))
        {
            strcpy(dt,tokens[i]);
            return dt;
        }
        i--;
    }
}

int findScope(int i)
{
    for(int j=i;j>=0;j--)
    {
        if(!strcmp(tokens[j],"("))
            return j-1;
        else if(!strcmp(tokens[j],")"))
            return 0;//global
    }
    return 0;//means global scope
}

void implementing_Symbol_Table(FILE *f1,FILE *f2)
{
    char ch;
    char str[100];
    int i=0;
    while((ch=fgetc(f1))!=EOF)
    {
        if(ch!='[')
        {
            if(ch==']')
            {
                fileInput[i]=' ';
                i++;
            }
            else
            {
                fileInput[i]=ch;
                i++;
            }
        }
    }
    /*for(int i=0; i<strlen(fileInput); i++)
    {
        printf("%c",fileInput[i]);
    }*/
    int j=0,ctr=0;
    for(int i=0; i<strlen(fileInput); i++)
    {
        if(fileInput[i]==' ' || fileInput[i]=='\0')
        {
            tokens[ctr][j]='\0';
            ctr++;
            j=0;
        }
        else
        {
            tokens[ctr][j]=fileInput[i];
            j++;
        }
    }
    /*for(int i=0; i<ctr; i++)
    {
        printf("%d. %s\n",i,tokens[i]);
    }*/
    int serial=0;
    //Inserting serial number and name in table
    for(int i=0; i<ctr; i++)
    {
        if(!strcmp(tokens[i],"id"))
        {
            symbolTable[serial].sl=serial+1;
            strcpy(str,tokens[i+1]);
            if(searchId(str,i+1))
            {
                strcpy(symbolTable[serial].name,tokens[i+1]);
                serial++;
            }
        }
    }
    //Inserting datatype in table
    char datatype[100];
    serial=0;
    for(int i=0; i<ctr; i++)
    {
        if(!strcmp(tokens[i],"id"))
        {
            if(!strcmp(tokens[i+1],symbolTable[serial].name))
            {
                strcpy(symbolTable[serial].datatype,tokens[i-1]);
                serial++;
            }
        }
    }
    //Inserting Idtype
    serial = 0;
    for(int i=0; i<ctr; i++)
    {
        if(!strcmp(tokens[i],symbolTable[serial].name))
        {
            if(!strcmp(tokens[i+1],"("))
            {
                strcpy(symbolTable[serial].idtype,"func");
                serial++;
            }
            else
            {
                strcpy(symbolTable[serial].idtype,"var");
                serial++;
            }
        }
    }
    j=serial;
    //Inserting value
    serial = 0;
    for(int i=0; i<ctr; i++)
    {
        if(!strcmp(tokens[i],symbolTable[serial].name) && !strcmp(symbolTable[serial].idtype,"var"))
        {
            if(!strcmp(tokens[i+1],"="))
            {
                strcpy(symbolTable[serial].value,tokens[i+2]);
                serial++;
            }
            else
            {
                strcpy(symbolTable[serial].value,"-");
                serial++;
            }
        }
    }
    //Find scope
    int scope;
    serial=0;
    for(int i=0;i<ctr;i++)
    {
        if(!strcmp(tokens[i],"id"))
        {
            scope = findScope(i);
            if(scope==0)
            {
                strcpy(symbolTable[serial].scope,"global");
                serial++;
            }
            else
            {
                strcpy(symbolTable[serial].scope,tokens[scope]);
                serial++;
            }
        }
    }
    printf("---Symbol Table---\n");
    printf("Sl\tName\tIdtype\tDatatype\tScope\tValue\n");
    for(int i=0; i<j; i++)
    {
        printf("%d\t%s\t%s\t%s\t\t%s\t%s\n",symbolTable[i].sl,symbolTable[i].name,symbolTable[i].idtype,symbolTable[i].datatype,symbolTable[i].scope,symbolTable[i].value);
    }
}

void removing_Lexemes_Except_Id(FILE *f1,FILE *f2)
{
    char ch,c,space=' ';
    char string[100],separate[100][100];
    int ctr,j;

    while((ch=fgetc(f1))!=EOF)
    {
        if(ch=='[')
        {
            fputc(ch,f2);
            int i=0;
            memset(string,'\0',sizeof(string));
            do
            {
                ch=fgetc(f1);
                if(ch!=']')
                {
                    string[i]=ch;
                    i++;
                }
            }
            while(ch!=']');
            c=ch;
            string[i]='\0';
            j=0,ctr=0;
            for(int i=0; i<=(strlen(string)); i++)
            {
                if(string[i]==' ' || string[i]=='\0')
                {
                    separate[ctr][j]='\0';
                    ctr++;
                    j=0;
                }
                else
                {
                    separate[ctr][j]=string[i];
                    j++;
                }
            }
            if(!strcmp(separate[0],"id"))
            {
                fputs(separate[0],f2);
                fputc(space,f2);
                fputs(separate[1],f2);
            }
            else
            {
                fputs(separate[1],f2);
            }
            fputc(c,f2);
        }
    }
}

void final_Output(FILE *f1,FILE *f2)
{
    int serial=0;
    for(int i=0;i<strlen(fileInput);i++)
    {
        if(fileInput[i]=='i' && fileInput[i+1]=='d')
        {
            fileInput[i+3]=(char)symbolTable[serial].sl;
            serial++;
        }
    }
    for(int i=0;i<strlen(fileInput);i++)
    {
        fputc(fileInput[i],f2);
    }
}
//Assignment 3 end

//Assignment 4 start
//Initialize top index to -1
void initialize()
{
    st.top=-1;
}

//Check to see if stack is full
bool isFull()
{
    if(st.top >= buffer-1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Check to see if stack is empty
bool isEmptyy()
{
    if(st.top == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Add element to stack
void push(int par,int line)
{
    if(isFull())
    {
        printf("Stack error\n");
    }
    else
    {
        st.parentheses[st.top+1] = par;
        st.line[st.top+1] = line;
        st.top++;
    }
}

//Pop parentheses from the stack
int pop1(int line)
{
    if(isEmptyy())
    {
        printf("Mismatched parentheses at line %d,\n",line);
    }
    else
    {
        st.top=st.top-1;
        return st.parentheses[st.top+1];
    }
}

//Pop line number from the stack
int pop2()
{
    return st.line[st.top+1];
}

void printLineNumbers(FILE *f1,FILE *f2)
{
    int line=1;
    char ch;

    int i=0;
    while((ch=fgetc(f1))!=EOF)
    {
        temp[i]=ch;
        i++;
    }
    /*for(int i=0; i<strlen(temp); i++)
    {
        printf("%c",temp[i]);
    }*/
    int j=0,ctr=1;
    for(int i=0; i<strlen(temp); i++)
    {
        if(temp[i]=='\n')
        {
            addline[ctr][j]='\0';
            ctr++;
            j=0;
        }
        else
        {
            addline[ctr][j]=temp[i];
            j++;
        }
    }
    for(int i=1; i<ctr; i++)
    {
        printf("%d. %s\n",i,addline[i]);
        fprintf(f2,"%d %s\n",i,addline[i]);
    }
    arraylen=ctr;
}

void checkSemicolon()
{
    printf("\n\n");
    for(int i=1; i<arraylen; i++)
    {
        for(int j=0; j<strlen(addline[i]); j++)
        {
            if(addline[i][j]=='f' && addline[i][j+1]=='o' && addline[i][j+2]=='r' && addline[i][j+3]=='(' && addline[i][j+4]==';' && addline[i][j+5]==';' && addline[i][j+6]==')')
            {
                continue;
            }
            if(addline[i][j]==';' && addline[i][j+1]==';')
            {
                printf("Duplicate token at line %d,\n",i);
                break;
            }
        }
    }
}

void checkParantheses()
{
    initialize();
    for(int i=1; i<=arraylen; i++)
    {
        for(int j=0; j<strlen(addline[i]); j++)
        {
            if(addline[i][j]=='(' || addline[i][j]=='{' || addline[i][j]=='[')
            {
                push(addline[i][j],i);
            }
            else if(addline[i][j]==')' || addline[i][j]=='}' || addline[i][j]==']')
            {
                pop1(i);
                pop2();
            }
        }
    }

    if(!isEmptyy())
    {
        printf("Unmatched parentheses at line %d,\n",st.line[st.top+1]);
    }
}

void checkIfElse()
{
    char ifElse[buffer][buffer];
    int x=1,ifElseArr[buffer];
    for(int i=1; i<arraylen; i++)
    {
        for(int j=0; j<strlen(addline[i]); j++)
        {
            if(addline[i][j]=='i' && addline[i][j+1]=='f')
            {
                strcpy(ifElse[x],"if");
                ifElseArr[x]=i;
                x++;
            }
            if(addline[i][j]=='e' && addline[i][j+1]=='l' && addline[i][j+2]=='s' && addline[i][j]=='e')
            {
                strcpy(ifElse[x],"else");
                ifElseArr[x]=i;
                x++;
            }
            if(addline[i][j]=='e' && addline[i][j+1]=='l' && addline[i][j+2]=='s' && addline[i][j]=='e' && addline[i][j]==' ' && addline[i][j]=='i' && addline[i][j]=='f')
            {
                strcpy(ifElse[x],"else if");
                ifElseArr[x]=i;
                x++;
            }
        }
    }
    for(int i=1; i<=x; i++)
    {
        if(!strcmp(ifElse[i],"else") && !strcmp(ifElse[i+1],"else"))
        {
            printf("Unmatched 'else' at line %d,\n",ifElseArr[i+1]);
        }
        if(!strcmp(ifElse[i],"else") && !strcmp(ifElse[i+1],"else if"))
        {
            printf("Unmatched 'else if' at line %d,\n",ifElseArr[i+1]);
        }
    }
}

void checkKeyword()
{
    char separate[buffer][buffer];
    int i,j,ctr=0,x=0;
    for(i=1; i<=arraylen; i++)
    {
        for(j=0; j<strlen(addline[i]); j++)
        {
            if(addline[i][j]=='\0' || addline[i][j]==' ')
            {
                separate[ctr][x]='\0';
                ctr++;
                x=0;
            }
            else
            {
                separate[ctr][x]=addline[i][j];
                x++;
            }
        }
    }
}
//Assignment 4 end

int main()
{
    //Assignment 1 part begin
    FILE *p1,*p2;
    char c;
    p1 = fopen("input.txt","r");
    p2 = fopen("Output_Assignment1_1.txt","w");

    if(p1 == NULL || p2 == NULL)
    {
        printf("File Can't be Opened.");
        return 0;
    }

    removeComments(p1,p2);
    fclose(p1);
    fclose(p2);


    p1 = fopen("Output_Assignment1_1.txt","r");
    p2 = fopen("Output_Assignment1_2.txt","w");

    removeEmptyLines(p1,p2);
    fclose(p1);
    fclose(p2);

    p1 = fopen("Output_Assignment1_2.txt","r");
    p2 = fopen("Output_Assignment1_3.txt","w");

    removeNewLines(p1,p2);
    fclose(p1);
    fclose(p2);

    p2 = fopen("Output_Assignment1_3.txt","r");
    printf("Output 1: \n");
    while((c=fgetc(p2))!=EOF)
    {
        printf("%c",c);
    }
    fclose(p2);
    printf("\n\n");
    //Assignment 1 part end


    //Assignment 2 part begin
    char ch,input[BUFFER],text[BUFFER];
    int len,i,j;
    FILE *file;
    file = fopen("Output_Assignment1_3.txt","r");
    if(file == NULL)
    {
        printf("File Can't be Opened.");
        return 0;
    }
    for(i=0; (ch=fgetc(file))!=EOF; i++)
    {
        text[i]=ch;
    }
    fclose(file);
    len=strlen(text);
    for(i=0,j=0; i<len; i++,j++)
    {
        if(text[i]==';' || text[i]==',' || text[i]=='=' || text[i]==':' || text[i]=='(' || text[i]==')' || text[i]=='{'
                || text[i]=='}' || text[i]=='[' || text[i]==']' || text[i]=='\"' || text[i]=='\'')
        {
            input[j]=' ';
            j=j+1;
            input[j]=text[i];
            j=j+1;
            input[j]=' ';
        }
        else
        {
            input[j]=text[i];
        }
    }
    file = fopen("Output_Assignment2_1.txt","w");
    len=strlen(input);

    printf("Output 2: \n");
    /*for(i=0;i<len;i++)
    {
        printf("%c",input[i]);
    }*/

    fputs(input,file);
    fclose(file);
    iterate(input);
    printf("\n\n");
    //Assignment 2 part end

    //Assignment 3 part begin
    printf("Output 3: \n");
    FILE *f1,*f2;
    f1 = fopen("Input_Assignment3_1.txt","r");
    f2 = fopen("Output_Assignment3_1.txt","w");

    if(f1 == NULL || f2 == NULL)
    {
        printf("File Can't be Opened.");
        return 0;
    }

    removing_Lexemes_Except_Id(f1,f2);
    fclose(f1);
    fclose(f2);

    /*f2 = fopen("Output_Assignment3_1.txt","r");
    display(f2);
    fclose(f2);*/

    f1 = fopen("Output_Assignment3_1.txt","r");
    f2 = fopen("Output_Assignment3_2.txt","w");
    implementing_Symbol_Table(f1,f2);
    fclose(f1);
    fclose(f2);

    f1 = fopen("Output_Assignment3_1.txt","r");
    f2 = fopen("Output_Assignment3_2.txt","w");
    final_Output(f1,f2);
    fclose(f1);
    fclose(f2);
    printf("\n\n");
    //Assignment 3 part end

    //Assignment 4 part begin
    printf("Output 4: \n");
    f1 = fopen("input.txt","r");
    f2 = fopen("Output_Assignment4_1.txt","w");

    if(f1 == NULL || f2 == NULL)
    {
        printf("File can't be opened");
        return 0;
    }

    removeComments(f1,f2);

    fclose(f1);
    fclose(f2);

    f1 = fopen("Output_Assignment4_1.txt","r");
    f2 = fopen("Output_Assignment4_2.txt","w");

    printLineNumbers(f1,f2);

    fclose(f1);
    fclose(f2);

    checkSemicolon();
    checkParantheses();
    checkIfElse();
    checkKeyword();
    //Assignment 4 part end
    return 0;
}
