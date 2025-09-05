#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define GROUP_LABEL_SUBTREE 1

const char* p;
int errorFlag = 0;   
int notBinary = 0;  

static inline void skip(void) { 
    while (isspace(*p))
        p++;
}

static int readLabel(void) {
    skip();
    if (!isalpha((unsigned char)*p)) 
        return 0;
    while (isalpha((unsigned char)*p)) 
        p++;
    return 1;
}

static int lookStd(const char* qq) {
    const char* q = qq;
    skip();
    if (*q != '(') 
        return 0;
    q++;
    skip();
    if (!isalpha((unsigned char)*q)) 
        return 0;
    while (isalpha((unsigned char)*q)) 
        q++;
    return 1;
}

static int lookPair(const char* qq) {
    const char* q = qq;
    skip();
    if (*q != '(') 
        return 0;
    q++; 
    skip();
    if (!isalpha((unsigned char)*q)) 
        return 0;
    while (isalpha((unsigned char)*q)) 
        q++;
    skip();
    if (!isalpha((unsigned char)*q)) 
        return 0;
    while (isalpha((unsigned char)*q)) 
        q++;
    skip();
    return (*q == ')');
}

static int atChildStart(void) {
    skip();
    return (*p != '\0' && *p != ')' && (*p == '(' || isalpha((unsigned char)*p)));
}

static void parseNode(void); 

static void parsePairGroup(void) {
    skip();
    if (*p != '(') { errorFlag = 1; return; }
    p++; skip();
    if (!readLabel()) { errorFlag = 1; return; }
    skip();
    if (!readLabel()) { errorFlag = 1; return; }
    skip();
    if (*p != ')') { errorFlag = 1; return; }
    p++; 
}

static void parsePart(void) {
    skip();
    if (*p == '(') {
        if (lookPair(p)) {
            parsePairGroup();
        }
        else if (lookStd(p)) {
            parseNode();
        }
        else {
            errorFlag = 1;
        }
    }
    else if (isalpha((unsigned char)*p)) {
        readLabel();
#if GROUP_LABEL_SUBTREE
        skip();
        if (lookStd(p)) parseNode();  
#endif
    }
    else {
        errorFlag = 1;
    }
}

static void parseNode(void) {
    skip();
    if (*p != '(') {
        errorFlag = 1; 
        return; 
    }
    p++; 
    skip();

    if (!readLabel()) { 
        errorFlag = 1; 
        return; 
    }

    int parts = 0;
    while (!errorFlag && atChildStart()) {
        parsePart();
        parts++;
        if (parts > 2) 
            notBinary = 1;  
    }

    skip();
    if (*p != ')') { 
        errorFlag = 1; 
        return; 
    }
    p++;
}

int main(void) {
    static char buf[10000];
    if (!fgets(buf, sizeof(buf), stdin)) { 
        puts("ERROR"); 
        return 0; 
    }
    buf[strcspn(buf, "\n")] = '\0';

    p = buf;
    skip();
    parseNode();
    skip();
    if (*p != '\0') 
        errorFlag = 1;

    if (errorFlag) 
        printf("ERROR");
    else if (notBinary) 
        printf("FALSE");
    else 
        printf("TRUE");
    return 0;
}
