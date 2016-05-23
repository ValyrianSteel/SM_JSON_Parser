//
// Created by ubun on 16-5-13.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"


#define nil ((void*)0)
#define must(b) do { if (!(b)) return 0; } while (0)

enum KEYS {
    STATES = 1, STATE_NAME, STATE_DATA, STATE_PARE,
    STATE_ENTR, STATE_NEXT, STATE_TRAN,
    TRANS, TRANS_TYPE, TRANS_COND, TRANS_GURD,
    TRANS_NUM, TRANS_ACT, ENTR_ACT, EXIT_ACT,
    EVENTS, EVENT_TYPE, EVENT_DATA, EVENT_STATE,
};
enum VALUES {
    SM_STATE = 1, SM_TRAN, SM_EVENT, SM_ACT,
    SM_OBJ = '{', SM_ARRY = '[', SM_NUMB = '0', SM_STR = '"', SM_TRUE = 't', SM_FALSE = 'f', SM_NULL = 'n',
};


// 構造 狀態結構體
// 怎麼建立一張表，存儲狀態名和狀態序號的關係
STATE *state_all[100];
int i = 0;

void state_init()
{

}
void state_create_mem(STATE s)
{
    state_init();
//    state_all[i]->name =
}

static char* strdup_mem(const char* str)
{
    size_t len;
    char* copy;

    len = strlen(str) + 1;
    if (!(copy = (char*)malloc(len))) return 0;
    memcpy(copy,str,len);
    return copy;
}







// 解析

typedef struct Parser Parser;

struct Parser {
    char *src_p;
    int  n_item;
    JSON *json_p;
    int  n_json_deep;
};

static int consume(Parser *parser, char *src_p);

static void skip_ws(Parser *parser);

// static void scan_digits(Parser *parser);

static int scan_hex4(Parser *parser);
static unsigned parse_hex4(const char *str);

static JSON *init_item(Parser *parser, JSON *parent, JSON **prev, char item_type);

static int parse_word(Parser *parser, JSON *parent, JSON **prev, char *lit);


//static const char
static int parse_string(Parser *parser, JSON *parent, JSON **prev);

static int parse_number(Parser *parser, JSON *parent, JSON **prev);

static int parse_object(Parser *parser, JSON *parent, JSON **prev);

static int parse_array(Parser *parser, JSON *parent, JSON **prev);

static int parse_key(Parser *parser, JSON *parent, JSON **kprev);

static int parse_value(Parser *parser, JSON *parent, JSON **vprev);

static int parse_pair(Parser *parser, JSON *parent, JSON **kprev, JSON **vprev);

static int parse_item(Parser *parser, JSON *parent, JSON **prev);

static int parse_text(Parser *parser);

//int json_parse(char *src, JSON *part, int npart);
int json_parse(char *src, int n_item);


static int
consume(Parser *parser, char *src_p)
{
    while (*src_p) {
        must(*parser->src_p == *src_p);
        parser->src_p++;
        src_p++;
    }
    return 1;
}


static void
skip_ws(Parser *parser)
{
    while (*parser->src_p == ' ' || *parser->src_p == '\t' || *parser->src_p == '\n' || *parser->src_p == '\r') {
        parser->src_p++;
    }
}


static JSON *
init_item(Parser *parser, JSON *parent, JSON **prev, char item_type)
{
    parser->n_item++;

    if (parser->n_json_deep > 0) {
//        JSON *parser_json_item_p =
        JSON *json_item = (JSON*)malloc(sizeof(JSON));
        parser->json_p = json_item;
//        parser->json_p++;
        parser->n_json_deep--;

        json_item->item_type = item_type;
        json_item->src = parser->src_p;
        json_item->parent = parent;
        json_item->next = nil;
        json_item->prev = *prev;

        if (*prev) {
            (*prev)->next = json_item;
        }
        *prev = json_item;

//        int i = 0; int j = 0;
//        i= parser->json_p->item_type;
//        j= json_item->item_type;
//        printf("%p, %p", json_item, (parser->json_p));
//        printf("init type: %d %d\n", i, j);

        return json_item;
    }


    return nil;
}


static int
parse_word(Parser *parser, JSON *parent, JSON **prev, char *lit)
{
    JSON *json_item = init_item(parser, parent, prev, lit[0]);  // item_type init lit[0]
    must(consume(parser, lit));

    if (json_item) {
        json_item->item_end = parser->src_p;
    }
    return 1;
}



//static int
//scan_hex4(Parser *parser)
//{
//    for (int i = 0; i < 4; i++) {
//        char c = *parser->src_p++;
//        must(('0'<=c && c<='9') || ('a'<=c && c<='f') || ('A'<=c && c<='F'));
//    }
//    return 1;
//}
static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static unsigned 
parse_hex4(const char *str)
{
    unsigned h=0;
    if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4;str++;
    if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4;str++;
    if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    h=h<<4;str++;
    if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
    return h;
}


//static const char *
static int
parse_string(Parser *parser, JSON *parent, JSON **prev)
{
    if (*parser->src_p != '\"') return 0;

    char *ptr1 = parser->src_p + 1;     char *ptr2;
    char *string_out;    int string_len = 0;
    unsigned uc1, uc2;

    // string_len
    while (*ptr1 != '\"' && *ptr1 && ++string_len) {
        if (*ptr1++ == '\\') ptr1++;
    }

    string_out = (char*)malloc(string_len + 1);
    if (!string_len) return 0;

    ptr2 = string_out;

    // 初始化一個item，爲string
    JSON *json_item = init_item(parser, parent, prev, '"');
    parser->src_p++; /* consume " */
    while (*parser->src_p != '"' && *parser->src_p) {
        // char c = *parser->src_p;
        // must(c >= ' '); /* no control chars */
        // parser->src_p++;
        // if (c == '\\') {
        //     switch (c = *parser->src_p++) {
        //         case 'b': case 'f': case 'n': case 'r':
        //         case 't': case '"': case '\\': case '/':
        //             continue;
        //         case 'u':
        //             must(scan_hex4(parser));
        //             continue;
        //     }
        //     return 0;
        // }

        must( *parser->src_p >= ' ');
        if (*parser->src_p!='\\') *ptr2++=*parser->src_p++;
        else
        {
            parser->src_p++;
            switch (*parser->src_p)
            {
                case 'b': *ptr2++='\b'; break;
                case 'f': *ptr2++='\f'; break;
                case 'n': *ptr2++='\n'; break;
                case 'r': *ptr2++='\r'; break;
                case 't': *ptr2++='\t'; break;
                case 'u':    /* transcode utf16 to utf8. */
                    uc1=parse_hex4(parser->src_p+1);parser->src_p+=4;    /* get the unicode char. */

                    if ((uc1>=0xDC00 && uc1<=0xDFFF) || uc1==0)    break;  /* check for invalid.   */

                    if (uc1>=0xD800 && uc1<=0xDBFF)   /* UTF16 surrogate pairs.   */
                    {
                        if (parser->src_p[1]!='\\' || parser->src_p[2]!='u')    break;  /* missing second-half of surrogate.    */
                        uc2=parse_hex4(parser->src_p+3);parser->src_p+=6;
                        if (uc2<0xDC00 || uc2>0xDFFF)       break;  /* invalid second-half of surrogate.    */
                        uc1=0x10000 + (((uc1&0x3FF)<<10) | (uc2&0x3FF));
                    }

                    string_len=4;if (uc1<0x80) string_len=1;else if (uc1<0x800) string_len=2;else if (uc1<0x10000) string_len=3; ptr2+=string_len;

                    switch (string_len) {
                        case 4: *--ptr2 =((uc1 | 0x80) & 0xBF); uc1 >>= 6;
                        case 3: *--ptr2 =((uc1 | 0x80) & 0xBF); uc1 >>= 6;
                        case 2: *--ptr2 =((uc1 | 0x80) & 0xBF); uc1 >>= 6;
                        case 1: *--ptr2 =(uc1 | firstByteMark[string_len]);
                    }
                    ptr2+=string_len;
                    break;
                default:  *ptr2++=*parser->src_p; break;
            }
            parser->src_p++;
        }
    }
    if (*parser->src_p=='\"') parser->src_p++; /* consume " */

    if (json_item) {
        json_item->item_end = parser->src_p;
    }

//    parser->json_p->item_string = strdup_mem(string_out);
    json_item->item_string = strdup_mem(string_out);
//   return string_out;
//    return json_item;
    return 1;
}


// static void
// scan_digits(Parser *parser)
// {
//     while ('0' <= *parser->src_p && *parser->src_p <= '9') {
//         parser->src_p++;
//     }
// }

static int
parse_number(Parser *parser, JSON *parent, JSON **prev)
{
    double number = 0, sign = 1;

    JSON *json_item = init_item(parser, parent, prev, '0');

    if (*parser->src_p == '-') {
        sign = -1;
        parser->src_p++;
    }
    if (*parser->src_p == '0') { /* special case, single 0 */
        parser->src_p++;
    } 
    if ('1' <= *parser->src_p && *parser->src_p <= '9')
    { /* parser->src_p always starts with a digit */
        // scan_digits(parser);
        {
            do {
                number=(number*10.0)+(*parser->src_p++ -'0');   
            } while ('0' <= *parser->src_p && *parser->src_p <= '9');
            // while ('0' <= *parser->src_p && *parser->src_p <= '9') {
            //     parser->src_p++;
            // }    
        }
    }
    if (*parser->src_p == '.') {
        parser->src_p++;
        must('0' <= *parser->src_p && *parser->src_p <= '9');
        // scan_digits(parser);
        double fract = 0;
        {
            // while ('0' <= *parser->src_p && *parser->src_p <= '9') {
            //     parser->src_p++;
            // }
            double scale = 1; 
            do {
                scale *= 10.0;
                fract = fract + (*parser->src_p++ - '0') / scale;
            } while ('0' <= *parser->src_p && *parser->src_p <= '9');
        }
        number += fract;
    }
    if (*parser->src_p == 'e' || *parser->src_p == 'E') {
        parser->src_p++;

        double expo = 1; 
        double exposigh = 1;
        double expovalue = 0;
        if (*parser->src_p == '+') {
            parser->src_p++;
        } else if(*parser->src_p == '-') {
            exposigh = -1;
            parser->src_p++;
        }
        must('0' <= *parser->src_p && *parser->src_p <= '9');
        do {
            expovalue=(expovalue*10.0)+(*parser->src_p++ -'0');   
        } while ('0' <= *parser->src_p && *parser->src_p <= '9');
        while (expovalue > 0) {
            expo *= 10;
            expovalue--;
        }
            
        if (exposigh == 1) number *= expo;
        else if (exposigh == -1) number /= expo;
        
        // // scan_digits(parser);
        // {
        //     while ('0' <= *parser->src_p && *parser->src_p <= '9') {
        //         parser->src_p++;
        //     }
        // }
    }
    if (json_item) {
        json_item->item_end = parser->src_p;
    }

    number = sign * number;
    json_item->item_double = number;
    json_item->item_int = (int)number;
//    printf("==================%g\n", number);

    return 1;
}


static int
parse_object(Parser *parser, JSON *parent, JSON **prev)
{
    JSON *kprev = nil, *vprev = nil;
    JSON *json_item = init_item(parser, parent, prev, '{');
    must(consume(parser, "{"));
    skip_ws(parser);


    if (*parser->src_p != '}') {
        must(parse_pair(parser, json_item, &kprev, &vprev));
        for (skip_ws(parser); *parser->src_p == ','; skip_ws(parser)) {
            parser->src_p++; /* consume , */
            skip_ws(parser);
            must(parse_pair(parser, json_item, &kprev, &vprev));
        }
    }
    must(consume(parser, "}"));
    if (json_item) {
        json_item->item_end = parser->src_p;
    }
    return 1;
}


static int
parse_array(Parser *parser, JSON *parent, JSON **prev)
{
    JSON *json_item = init_item(parser, parent, prev, '[');
    must(consume(parser, "["));
    skip_ws(parser);
    if (*parser->src_p != ']') {
        JSON *aprev = nil;
        must(parse_item(parser, json_item, &aprev));
        for (skip_ws(parser); *parser->src_p == ','; skip_ws(parser)) {
            parser->src_p++; /* consume , */
            skip_ws(parser);
            must(parse_item(parser, json_item, &aprev));
        }
    }
    must(consume(parser, "]"));
    if (json_item) {
        json_item->item_end = parser->src_p;
    }
    return 1;
//    return json_item;
}


static int
parse_key(Parser *parser, JSON *parent, JSON **kprev)
{
    int KEY = 0;
//    JSON *json_item = parse_string(parser, parent, kprev);
//    const char *key = json_item->item_string;
    parse_string(parser, parent, kprev);
    const char *key = parser->json_p->item_string;

    if (!strncmp(key,"STATES",10)) {
        KEY = STATES;
    } else if (!strncmp(key,"STATE_NAME",10)) {
//        printf("key is %s\n", key);
        KEY = STATE_NAME;
    } else if (!strncmp(key,"STATE_DATA",10)) {
//        printf("key is %s\n", key);
        KEY = STATE_DATA;
    } else if (!strncmp(key,"STATE_PARE",10)) {
        KEY = STATE_PARE;
    } else if (!strncmp(key,"STATE_ENTR",10)) {
        KEY = STATE_ENTR;
    } else if (!strncmp(key,"STATE_NEXT",10)) {
        KEY = STATE_ENTR;
    } else if (!strncmp(key,"STATE_TRAN",10)) {
        KEY = STATE_TRAN;
    } else if (!strncmp(key,"TRANS",10)) {
        KEY = TRANS;
    } else if (!strncmp(key,"TRANS_TYPE",10)) {
        KEY = TRANS_TYPE;
    } else if (!strncmp(key,"TRANS_COND",10)) {
//        printf("key is %s\n", key);
        KEY = TRANS_COND;
    } else if (!strncmp(key,"TRANS_GURD",10)) {
        KEY = TRANS_GURD;
    } else if (!strncmp(key,"TRANS_NUM",10)) {
        KEY = TRANS_NUM;
    } else if (!strncmp(key,"TRANS_ACT",10)) {
        KEY = TRANS_ACT;
    } else if (!strncmp(key,"ENTR_ACT",10)) {
        KEY = ENTR_ACT;
    } else if (!strncmp(key,"EXIT_ACT",10)) {
        KEY = EXIT_ACT;
    } else if (!strncmp(key,"EVENTS",10)) {
        KEY = EVENTS;
    } else if (!strncmp(key,"EVENT_TYPE",10)) {
        KEY = EVENT_TYPE;
    } else if (!strncmp(key,"EVENT_DATA",10)) {
        KEY = EVENT_DATA;
    } else if (!strncmp(key,"EVENT_STATE",10)) {
        KEY = EVENT_STATE;
    }

    return KEY;
}

static int
parse_value(Parser *parser, JSON *parent, JSON **vprev)
{
    int VALUE = 0;
//    JSON *json_item = parse_item(parser, parent, vprev);
//    VALUE = json_item->item_type;
    parse_item(parser, parent, vprev);
    VALUE = parser->json_p->item_type;

    return VALUE;
}

// 屬性 狀態STATES 事件EVENTS 轉移TRANS 單獨處理
static int
parse_pair(Parser *parser, JSON *parent, JSON **kprev, JSON **vprev)
{
    int KEY = 0; int VALUE = 0;
    must(*parser->src_p == '"');
    KEY = parse_key(parser, parent, kprev);

    // add
    //解析對象之前先解析一個key,
    switch (KEY) {
        case STATES:
            printf("================states is ...\n");
            break;
        case EVENTS:
            printf("================events is ...\n");
            break;
        default:
            break;
    }

    skip_ws(parser);
    must(consume(parser, ":"));

    skip_ws(parser);
    VALUE = parse_value(parser, parent, vprev);
    printf("VALUE type is :%4d, ",VALUE);

    STATE *a = (STATE*)malloc(sizeof(STATE));
    TRANSITION *a_trans_p  = (TRANSITION*)malloc(sizeof(TRANSITION));
    a->transitions = a_trans_p;
    EVENT *event = (EVENT*)malloc(sizeof(EVENT));

    switch (KEY) {
        case STATES:
            printf("states is ...============end======\n");
            break;
        case STATE_NAME:
//            a->name = strdup_mem(parser->json_p->item_string);
            a->name = parser->json_p->item_string;
            printf("state name is %s\n", a->name);
            break;
        case STATE_DATA:
            a->state_data = parser->json_p->item_string;
            printf("state data is %s\n", (char*)a->state_data);
            break;
        case STATE_PARE:
//            a->parent_state =     // 付一個狀態指針，而這裏的parser->json_p->item_string爲字符串指針
            printf("state's parent is %s\n", parser->json_p->item_string);
            break;
        case STATE_ENTR:
            printf("state's entry is %s\n", parser->json_p->item_string);
            break;
        case STATE_NEXT:
            printf("state's next is %s\n", parser->json_p->item_string);
            break;
        case STATE_TRAN:
            break;
        case TRANS:
            printf("state's transition is \n");
            break;
        case TRANS_TYPE:
            a_trans_p->trans_type = parser->json_p->item_int;
            printf("trans's type is %d\n", a_trans_p->trans_type);
            break;
        case TRANS_COND:
            a_trans_p->condition = parser->json_p->item_string;
            printf("trans's condition is %s\n", (char*)a_trans_p->condition);
            break;
        case TRANS_GURD:
            a_trans_p->guard = parser->json_p->item_string;
            printf("trans's guard condition is %s\n", (char*)a_trans_p->guard);
            break;
        case TRANS_NUM:
            a->num_transitions = parser->json_p->item_int;
            printf("trans's number is %d\n", (int)a->num_transitions);
            break;
        case TRANS_ACT:
            a->transitions->trans_action = parser->json_p->item_string;
            printf("trans's action is %s\n", (char*)a->transitions->trans_action);
            break;
        case ENTR_ACT:
            a->entry_action = parser->json_p->item_string;
            printf("state's entry action is %s\n", (char*)a->entry_action);
            break;
        case EXIT_ACT:
            a->exit_action = parser->json_p->item_string;
            printf("state's exit action is %s\n", (char*)a->exit_action);
            break;
        case EVENTS:
            printf("events is ...============end======\n");
            break;
        case EVENT_TYPE:
            event->event_type = parser->json_p->item_int;
            printf("event type is %d\n", event->event_type);
            break;
        case EVENT_DATA:
            event->data = parser->json_p->item_string;
            printf("event data is %s\n", event->data);
            break;
        case EVENT_STATE:
            event->expectedState = parser->json_p->item_string;
            printf("event expected state is %s\n", event->expectedState);
            break;
        default:
            break;
    }

    return 1;
}


static int
parse_item(Parser *parser, JSON *parent, JSON **prev)
{

    switch (*parser->src_p) {
        case '{':
            return parse_object(parser, parent, prev);
        case '[':
            return parse_array(parser, parent, prev);
        case '"':
            return parse_string(parser, parent, prev);
        case 't':
            return parse_word(parser, parent, prev, "true");  // parse keyword
        case 'f':
            return parse_word(parser, parent, prev, "false");
        case 'n':
            return parse_word(parser, parent, prev, "null");
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return parse_number(parser, parent, prev);
    }
    return 0;

}


static int
parse_text(Parser *parser)
{
    JSON *prev = nil;
    switch (*parser->src_p) {
        case '{': return parse_object(parser, nil, &prev);
        case '[': return parse_array(parser, nil, &prev);
    }
    return 1;
}


int
json_parse(char *src, int n_item)
{
    Parser parser = {};

//    parser_json_item_p = part;

    parser.src_p = src;
    parser.n_item = 0;  // 總共item 計數
    parser.json_p = nil; //parser_json_item_p;
    parser.n_json_deep = n_item;

    skip_ws(&parser);
    must(parse_text(&parser));
    skip_ws(&parser);

    must(*parser.src_p == '\0');

//    if (part) {
        if (parser.n_item < n_item)
            n_item = parser.n_item;
//        }
//        for (int i = 0; i < npart; i++) {
//            part[i].item_len = part[i].item_end - part[i].src;
//            printf("part[%d].item_type:%c, \npart[%d].src:%s, \npart[%d].item_end:%s, \npart[%d].item_len:%d\n",i, part[i].item_type, i, part[i].src, i, part[i].item_end, i, part[i].item_len);
//        }
//    }

    printf("npart:%d\n", n_item);
    printf("n_json_max_deep:%d\n", parser.n_json_deep);

    return parser.n_item;
}






