#ifndef PERSON_H
#define PERSON_H

class person
{
public:
    person();
public:
    char *username;
    int len_username;
    char *eidNum;
    int len_eidNum;
    char *skey;
    int len_skey;
//    char *guid;
//    int len_guit;
    char *ti;
    int len_ti;
    char *tm;
    int len_tm;
    char *m;
    int len_m;
    char *m1;
    int len_m1;
    char *y0;
    int len_y0;
    char *y1;
    int len_y1;

    char k1[32];
};

#endif // PERSON_H
