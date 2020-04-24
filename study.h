#include <stdbool.h>
#include <time.h>

char *readLine(void);
void flushInputBuffer(void);

typedef struct tm Time_tm;

Time_tm *inputTime(void);
char *getDate(const Time_tm *);
char *getTime(const Time_tm *);
time_t compareTime(Time_tm *, Time_tm *);

typedef struct event
{
    bool active;
    char *name;
    char *details;
    char *location;
    struct tm *date_time;
    struct event *next;
} Event;

Event *initEvent(void);
Event *setEvent(Event *, char *, char *, char *, Time_tm *);
Event *inputEvent(void);
void printEvent(Event *);

typedef struct eventList
{
    struct event *head;
    int size;
} EventList;

EventList *initEventList(void);
void pushEvent(EventList *, Event *);
void insertEvent(EventList *, Event *);
void insertEventAt(EventList *, Event *, int);
void printEventList(EventList *);

typedef struct course
{
    bool active;
    char *code;
    char *name;
    char *instructor;
    float credits;
    struct eventList *assignment_list;
    struct eventList *exam_list;
    struct course *next;
} Course;

Course *initCourse(void);
Course *setCourse(Course *, char *, char *, char *, float, EventList *, EventList *);
Course *inputCourse(void);
void printCourse(Course *);

typedef struct courseList
{
    struct course *head;
    int size;
} CourseList;

CourseList *initCourseList(void);
void pushCourse(CourseList *, Course *);
void insertCourse(CourseList *, Course *);
void insertCourseAt(CourseList *, Course *, int);
void printCourseList(CourseList *);

char *getState(bool state);
void displayDateTimeNow(void);
void displayMenu(void);
void quickView(void);
