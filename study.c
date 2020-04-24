#define _XOPEN_SOURCE 700  //used in time.h functions: strptime()

#include "study.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *readLine(void) {
    char *line = NULL;
    int ch;
    line = malloc(sizeof(char));
    if (!line) {
        exit(1);
    }
    line[0] = '\0';
    int index;
    for (index = 0; ((ch = getchar()) != '\n') && (ch != EOF); index++) {
        line = realloc(line, sizeof(char) * (index + 2));
        if (!line) {
            exit(1);
        }
        line[index]     = (char)ch;
        line[index + 1] = '\0';
    }
    fflush(stdin);
    return line;
}

void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* TIME RELATED */
//---------------------------------------------------------------

//wrapper for strptime() parser
Time_tm *inputTime(void) {
    Time_tm *new = malloc(sizeof(Time_tm));
    while (1) {
        printf("Enter date and time(dd/mm/yyyy hh:mm): ");
        //printf("\nexample : 31/03/20 17:38\n");
        char *buf = readLine();
        char *s   = strptime(buf, "%d/%m/%Y %H:%M", new);
        if (s) {
            return new;
        } else
            printf("\nDate/Time Input error. Try again.\n\n");
    }
}

//wrapper for strftime()
char *getDate(const Time_tm *foo) {
    char *date = calloc(sizeof(char), 16);
    strftime(date, 16, "%d/%m/%y", foo);
    return date;
}

//wrapper for strftime()
char *getTime(const Time_tm *foo) {
    char *time = calloc(sizeof(char), 16);
    strftime(time, 16, "%H:%M", foo);
    return time;
}

// t1 > t2 : compareTime() > 0
// t1 < t2 : compareTime() < 0
time_t compareTime(Time_tm *first, Time_tm *second) {
    return mktime(first) - mktime(second);
}

/* EVENT RELATED */
//---------------------------------------------------------------

//null init for any event
Event *initEvent(void) {
    Event *new     = (Event *)malloc(sizeof(Event));
    new->active    = false;
    new->name      = NULL;
    new->details   = NULL;
    new->location  = NULL;
    new->date_time = NULL;
    new->next      = NULL;
    return new;
}

//event fields setter
Event *setEvent(Event *set, char *set_name, char *set_details, char *set_location, Time_tm *set_time) {
    if (!set)
        set = initEvent();
    set->active    = true;
    set->name      = set_name;
    set->details   = set_details;
    set->location  = set_location;
    set->date_time = set_time;
    set->next      = NULL;
    return set;
}

//superwrapper for initEvent() and setEvent()
Event *inputEvent(void) {
    printf("Enter name of event: ");
    char *input_name    = readLine();
    char *input_details = NULL;
    printf("Would you like to enter details(y/n): ");
    if (strcmp(readLine(), "y\0") == 0) {
        printf("Enter details of event: ");
        input_details = readLine();
    }
    printf("Enter location of event: ");
    char *input_location = readLine();
    return setEvent(initEvent(), input_name, input_details, input_location, inputTime());
}

void printEvent(Event *foo) {
    printf("Name     - %s\n", foo->name);
    printf("Details  - %s\n", foo->details);
    printf("Location - %s\n", foo->location);
    printf("Date     - %s\n", getDate(foo->date_time));
    printf("Time     - %s\n", getTime(foo->date_time));
    printf("State    - %s\n", getState(foo->active));
}

/* EVENTLIST RELATED */
//---------------------------------------------------------------

EventList *initEventList(void) {
    EventList *new = (EventList *)malloc(sizeof(EventList));
    new->head      = NULL;
    new->size      = 0;
    return new;
}

void pushEvent(EventList *list, Event *node) {
    node->next = list->head;
    list->head = node;
    ++(list->size);
}

//wrapper for insertEventAt()
void insertEvent(EventList *list, Event *node) {
    Event *trav         = list->head;
    int insert_position = 0;
    while (trav && compareTime(node->date_time, trav->date_time) >= 0) {
        trav = trav->next;
        insert_position++;
    }
    insertEventAt(list, node, insert_position);
}

void insertEventAt(EventList *list, Event *node, int position) {
    if (position < 0 || position > list->size) {
        return;
    } else if (position == 0) {
        pushEvent(list, node);
    } else {
        Event *trav = list->head;
        Event *prev;
        while (position--) {
            prev = trav;
            trav = trav->next;
        }
        prev->next = node;
        node->next = trav;
        ++(list->size);
    }
}

void printEventList(EventList *list) {
    Event *trav = list->head;
    while (trav) {
        printf("\n--------------------\n\n");
        printEvent(trav);
        trav = trav->next;
    }
}

/* COURSE RELATED */
//---------------------------------------------------------------

Course *initCourse(void) {
    Course *new          = (Course *)malloc(sizeof(Course));
    new->active          = false;
    new->code            = NULL;
    new->name            = NULL;
    new->instructor      = NULL;
    new->credits         = 0.0;
    new->assignment_list = NULL;
    new->exam_list       = NULL;
    new->next            = NULL;
    return new;
}

Course *setCourse(Course *set, char *set_code, char *set_name, char *set_instructor, float set_credits, EventList *set_assignment_list, EventList *set_exam_list) {
    if (!set)
        set = initCourse();
    set->active          = true;
    set->code            = set_code;
    set->name            = set_name;
    set->instructor      = set_instructor;
    set->credits         = set_credits;
    set->assignment_list = set_assignment_list;
    set->exam_list       = set_exam_list;
    set->next            = NULL;
    return set;
}

Course *inputCourse(void) {
    printf("Enter course code: ");
    char *input_code = readLine();
    printf("Enter name of course: ");
    char *input_name = readLine();
    printf("Enter name of instructor: ");
    char *input_instructor = readLine();
    printf("Enter course credits: ");
    float input_credits;
    scanf("%f", &input_credits);
    flushInputBuffer();
    return setCourse(initCourse(), input_code, input_name, input_instructor, input_credits, initEventList(), initEventList());
}

void printCourse(Course *foo) {
    printf("Code               - %s\n", foo->code);
    printf("Name               - %s\n", foo->name);
    printf("Instructor         - %s\n", foo->instructor);
    printf("Credits            - %.1f\n", foo->credits);
    printf("No. of Assignments - %d\n", foo->assignment_list->size);
    printf("No. of Exams       - %d\n", foo->exam_list->size);
    printf("State              - %s\n", getState(foo->active));
}

/* COURSELIST RELATED */
//---------------------------------------------------------------

CourseList *initCourseList(void) {
    CourseList *new = (CourseList *)malloc(sizeof(CourseList));
    new->head       = NULL;
    new->size       = 0;
    return new;
}

void pushCourse(CourseList *list, Course *node) {
    node->next = list->head;
    list->head = node;
    ++(list->size);
}

void insertCourse(CourseList *list, Course *node) {
    Course *trav        = list->head;
    int insert_position = 0;
    while (trav && node->credits >= trav->credits) {
        trav = trav->next;
        insert_position++;
    }
    insertCourseAt(list, node, insert_position);
}

void insertCourseAt(CourseList *list, Course *node, int position) {
    if (position < 0 || position > list->size) {
        return;
    } else if (position == 0) {
        pushCourse(list, node);
    } else {
        Course *trav = list->head;
        Course *prev;
        while (position--) {
            prev = trav;
            trav = trav->next;
        }
        prev->next = node;
        node->next = trav;
        ++(list->size);
    }
}

void printCourseList(CourseList *list) {
    Course *trav = list->head;
    while (trav) {
        printf("\n--------------------\n\n");
        printCourse(trav);
        trav = trav->next;
    }
}

/* MISC */
//---------------------------------------------------------------

char *getState(bool state) {
    static char str[16];
    if (state)
        strcpy(str, "Active");
    else
        strcpy(str, "Inactive");
    return str;
}

void displayDateTimeNow(void) {
    time_t t     = time(NULL);
    Time_tm *now = localtime(&t);
    printf("%s %s\n", getTime(now), getDate(now));
}

void displayMenu(void) {
    printf("\nMENU");
    printf("\n1. ASSIGNMENTS");
    printf("\n2. EXAMS");
    printf("\n3. COURSE DETAILS");
    printf("\n4. EXIT");
}

void quickView(void) {
    //Couse *trav
}
