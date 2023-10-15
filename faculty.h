#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>  // Added for open()
#include "admin.h"


//#include "structure.h"
void addNewCourse(int, struct userCred );
bool authenticatefac(struct userCred);
void viewofferedCourses(int, struct userCred user);
void updateCourse(int , struct userCred);
void removeCourse(int ,struct userCred);
void updatePassword(int);
char username[15];
char userid[15];

bool facultyHandle(int clientSocket) {
    char readbuff1[15], readbuff2[15], readbuff[1024];
    ssize_t readBytes1, readBytes2;
	struct userCred user;
	 memset(user.loginId, '\0', sizeof(user.loginId));
             memset(user.password, '\0', sizeof(user.password));
    send(clientSocket, "Enter loginId", strlen("Enter loginId"), 0);
    readBytes1 = recv(clientSocket, &user.loginId, sizeof(user.loginId), 0);
    if (readBytes1 == -1) {
        perror("Error in the LoginId");
        return false;
    }
    readbuff1[readBytes1] = '\0';
    

    send(clientSocket, "Enter Password", strlen("Enter Password"), 0);
    readBytes2 = recv(clientSocket, user.password, sizeof(user.password), 0);
    if (readBytes2 == -1) {
        perror("Error in the Password");
        return false;
    }
        readbuff1[readBytes1] = '\0';
      
        
    if (authenticatefac(user)) {
        send(clientSocket, "Login Successfully\n", strlen("Login Successfully\n"), 0);

        while (true) {
            char studentPrompt[] = "\t\t...................Welcome To Faculty Menu..................\n"
                                  " - 1. View offered Courses\n"
                                  " - 2. Add new New Course\n"
                                  " - 3. Remove Course from catalog\n"
                                  " - 4. Update Course Details\n"
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
                    viewofferedCourses(clientSocket, user);
                    break;
                case 2:
                     addNewCourse(clientSocket, user);
                    break;
                case 3:
                   removeCourse(clientSocket ,user);
                  
                    break;
                case 4:
                    
                     updateCourse(clientSocket, user);
                    break;
                case 5:
                    updatePassword(clientSocket);
                    break;
                default:
                    return false; 
            }
        }
    }
    else
    send(clientSocket, "Login Failed\n", strlen("Login Failed\n"), 0);
    
    return false;
}

// Function to authenticate a student
bool authenticatefac(struct userCred user) {
    struct faculty temp;
    int openFD = open("faculty_database.txt", O_RDONLY, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        return 0; // Return false for failure
    }

    bool found = false;

    lseek(openFD, 0, SEEK_SET);

    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(user.loginId, temp.loginId) == 0) {
            if (strcmp(user.password, temp.password) == 0) {
             memset(username, '\0', sizeof(username));
              memset(userid, '\0', sizeof(userid));
            strcpy(username,temp.name);
       strcpy(userid,temp.loginId);
                close(openFD);
                return 1; // Return true for successful authentication
            }
        }
    }

    close(openFD);
    return 0; // Return false if the user is not found
}


int generateCid() {
    int fd = open("generate_cid_file.txt", O_RDWR);
if (fd == -1) {
    perror("Error opening generate_sid_file.txt");
	}
	int buf;
	read(fd, &buf, sizeof(buf));
	
	buf = buf+1;
	lseek(fd, 0, SEEK_SET);
	write(fd, &buf, sizeof(buf));
	close(fd);
return buf;

}

void addNewCourse(int clientSocket, struct userCred user) {
    struct courses temp;
    memset(temp.courseName, '\0', sizeof(temp.courseName));
    memset(temp.courseCode, '\0', sizeof(temp.courseCode));
    memset(temp.CourseDept, '\0', sizeof(temp.CourseDept));
    memset(temp.facultyName, '\0', sizeof(temp.facultyName));
    memset(temp.enrolledCount, '\0', sizeof(temp.enrolledCount));
     memset(temp.facultyID, '\0', sizeof(temp.facultyID));
    
    strcpy(temp.enrolledCount, "150");
    int openFD = open("offered_courses_database.txt", O_WRONLY|O_CREAT|O_APPEND, 0644); // Use O_WRONLY and O_APPEND for writing

    if (openFD == -1) {
        perror("Error opening file");
        close(clientSocket);
        return;
    }

    bool found = false;

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Course Name: ", temp.courseName, sizeof(temp.courseName)) == -1) {
        close(clientSocket);
        return;
    }
    strcpy(temp.facultyID,userid);
   strcpy(temp.facultyName,username);
  
    if (sendPromptAndReceiveResponse(clientSocket, "Enter Department to which this course was offered {CS / NC / AI}: ", temp.CourseDept, sizeof(temp.CourseDept)) == -1) {
        close(clientSocket);
        return;
    }
    //sleep(1);
    
    //printf("User entered department: %s\n", temp.CourseDept);

    // Assuming the stud structure is declared and you have a function generateSid() to generate student ID.
    // You should replace this logic with your actual implementation.

	if(strcmp(temp.CourseDept, "CS")==0)
    sprintf(temp.courseCode, "CS50%d", generateCid());
   else if(strcmp(temp.CourseDept, "NC")==0)
   sprintf(temp.courseCode, "NC10%d", generateCid());
  
  else if(strcmp(temp.CourseDept, "AI")==0)
   sprintf(temp.courseCode, "AI20%d", generateCid());
  
  else{
    write(clientSocket, "Invalid Course department selection", sizeof("Invalid Course department selection"));
   return;
  }
    lseek(openFD, 0, SEEK_END);
    ssize_t bytes_written = write(openFD, &temp, sizeof(temp));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return;
    }
	char successMessage[1024] = "";
sprintf(successMessage, "\n\n************ Course added successfully ***************\nCourse Code: %s\nDepartment: %s\nCourse Name: %s\nFaculty Name: %s\nFaculty ID: %s\nEnrolled Count: %s\n******************************************************\n", temp.courseCode, temp.CourseDept, temp.courseName, temp.facultyName, temp.facultyID, temp.enrolledCount);


    send(clientSocket, successMessage, strlen(successMessage), 0);

    close(openFD);
    //close(clientSocket);
}

void viewofferedCourses(int clientSocket, struct userCred user) {
    struct courses temp; 
    int openFD = open("offered_courses_database.txt", O_RDONLY | O_CREAT, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }
    close(openFD);
    openFD = open("offered_courses_database.txt", O_RDONLY | O_CREAT, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    char buffer[1024] = ""; // Initialize the buffer
    buffer[0] = '\0';  // Initialize buffer as an empty string
    lseek(openFD, 0, SEEK_SET);
//write(clientSocket, "\n", strlen("\n"));
  
while (read(openFD, &temp, sizeof(temp)) > 0) {
    if (strcmp(temp.facultyID, userid) == 0) {
        /*char tempbuff[100]="";
        sprintf(tempbuff, "Course Code: %s\tCourse Name: %s\n", temp.courseCode, temp.courseName);
        strcat(buffer, tempbuff);*/
        
        strcat(buffer, "\n");
        strcat(buffer, temp.courseCode);
        strcat(buffer, "-");
        strcat(buffer, temp.courseName);
        
       
        }
    }


close(openFD); // Close the file descriptor

if (strlen(buffer) == 0) {
    // Send a message to the client when no courses are found
    char noCoursesMessage[] = "\n\nNo courses\n\n\n\n";
    write(clientSocket, noCoursesMessage, strlen(noCoursesMessage));
} else {
    // Send the course information to the client
    strcat(buffer, "\n");
    strcat(buffer, "\n");
    write(clientSocket, buffer, strlen(buffer));
}

    close(openFD);
    return;
}
void updateCourse(int clientSocket, struct userCred user) {
	struct courses temp; 
    char cname[100];
      cname[0] = '\0';
    int openFD = open("offered_courses_database.txt", O_RDWR, 0644); // Open in read-only mode

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    bool found = false; // Initialize found to false

    char buffer[1024]; // Declare buffer for sending data

    send(clientSocket, "Enter Course Code: ", strlen("Enter Course Code: "), 0);
    int readResult = read(clientSocket, &cname, sizeof(cname) - 1);

    if (readResult <= 0) {
        send(clientSocket, "Error receiving Course Code from server", strlen("Error receiving Course code from server"), 0);
        return;
    }

    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    // Loop to search for the student in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(userid, temp.facultyID) == 0 && strcmp(cname, temp.courseCode) == 0 ) { // Compare the student IDs
            found = true;
            break;
        }
    }

    if (found) {
    char successMessage[1024];
    sprintf(successMessage, "\n************ Course Current Details ***************\nCourse Code: %s\nDepartment: %s\nCourse Name: %s\nFaculty Name: %s\nFaculty ID: %s\nEnrolled Count: %s\n******************************************************\n", temp.courseCode, temp.CourseDept, temp.courseName, temp.facultyName, temp.facultyID, temp.enrolledCount);


    send(clientSocket, successMessage, strlen(successMessage), 0);
        // Construct the details message
  
	
    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty ID: ", temp.facultyID, sizeof(temp.facultyID)) == -1) {
        close(clientSocket);
        return ;
    }


    
    
    struct faculty fac; 
    bool name_found=false;
    int facFD = open("faculty_database.txt", O_RDONLY | O_CREAT, 0644);

    if (facFD == -1) {
        perror("Error opening file");
        return;
    }

   lseek(facFD, 0, SEEK_SET);
    while (read(facFD, &fac, sizeof(fac)) > 0) {
        if (strcmp(fac.loginId, temp.facultyID) == 0) {
         strcpy(temp.facultyName,fac.name);
         name_found=true;
         break;
        }
    }

    if (name_found) {
        lseek(openFD, -sizeof(temp), SEEK_CUR);
    ssize_t bytes_written = write(openFD, &temp, sizeof(temp));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return ;}
        memset(successMessage,'\0',sizeof(successMessage));
        sprintf(successMessage, "\n************ Course Updated successfully ***************\nCourse Code: %s\nDepartment: %s\nCourse Name: %s\nFaculty Name: %s\nFaculty ID: %s\nEnrolled Count: %s\n******************************************************\n", temp.courseCode, temp.CourseDept, temp.courseName, temp.facultyName, temp.facultyID, temp.enrolledCount);

        // Send the details to the client
        send(clientSocket, successMessage, strlen(successMessage), 0);
    } else {
        send(clientSocket, "\n\nFaculty Not Found\n", strlen("\n\nFaculty Not Found\n"), 0);
    }
    
   
       
    } else {
    char sm[1000];
     memset(sm,'\0',sizeof(sm));
     sprintf(sm, "Either course does not exist or is not associated with ID: %s\n", userid);
        write(clientSocket, sm, sizeof(sm));

    }

    close(openFD); // Close the file after use
}
void removeCourse(int clientSocket, struct userCred user) {
    struct courses temp;
    char cname[100] = "";
    int openFD = open("offered_courses_database.txt", O_RDWR, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        send(clientSocket, "Error opening database file", strlen("Error opening database file"), 0);
        return;
    }

    bool found = false;

    char buffer[1024];

    send(clientSocket, "Enter Course Code: ", strlen("Enter Course Code: "), 0);
    int readResult = read(clientSocket, cname, sizeof(cname) - 1);
    cname[readResult] = '\0'; // Null-terminate the course code

    if (readResult <= 0) {
        send(clientSocket, "Error receiving Course Code from client", strlen("Error receiving Course code from client"), 0);
        fflush(stdin); // Clear the input buffer
        fflush(stdout); // Clear the output buffer
        close(openFD);
        return;
    }

    // Keep track of the position where the record was found
    off_t recordPosition = -1;

    // Loop to search for the course in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(userid, temp.facultyID) == 0 && strcmp(cname, temp.courseCode) == 0) {
            found = true;
            // Store the position of the record
            recordPosition = lseek(openFD, 0, SEEK_CUR) - sizeof(temp);
            break;
        }
    }

    if (found) {
        if (atoi(temp.enrolledCount) < 150) {
            char errorMsg[] = "Cannot be deleted (Students are currently enrolled in this course)";
            write(clientSocket, errorMsg, sizeof(errorMsg));
        } else {
            // Delete the course from the database
            if (recordPosition >= 0) {
                lseek(openFD, recordPosition, SEEK_SET);
                // Fill the record with zeros (or any other placeholder data)
                memset(&temp, 0, sizeof(temp));
                write(openFD, &temp, sizeof(temp));
                write(clientSocket, "Record Deleted", sizeof("Record Deleted"));
            }
        }
    } else {
        write(clientSocket, "Either course does not exist or is not allocated to your ID", strlen("Either course does not exist or is not allocated to your ID"));
    }

    fflush(stdin); // Clear the input buffer
    fflush(stdout); // Clear the output buffer

    close(openFD);
}



void updatePassword(int clientSocket) {
    struct faculty temp;
    bool found;
    char buffer[1024]="";
    int openFD = open("faculty_database.txt", O_RDWR, 0644); // Open in read-only mode

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }


    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    // Loop to search for the student in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.loginId, userid) == 0) { // Compare the student IDs
            found = true;
            break;
        }
    }

    if (found) {
    
   if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Password: ", temp.password, sizeof(temp.password)) == -1) {
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
    
   
       sprintf(buffer, "...................Updated Faculty Data..................\nLogin ID: %s\nPassword: %s\n.............................................\n",
               temp.loginId, temp.password);
   send(clientSocket, buffer, strlen(buffer), 0);

        // Send the details to the client
        send(clientSocket, "Successfully updated\n", strlen("Successfully updated\n"), 0);
    } 

    close(openFD); // Close the file after use
}



