//
// Created by ubun on 16-5-19.
//
#include <stdio.h>
#include "parse.h"

void test_parser()
{
    int n = 100;

    int a = json_parse("{\
        \"STATES\": [{          \
        \"STATE_NAME\": \"STATE1\", \
        \"STATE_DATA\": \"1\",\
        \"STATE_ENTR\":   null,      \
        \"STATE_PARE\":  \"STATE9\",      \
        \"ENTR_ACT\":  \"entry action...\",      \
        \"EXIT_ACT\":   \"exit state1\",      \
        \"TRANS\":  {        \
            \"TRANS_TYPE\":    0,   \
            \"TRANS_COND\":    \"condition\",\
            \"TRANS_GURD\":    \"guard_what\",\
            \"TRANS_ACT\":   \"transitionAction..js...\",\
            \"STATE_NEXT\":    \"STATE3\"\
        },\
        \"TRANS_NUM\":   1 }], \
        \"EVENTS\": [{        \
          \"EVENT_TYPE\": 1, \
          \"EVENT_DATA\": \"state9\", \
          \"EVENT_STATE\": \"STATE9\" }] }", n);

    if (a == 0)
        printf("bad json");
    if (a > n)
        printf("too many values");
    printf("%d\n", a);
//    for (int i=0; i<n; i++)
//        printf("%.*s;\n", x[i].item_end, x[i].src);

//    for (int i = 0; i < n; i++) {
//        printf("%p, ", &x[i]);
//    }
}