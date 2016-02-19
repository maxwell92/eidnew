#include "person.h"
#include "malloc.h"
#include "string.h"

person::person()
{
    len_username = 21;
    username = (char *)malloc(len_username * sizeof(char));
    memset(username, '\0', len_username);

    len_eidNum = 9;
    eidNum = (char *)malloc(len_eidNum * sizeof(char));
    memset(eidNum, '\0', len_eidNum);

    len_skey = 21;
    skey = (char *)malloc(len_skey * sizeof(char));
    memset(skey, '\0', len_skey);

    len_ti = 20; //yyyy:MM:dd:hh:mm:ss
    ti = (char *)malloc(len_ti * sizeof(char));
    memset(ti, '\0', len_ti);

    len_m = 33;
    m = (char *)malloc(len_m * sizeof(char));
    memset(m, '\0', len_m);

    len_m1 = 33;
    m1 = (char *)malloc(len_m1 * sizeof(char));
    memset(m1, '\0', len_m1);

    len_y0 = 33;
    y0 = (char *)malloc(len_y0 * sizeof(char));
    memset(y0, '\0', len_y0);
}
