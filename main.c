#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "study.h"

int main() {
    while (1) {
        printf("\nSTUDY ORGANISER\n");
        displayDateTimeNow();
        //quickView();
        displayMenu();
        getchar();
    }
    return 0;
}
