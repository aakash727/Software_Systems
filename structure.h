struct userCred{
char loginId[10];
char password[10];

};
struct student{

char loginId[15];
char password[15];
char name[15];
char age[15];
char emailAddress[15];
char no_courses[15];
char subjects[5][10];
};

struct faculty{

char loginId[10];
char password[20];
char name[20];
char age[3];
char emailAddress[50];
char department[4];


};



struct courses{
char courseCode[10];
char CourseDept[10];
char courseName[20];
char facultyName[15];
char facultyID[15];
char enrolledCount[10];
 
};
