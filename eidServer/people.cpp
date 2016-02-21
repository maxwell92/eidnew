#include "people.h"
#include "string.h"

people::people()
{
    memset(eidNumber, '\0', 9);

    memset(realName, '\0', 20);
    memset(teleNumber, '\0', 14);
    memset(age, '\0', 3);
    memset(company, '\0', 50);
    memset(gender, '\0', 2);
    memset(address, '\0', 100);
    memset(city, '\0', 10);
    memset(idNumber, '\0', 19);
}
