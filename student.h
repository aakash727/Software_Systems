#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>  




bool authenticatestud(struct userCred);
void viewallCourses(int );
void enrolledCourses(int );
void viewCourses(int );
void dropCourse(int );
void viewenrolledcourses(int );
  void updatestudentPassword(int ) ;

struct student user_student;


/*int customStrCmp(const char *str1, const char *str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return (*str1 - *str2);
}*/

// Function to handle student interactions
bool studentHandle(int clientSocket) {
    char readbuff1[15], readbuff2[15], readbuff[1024];
    ssize_t readBytes1, readBytes2;
	struct userCred user;
	 memset(user.loginId, '\0', sizeof(user.loginId));
             memset(user.password, '\0', sizeof(user.password));
    send(clientSocket, "Enter loginId\n", strlen("Enter loginId\n"), 0);
    readBytes1 = recv(clientSocket, &user.loginId, sizeof(user.loginId), 0);
    if (readBytes1 == -1) {
        perror("Error in the LoginId");
        return false;
    }
    readbuff1[readBytes1] = '\0';
    

    send(clientSocket, "Enter Password\n", strlen("Enter Password\n"), 0);
    readBytes2 = recv(clientSocket, user.password, sizeof(user.password), 0);
    if (readBytes2 == -1) {
        perror("Error in the Password");
        return false;
    }
        readbuff1[readBytes1] = '\0';
      
        
    if (authenticatestud(user)) {
        send(clientSocket, "Login Successfully\n", strlen("Login Successfully\n"), 0);

        while (true) {
            char studentPrompt[] = "\n\n\t\t...................Welcome To Student Menu..................\n"
                                  " - 1. View All Courses\n"
                                  " - 2. Enroll into New Course\n"
                                  " - 3. Drop Course\n"
                                  " - 4. View Enrolled Course\n"
                                  " - 5. Change Password\n"
                                  " - 6. Logout and Exit\n";

            write(clientSocket, studentPrompt, strlen(studentPrompt));

            ssize_t readBytes = recv(clientSocket, readbuff, sizeof(readbuff), 0);
            if (readBytes == -1) {
                perror("Error in the choice you provided");
                return false;
            }

            int choice = atoi(readbuff);

            switch (choice) {
                case 1:
                    viewallCourses(clientSocket);
                    break;
                case 2:
                    enrolledCourses(clientSocket);
                    break;
                case 3:
                     dropCourse(clientSocket);
                    break;
                case 4:
                     viewCourses(clientSocket);
                    break;
                case 5:
                       updatestudentPassword(clientSocket);
                default:
                    return false; // Logout and exit
            }
        }
    }
    else
    send(clientSocket, "Login Failed\n", strlen("Login Failed\n"), 0);
    
    return false;
}

// Function to authenticate a student
bool authenticatestud(struct userCred user) {
    struct student temp;

    int openFD = open("student_database.txt", O_RDONLY, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        return false;
    }

    bool found = false;

    lseek(openFD, 0, SEEK_SET);

    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(user.loginId, temp.loginId) == 0) {
            if (strcmp(user.password, temp.password) == 0) {
            
            user_student=temp;
                close(openFD);
                return true; // Authentication successful
            } 
            }
        }
    

    close(openFD);
    return false; // User not found
}
void viewallCourses(int clientSocket) {
    struct courses temp;
    char buffer[1024]="";
    int openFD = open("offered_courses_database.txt", O_RDONLY|O_CREAT, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    lseek(openFD, 0, SEEK_SET);

    while (read(openFD, &temp, sizeof(temp)) > 0) {
    if(strcmp(temp.courseCode,"\0")==0){}
    else{
        strcat(buffer,"\n");
        strcat(buffer,"Course Code: ");
        strcat(buffer,temp.courseCode);
        strcat(buffer,"\n");
        strcat(buffer,"Course Name: ");
        strcat(buffer,temp.courseName);
        strcat(buffer,"\n");
        strcat(buffer,"Faculty Name: ");
        strcat(buffer,temp.facultyName);
        strcat(buffer,"\n");
        strcat(buffer,"Number of available enrollments: ");
        strcat(buffer,temp.enrolledCount);
        strcat(buffer,"\n");
        strcat(buffer,"________________\n");
        }}
    
        send(clientSocket, buffer, strlen(buffer), 0);

        close(openFD);
        return;
    }
void enrolledCourses(int clientSocket) {
    struct courses temp;
    struct student stud;

    char cname[51] = "";  // Increased buffer size to account for null-termination
    int openFD_offered = open("offered_courses_database.txt", O_RDWR | O_CREAT, 0777);
    int openFD_Student = open("student_database.txt", O_RDWR | O_CREAT, 0777);

    if (openFD_offered == -1 || openFD_Student == -1) {
        perror("Error opening files");
        close(openFD_offered);
        close(openFD_Student);
        return;
    }

    write(clientSocket, "Enter Course Code: ", strlen("Enter Course Code: "));
    int readResult = recv(clientSocket, cname, sizeof(cname) - 1, 0);
    if (readResult == -1) {
        perror("Error in the Course Code");
        close(openFD_offered);
        close(openFD_Student);
        return;
    }
    cname[readResult] = '\0';

    bool found = false;
lseek(openFD_offered, 0, SEEK_SET);
    while (read(openFD_offered, &temp, sizeof(temp)) > 0) {
        if (strcmp(cname, temp.courseCode) == 0) {
            lseek(openFD_Student, 0, SEEK_SET);
            while (read(openFD_Student, &stud, sizeof(stud)) > 0) {
                if (strcmp(user_student.loginId, stud.loginId) == 0) {
                
                
                for (int i = 0; i < atoi(stud.no_courses); i++) {
                    if (strcmp(stud.subjects[i], cname) == 0) {
                        send(clientSocket, "Already registered.", strlen("Already registered."), 0);
                        return; // Exit the function if already registered
                    }
                }
               
                    found = true;
                    break;
                }
            }
            break;
        }
    }

    if (found) {
        int index = atoi(stud.no_courses);
        int rec = atoi(temp.enrolledCount);

        if (rec <= 0) {
            write(clientSocket, "Enrollments are full. You can't enroll", strlen("Enrollments are full. You can't enroll"));
        } else if (index >= 5) {
            write(clientSocket, "You have registered a total of 5 subjects, and you are unable to register any more", strlen("You have registered a total of 5 subjects, and you are unable to register any more"));
        } else {
            // Allocate memory for the new subject and update student data
            strcpy(stud.subjects[index],cname); // Assuming you want to allocate memory for each subject
            index++;
            sprintf(stud.no_courses, "%d", index);
            
            rec--;
            sprintf(temp.enrolledCount, "%d", rec);

            // Write the updated student data to the enrolled courses file
            lseek(openFD_Student, -sizeof(stud), SEEK_CUR);
            write(openFD_Student, &stud, sizeof(stud));

            // Update the course in the offered course database
            lseek(openFD_offered, -sizeof(temp), SEEK_CUR);
            write(openFD_offered, &temp, sizeof(temp));

            write(clientSocket, "Successfully Enrolled", strlen("Successfully Enrolled"));
        }
    }

    // Clean buffers and close files
    memset(cname, 0, sizeof(cname));
    close(openFD_offered);
    close(openFD_Student);
}


void viewCourses(int clientSocket) {
    struct student temp;
    char buffer[1024] = ""; // Initialize with null-terminating character
    int openFD = open("student_database.txt", O_RDONLY | O_CREAT, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    lseek(openFD, 0, SEEK_SET);

    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.loginId, user_student.loginId) == 0) {
            int index = atoi(temp.no_courses);
            for (int i = 0; i < index; i++) {
                strcat(buffer, "\n");
                strcat(buffer, "Course Code: ");
                strcat(buffer, temp.subjects[i]);
            }
        }
    }

    send(clientSocket, buffer, strlen(buffer), 0);

    close(openFD);
    return;
}










void dropCourse(int clientSocket){
    struct courses temp;
    struct student stud;

    char cname[51] = "";  // Increased buffer size to account for null-termination
    int openFD_offered = open("offered_courses_database.txt", O_RDWR | O_CREAT, 0777);
    int openFD_Student = open("student_database.txt", O_RDWR | O_CREAT, 0777);

    if (openFD_offered == -1 || openFD_Student == -1) {
        perror("Error opening files");
        close(openFD_offered);
        close(openFD_Student);
        return;
    }

    write(clientSocket, "Enter Course Code: ", strlen("Enter Course Code: "));
    int readResult = recv(clientSocket, cname, sizeof(cname) - 1, 0);
    if (readResult == -1) {
        perror("Error in the Course Code");
        close(openFD_offered);
        close(openFD_Student);
        return;
    }
    cname[readResult] = '\0';

    bool found = false;
lseek(openFD_offered, 0, SEEK_SET);
    while (read(openFD_offered, &temp, sizeof(temp)) > 0) {
        if (strcmp(cname, temp.courseCode) == 0) {
            lseek(openFD_Student, 0, SEEK_SET);
            while (read(openFD_Student, &stud, sizeof(stud)) > 0) {
                if (strcmp(user_student.loginId, stud.loginId) == 0) {
                
                
                for (int i = 0; i < 5; i++) {
                    if (strcmp(stud.subjects[i], cname) == 0) {
                    
                    for (int del= i; del < 4; del++) {
        			strcpy(stud.subjects[del], stud.subjects[del + 1]);
    				}
                                  send(clientSocket, "successfully deleted.", strlen("successfully deleted."), 0);
                       		found = true; 
                    }
                }
               
                    break;
                }
            }
            break;
        }
    }

    if (found) {
        int index = atoi(stud.no_courses);
        int rec = atoi(temp.enrolledCount);

        
            // Allocate memory for the new subject and update student data
            strcpy(stud.subjects[index],cname); // Assuming you want to allocate memory for each subject
            index--;
            sprintf(stud.no_courses, "%d", index);
            
            rec++;
            sprintf(temp.enrolledCount, "%d", rec);

            // Write the updated student data to the enrolled courses file
            lseek(openFD_Student, -sizeof(stud), SEEK_CUR);
            write(openFD_Student, &stud, sizeof(stud));

            // Update the course in the offered course database
            lseek(openFD_offered, -sizeof(temp), SEEK_CUR);
            write(openFD_offered, &temp, sizeof(temp));

            write(clientSocket, "Successfully Enrolled", strlen("Successfully Enrolled"));
        
    }

    // Clean buffers and close files
    memset(cname, 0, sizeof(cname));
    close(openFD_offered);
    close(openFD_Student);
}

/*
void viewenrolledcourses(int clientSocket) {
    struct student temp;
    struct courses cid;
    char buffer[1024] = ""; // Initialize with a null-terminating character
    int openFD = open("student_database.txt", O_RDWR | O_CREAT, 0644); // Use O_RDWR
    int openFD_offered = open("offered_courses_database.txt", O_RDONLY | O_CREAT, 0644); // Use O_RDONLY
    
    if (openFD == -1 || openFD_offered == -1) {
        perror("Error opening file");
        return;
    }

    lseek(openFD, 0, SEEK_SET);

    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.loginId, user_student.loginId) == 0) {
            int index = atoi(temp.no_courses);
            for (int i = 0; i < index; i++) {
                lseek(openFD_offered, 0, SEEK_SET);
                while (read(openFD_offered, &cid, sizeof(cid)) > 0) {
                    if (strcmp(cid.courseName, temp.subjects[i]) == 0) { // Fixed variable name and comparison
                    
                        sprintf(buffer, "\n\nCourse Name: %s\nCourse Code: %s\nCourse Department: %s\nFaculty Name: %s\nFaculty ID: %s\nEnrolled Count: %s",
                                cid.courseName, cid.courseCode, cid.CourseDept, cid.facultyName, cid.facultyID, cid.enrolledCount); // Fixed sprintf formatting
                                
                                strcat(buffer, "\n");
                strcat(buffer, "Course Code: ");
                strcat(buffer, cid.courseCode);
               
                strcat(buffer, "Course Name: ");
                strcat(buffer, cid.courseName);
                
                strcat(buffer, "nFaculty Name: ");
                strcat(buffer, cid.facultyID);
            
                strcat(buffer, "Enrolled CoCountde: ");
                strcat(buffer, cid.enrolledCount);
                    
                        // Send course information inside the loop
                        
                    }
                    send(clientSocket, buffer, strlen(buffer), 0);
                }
            }
        }
    }
	send(clientSocket, buffer, strlen(buffer), 0);
    close(openFD);
    close(openFD_offered);
}
void viewenrolledcourses(int clientSocket) {
    struct student temp;
    struct courses cid;
    char buffer[1024] = ""; // Initialize with a null-terminating character
    int openFD = open("student_database.txt", O_RDONLY);
    int openFD_offered = open("offered_courses_database.txt", O_RDONLY);

    if (openFD == -1 || openFD_offered == -1) {
        perror("Error opening file");
        return;
    }

    lseek(openFD, 0, SEEK_SET);
    
    // Clear the buffer before entering the loop
    buffer[0] = '\0';

    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.loginId, user_student.loginId) == 0) {
            int index = atoi(temp.no_courses);
            printf("%s",temp.loginId);
            for (int i = 0; i < index; i++) {
                lseek(openFD_offered, 0, SEEK_SET);
                while (read(openFD_offered, &cid, sizeof(cid)) > 0) {
                    if (strcmp(cid.courseName, temp.subjects[i]) == 0) {
                        // Clear the buffer before adding new data
                        printf("3\n");
                        buffer[0] = '\0';

                        sprintf(buffer, "\nCourse Name: %s\nCourse Code: %s\nCourse Department: %s\nFaculty Name: %s\nFaculty ID: %s\nEnrolled Count: %s",
                                cid.courseName, cid.courseCode, cid.CourseDept, cid.facultyName, cid.facultyID, cid.enrolledCount);
                        strcat(buffer, "\n");

                        send(clientSocket, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }

    // Close files after sending data
    close(openFD);
    close(openFD_offered);
}
   */
   
   void updatestudentPassword(int clientSocket) {
    struct student temp;
    bool found;
    char buffer[1024]="";
    int openFD = open("student_database.txt", O_RDWR, 0644); // Open in read-only mode

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }


    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    // Loop to search for the student in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.loginId, user_student.loginId) == 0) { // Compare the student IDs
            found = true;
            break;
        }
    }

    if (found) {
    
   if (sendPromptAndReceiveResponse(clientSocket, "Enter new Password: ", temp.password, sizeof(temp.password)) == -1) {
        close(clientSocket);
        return ;
    }

   
    
    lseek(openFD, -sizeof(temp), SEEK_CUR);
    ssize_t bytes_written = write(openFD, &temp, sizeof(temp));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return ;
    }
    
   
       sprintf(buffer, "...................Updated Student Data..................\nLogin ID: %s\nPassword: %s\n.............................................\n",
               temp.loginId, temp.password);
   send(clientSocket, buffer, strlen(buffer), 0);

        // Send the details to the client
        send(clientSocket, "Successfully updated\n", strlen("Successfully updated\n"), 0);
    } 

    close(openFD); // Close the file after use
}



