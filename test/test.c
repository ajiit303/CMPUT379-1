#include <stdio.h>
#include <string.h>

int main() {
    char strArray[2][10] = {
        "123456789",
        "!@#$^&*()"
    };

    char * ptr = "abcdefghi";

    strcpy(strArray[0], ptr);

    printf("%s", strArray[0]);

    return 0;
}