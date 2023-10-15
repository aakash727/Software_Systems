#include "structure.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

void addStudent(int clientSocket);
void viewStudent(int clientSocket);
void addFaculty(int clientSocket);
//void viewFacultyDetails(int clientSocket);
//void activateStudent(int clientSocket);
//void blockStudent(int clientSocket);
//void modifyStudentDetails(int clientSocket);
//void modifyFacultyDetails(int clientSocket);
//void logoutAndExit(int clientSocket);

#define studentFile "studentFile.txt"
#define facultyFile "facultyFile.txt"

bool adminHandle(int clientSocket) {
    if (1) {
    char readbuff[1024];
        send(clientSocket, "Login Successfully\n", strlen("Login Successfully\n"), 0);
        char adminPrompt[] = "\t\tWelcome to Admin Menu\n - 1.Add Student\n - 2.View Student Details\n - 3.Add Faculty\n - 4.View Faculty Details\n - 5.Activate Student\n - 6.Block Student\n - 7.Update Student\n - 8.Update Faculty\n - 9.Logout and Exit\n";

        send(clientSocket, adminPrompt, strlen(adminPrompt), 0);
        // readBytes stores the number of bytes read from the client by the server
        ssize_t readBytes = recv(clientSocket, readbuff, sizeof(readbuff), 0);
        if (readBytes == -1) {
            perror("Error in the choice you provided");
            return false;
        }
        int choice = atoi(readbuff);
         //send(clientSocket, readbuff, sizeof(readbuff), 0);

        switch (choice) {
            case 1:
                addStudent(clientSocket);
                break;
             case 2:   
                viewStudent(clientSocket);
                break;
          case 3:
          addFaculty(clientSocket);
            
            default:
                break;
        }
    }
    return false;
}


void addStudent(int clientSocket) {
    struct student stud;

    int openFD = open(studentFile, O_RDWR | O_CREAT | O_APPEND , 0644); // Provide appropriate permissions

    if (openFD == -1) {
        perror("open");
        close(clientSocket);
        return;
    }

  send(clientSocket, "Enter Student roll no. as loginId: ", strlen("Enter Student roll no. as loginId: : "), 0);
    ssize_t bytes_received = recv(clientSocket, stud.loginId, strlen(stud.loginId) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    stud.loginId[bytes_received] = '\0'; // Null-terminate the received string
    
   if (stud.loginId[bytes_received - 1] == '\n') 
        stud.loginId[bytes_received - 1] = '\0';

 
    send(clientSocket, "Enter Student Password: ", strlen("Enter Student Password: "), 0);
     bytes_received = recv(clientSocket, stud.password, strlen(stud.password) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    stud.name[bytes_received] = '\0'; // Null-terminate the received string

   if (stud.password[bytes_received - 1] == '\n') 
        stud.password[bytes_received - 1] = '\0';
    
    send(clientSocket, "Enter Student Name: ", strlen("Enter Student Name: "), 0);
    bytes_received = recv(clientSocket, stud.name, strlen(stud.name) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    stud.name[bytes_received] = '\0'; // Null-terminate the received string

   if (stud.name[bytes_received - 1] == '\n')
        stud.name[bytes_received - 1] = '\0';
    
     
   
    send(clientSocket, "Enter Student Age: ", strlen("Enter Student Age: "), 0);
    bytes_received = recv(clientSocket, stud.age, strlen(stud.age) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    stud.age[bytes_received] = '\0'; // Null-terminate the received string

   if (stud.age[bytes_received - 1] == '\n') {
        stud.age[bytes_received - 1] = '\0';}
   
    send(clientSocket, "Enter Student Email Address: ", strlen("Enter Student Email Address: "), 0);
    bytes_received = recv(clientSocket, stud.emailAddress, strlen(stud.emailAddress) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    stud.emailAddress[bytes_received] = '\0'; // Null-terminate the received string
    
   if (stud.emailAddress[bytes_received - 1] == '\n') 
        stud.emailAddress[bytes_received - 1] = '\0';

    lseek(openFD, 0, SEEK_END);
    ssize_t bytes_written = write(openFD, &stud, sizeof(stud));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return;
    }

    close(openFD);

    // Optionally, you can send a success message to the client.
    const char successMessage[] = "Student added successfully.\n";
    send(clientSocket, successMessage, strlen(successMessage), 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////



void viewStudent(int clientSocket) {
    struct student my_student, temp;
    int openFD = open("studentFile.txt", O_RDWR | O_CREAT, 0644); // Provide appropriate file path and permissions

    bool found = false; // Initialize found to false

    char buffer[1024]; // Declare buffer for sending data
    

    send(clientSocket, "Enter Student ID", strlen("Enter Student ID"), 0);
    int readResult = recv(clientSocket, my_student.loginId, sizeof(my_student.loginId) - 1, 0);
    if (readResult <= 0) {
        send(clientSocket, "Error receiving student ID from server", strlen("Error receiving student ID from server"), 0);
        return;
    }
    my_student.loginId[readResult] = '\0';
     if (my_student.loginId[readResult - 1] == '\n') {
        my_student.loginId[readResult - 1] = '\0';}

    // Use a separate buffer to store the student ID
    int id=atoi(my_student.loginId);
    //strcpy(buff, my_student.loginId);

    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    // Loop to search for the student in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (my_student.loginId==temp.loginId) {
            found = true;
            lseek(openFD, -sizeof( temp), SEEK_CUR);
            break;
        }
    }

    if (found) {
        // Construct the details message
        sprintf(buffer, "...................Student..................\nName: %s\nAge: %s\nLogin ID: %s\nPassword: %s\n.............................................\n",
                my_student.name, my_student.age, my_student.loginId, my_student.password);

        // Send the details to the client
        send(clientSocket, buffer, strlen(buffer), 0);
    } else {
        
        send(clientSocket, "Student not found\n", strlen("Student not found\n"), 0);
        return;
    }

    close(openFD); // Close the file after use
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////




void addFaculty(int clientSocket) {
    struct faculty fac;

    int openFD = open(facultyFile, O_RDWR | O_CREAT | O_APPEND , 0644); // Provide appropriate permissions

    if (openFD == -1) {
        perror("open");
        close(clientSocket);
        return;
    }

 
    send(clientSocket, "Enter Faculty Name: ", strlen("Enter Faculty Name: "), 0);
    ssize_t bytes_received = recv(clientSocket, fac.name, strlen(fac.name) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    fac.name[bytes_received] = '\0'; // Null-terminate the received string

   
    send(clientSocket, "Enter Faculty LoginId: ", strlen("Enter Faculty LoginId: "), 0);
    bytes_received = recv(clientSocket, fac.loginId, strlen(fac.loginId) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    fac.loginId[bytes_received] = '\0'; // Null-terminate the received string

    
    send(clientSocket, "Enter Faculty Password: ", strlen("Enter Faculty Password: "), 0);
    bytes_received = recv(clientSocket, fac.password, strlen(fac.password) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    fac.password[bytes_received] = '\0'; // Null-terminate the received string

    
    send(clientSocket, "Enter Faculty Age: ", strlen("Enter Faculty Age: "), 0);
    bytes_received = recv(clientSocket, fac.age, strlen(fac.age) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    fac.age[bytes_received] = '\0'; // Null-terminate the received string

   
    send(clientSocket, "Enter Faculty Email Address: ", strlen("Enter Faculty Email Address: "), 0);
    bytes_received = recv(clientSocket, fac.emailAddress, strlen(fac.emailAddress) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    fac.emailAddress[bytes_received] = '\0'; // Null-terminate the received string
    
    
    send(clientSocket, "Enter Faculty Departmment: ", strlen("Enter Faculty Departmment: "), 0);
    bytes_received = recv(clientSocket, fac.department, strlen(fac.department) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    fac.department[bytes_received] = '\0'; // Null-terminate the received string
    
    
    send(clientSocket, "Enter Faculty Designation: ", strlen("Enter Faculty Designation: "), 0);
    bytes_received = recv(clientSocket, fac.designation, strlen(fac.designation) - 1, 0);

    if (bytes_received == -1) {
        perror("recv");
        close(openFD);
        close(clientSocket);
        return;
    }
    fac.designation[bytes_received] = '\0'; // Null-terminate the received string

    lseek(openFD, 0, SEEK_END);
    ssize_t bytes_written = write(openFD, &fac, sizeof(fac));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return;
    }

    close(openFD);

    // Optionally, you can send a success message to the client.
    const char successMessage[] = "Faculty added successfully.\n";
    send(clientSocket, successMessage, strlen(successMessage), 0);
}


