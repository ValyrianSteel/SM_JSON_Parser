//
// Created by ubun on 16-5-13.
//

#ifndef LC_PARSE_H
#define LC_PARSE_H

typedef struct JSON JSON;
struct JSON {
    char item_type; /* one of: { [ " 0 t f n (0:numLable true false null)*/
    int  item_len;
    char *src;
    char *item_end; /* src + item_len */

    JSON *parent;
    JSON *next;
    JSON *prev;

    // JSON（ITEM）的類型 對應的直
    char *item_string;			/* The item's string, if item_type=='\"' */
    int item_int;				/* The item's number, if item_type=='0' */
    double item_double;			/* The item's number, if item_type=='0' */
};

//int json_parse(char *src, JSON *val, int nval);
int json_parse(char *src, int n_item);




typedef struct event_ EVENT;  
struct event_
{
    int event_type;							// EVENT_TYPE

    char *data;								// EVENT_DATA
    const char *expectedState;				// EVENT_STATE
};


typedef struct state_ STATE;                // STATES
typedef struct transition_ TRANSITION;      // TRANS
struct transition_
{
    int trans_type;  //轉換的類型              // TRANS_TYPE
    void *condition;                        // TRANS_COND
    void *guard;                            // TRANS_GURD
    void *trans_action;                 // TRANS_ACT
    STATE *nextState;                       // STATE_NEXT
};
struct state_
{
	char *name;                             // STATE_NAME   
    void *state_data;   // STATE_DATA
    STATE *parent_state;                    // STATE_PARE
    STATE *entry_state;                      // STATE_ENTR
    TRANSITION *transitions;                // STATE_TRAN
    size_t num_transitions;                 // TRANS_NUM
    void *entry_action;                     // ENTR_ACT
    void *exit_action;                      // EXIT_ACT
};




void test_parser();

#endif //LC_PARSE_H
