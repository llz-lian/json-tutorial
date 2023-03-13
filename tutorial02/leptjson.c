#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL, strtod() */
#include <math.h> /*inf*/
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
typedef struct {
    const char* json;
}lept_context;

static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}
/*
static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}
*/



int __checkValidNum(const char * start)
{
    int err_no = LEPT_PARSE_INVALID_VALUE;
    if(*start == '-')
        start++;
    if(*start == '0')
    {
        start++;
        if(*start>='0'&&*start<='9') return LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    else if(*start>='1'&&*start<='9')
       { while (*start>='0'&&*start<='9') start++;err_no = LEPT_PARSE_ROOT_NOT_SINGULAR;}
    else
        return err_no;
    if(*start == '\0') return LEPT_PARSE_OK;
    if(!(*start=='.'||*start=='e'||*start=='E')) return LEPT_PARSE_ROOT_NOT_SINGULAR;
    /*. e  E*/
    int has_frac = (*start == '.')?1:0;
    if(has_frac)
    {
        /*frac*/
        start++;
        if(*start == '\0') return LEPT_PARSE_INVALID_VALUE;
        while (*start>='0'&&*start<='9') start++;
        if(*start == '\0') return LEPT_PARSE_OK;
    }
    /*exp*/
    if(!(*start == 'e'||*start == 'E')) return err_no;
    start++;
    if(*start == '\0') return err_no;
    if(*start == '+'||*start =='-') start++;
    if(*start == '\0') return err_no;
    while (*start>='0'&&*start<='9') start++;

    return *start == '\0'?LEPT_PARSE_OK:err_no;
}


static int lept_parse_number(lept_context* c, lept_value* v) {
    char* end;
    /* \TODO validate number */
    const char * start = c->json;
    int check = __checkValidNum(start);
    if(check != LEPT_PARSE_OK)
    {
        v->type = LEPT_NULL;
        return check;
    }
    /*check "-"*/
    v->n = strtod(c->json, &end);
    if(isinf(v->n))
        return LEPT_PARSE_NUMBER_TOO_BIG;
    if (c->json == end)
        return LEPT_PARSE_INVALID_VALUE;
    c->json = end;
    v->type = LEPT_NUMBER;
    return LEPT_PARSE_OK;
}

static int lept_parse_literal(lept_context * c, lept_value * v,const char * cmp,size_t cmp_length,int target_value_type)
{
    int i = 0;
    for(;i<cmp_length;i++)
    {
        if(c->json[i]!=cmp[i])
        {
            return LEPT_PARSE_INVALID_VALUE;
        }
    }
    v->type = target_value_type;
    c->json += cmp_length;
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return lept_parse_literal(c, v,"true",4,LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v,"false",5,LEPT_FALSE);
        case 'n':  return lept_parse_literal(c, v,"null",4,LEPT_NULL);
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
