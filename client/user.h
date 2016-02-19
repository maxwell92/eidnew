#ifndef USER_H
#define USER_H

class user
{
public:
    user();
    ~user();

//private:
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
    char *m;
    int len_m;
    char *m1;
    int len_m1;
    char *y0;
    int len_y0;
};

#endif // USER_H
