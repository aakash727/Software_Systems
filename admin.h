#include "structure.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include "student.h"

  
int addStudent(int clientSocket);
void viewStudent(int clientSocket);
int addFaculty(int clientSocket);
void viewFaculty(int clientSocket);
//void activateStudent(int clientSocket);
//void blockStudent(int clientSocket);
void modifyStudent(int clientSocket);
int generateSid();
int generateFid();
void modifyFaculty(int clientSocket);


bool adminHandle(int clientSocket) {
    if (1) {
    char readbuff[1024];
        send(clientSocket, "Login Successfully\n", strlen("Login Successfully\n"), 0);
       while(true){
      char adminPrompt[] = "\t\tWelcome to Admin Menu\n"
                     "---------------------------------\n"
                     " 1. Add Student\n"
                     " 2. View Student Details\n"
                     " 3. Add Faculty\n"
                     " 4. View Faculty Details\n"
                     " 5. Activate Student\n"
                     " 6. Block Student\n"
                     " 7. Update Student\n"
                     " 8. Update Faculty\n"
                     " 9. Logout and Exit\n"
                     "---------------------------------\n"
                     "Please select an option: ";

        write(clientSocket, adminPrompt, strlen(adminPrompt));

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
                addStudent(clientSocket) ;
                break;
             case 2:   
                viewStudent(clientSocket);
                break;
          case 3:
          	addFaculty(clientSocket);
          break;
          case 4:
          	viewFaculty(clientSocket);
          break;
          case 7:
           modifyStudent(clientSocket);  
      break;  
          case 8:
           modifyFaculty(clientSocket);  
      break; 
            default:
            return true; 
                break;
        }
      }
    }
    return false;
}


int sendPromptAndReceiveResponse(int clientSocket, const char *prompt, char *response, size_t responseSize) {
    send(clientSocket, prompt, strlen(prompt), 0);
	ssize_t bytes_received = read(clientSocket, response, responseSize - 1);


    if (bytes_received <= 0) {
        perror("recv");
        return -1;
    }

    response[bytes_received] = '\0'; // Null-terminate the received string
    if (response[bytes_received - 1] == '\n') {
        response[bytes_received - 1] = '\0';
    }

    return 0; // Success
}

int addStudent(int clientSocket) {
    struct student stud;
    memset(stud.name, '\0', sizeof(stud.name));
    memset(stud.loginId, '\0', sizeof(stud.loginId));
    memset(stud.password, '\0', sizeof(stud.password));
    memset(stud.age, '\0', sizeof(stud.age));
    memset(stud.emailAddress, '\0', sizeof(stud.emailAddress));
    memset(stud.no_courses, '\0', sizeof(stud.no_courses));
    strcpy(stud.no_courses,"0");
    
    for (int i = 0; i < 7; i++) {
    strcpy(stud.subjects[i],"");
}
    

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Student Name: ", stud.name, sizeof(stud.name)) == -1) {
        close(clientSocket);
        return 0;
    }
 
    sprintf(stud.loginId,"MT%d",generateSid());	
    //if (sendPromptAndReceiveResponse(clientSocket, "Enter Student LoginId: ", stud.loginId, sizeof(stud.loginId)) == -1) {
       // close(clientSocket);
       // return 0;
    //}

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Password for generated loginId: ", stud.password, sizeof(stud.password)) == -1) {
        close(clientSocket);
        return 0;
    }

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Student Age: ", stud.age, sizeof(stud.age)) == -1) {
        close(clientSocket);
        return 0;
    }

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Student Email Address: ", stud.emailAddress, sizeof(stud.emailAddress)) ==-1) {
        close(clientSocket);
        return 0;
    }

    int openFD = open("student_database.txt", O_RDWR | O_CREAT | O_APPEND, 0644); // Open the file in append mode

    if (openFD == -1) {
        perror("open");
        close(clientSocket);
        return 0;
    }

    lseek(openFD, 0, SEEK_END);
    ssize_t bytes_written = write(openFD, &stud, sizeof(stud));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return 0;
    }

    close(openFD);

    // Optionally, you can send a success message to the client.
    char successMessage[1024] = "";
    sprintf(successMessage,"\n************ Student added successfully ***************\n                           Login ID is %s\n******************************************************",stud.loginId);
    send(clientSocket, successMessage, strlen(successMessage), 0);

    return 1; // Success
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void viewStudent(int clientSocket) {
    struct student my_student, temp;
    int openFD = open("student_database.txt", O_RDONLY, 0644); // Open in read-only mode

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    bool found = false; // Initialize found to false

    char buffer[1024]; // Declare buffer for sending data

    send(clientSocket, "Enter Student ID: ", strlen("Enter Student ID: "), 0);
    int readResult = read(clientSocket, my_student.loginId, sizeof(my_student.loginId) - 1);

    if (readResult <= 0) {
        send(clientSocket, "Error receiving student ID from server", strlen("Error receiving student ID from server"), 0);
        return;
    }
    my_student.loginId[readResult] = '\0';

    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    // Loop to search for the student in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(my_student.loginId, temp.loginId) == 0) { 
            found = true;
            break;
        }
    }

    if (found) {
        // Construct the details message
        sprintf(buffer, "...................Student..................\nName: %s\nAge: %s\nLogin ID: %s\nPassword: %s\n.............................................\n",
                temp.name, temp.age, temp.loginId, temp.password);

        // Send the details to the client
        send(clientSocket, buffer, strlen(buffer), 0);
    } else {
        send(clientSocket, "Student not found\n", strlen("Student not found\n"), 0);
    }

    close(openFD); // Close the file after use
}




///////////////////////////////////////////////////////////////////////////////////////////////////

void viewFaculty(int clientSocket) {
    struct faculty fac, temp;
    int openFD = open("faculty_database.txt", O_RDONLY, 0644);

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    bool found = false;

    char buffer[1024];

    send(clientSocket, "Enter Faculty ID: ", strlen("Enter Faculty ID: "), 0);
    int readResult = read(clientSocket, fac.loginId, sizeof(fac.loginId) - 1);

    if (readResult <= 0) {
        send(clientSocket, "Error receiving Faculty ID from server", strlen("Error receiving student ID from server"), 0);
        return;
    }
    fac.loginId[readResult] = '\0';

    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(fac.loginId, temp.loginId) == 0) {
            found = true;
            break;
        }
    }

    if (found) {
        // Construct the details message
        sprintf(buffer, "...................Faculty..................\nName: %s\nAge: %s\nLogin ID: %s\nPassword: %s\nDepartment: %s\n.............................................\n",
                temp.name, temp.age, temp.loginId, temp.password, temp.department);

        // Send the details to the client
        send(clientSocket, buffer, strlen(buffer), 0);
    } else {
        send(clientSocket, "Faculty not found\n", strlen("Faculty not found\n"), 0);
    }

    close(openFD);
}



///////////////////////////////////////////////////////////////////////////////////////////////////




int addFaculty(int clientSocket) {
    struct faculty facul;

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Name: ", facul.name, sizeof(facul.name)) == -1) {
        close(clientSocket);
        return 0;
    }

     sprintf(facul.loginId,"PF%d",generateFid());

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Password: ", facul.password, sizeof(facul.password)) == -1) {
        close(clientSocket);
        return 0;
    }

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Age: ", facul.age, sizeof(facul.age)) == -1) {
        close(clientSocket);
        return 0;
    }

   
    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Department: ", facul.department, sizeof(facul.department)) ==-1) {
        close(clientSocket);
        return 0;
    }
    
     if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Email Address: ", facul.emailAddress, sizeof(facul.emailAddress)) ==-1) {
        close(clientSocket);
        return 0;
    }

    int openFD = open("faculty_database.txt", O_RDWR | O_CREAT | O_APPEND, 0644); // Open the file in append mode

    if (openFD == -1) {
        perror("open");
        close(clientSocket);
        return 0;
    }

    lseek(openFD, 0, SEEK_END);
    ssize_t bytes_written = write(openFD, &facul, sizeof(facul));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return 0;
    }

    close(openFD);

    // Optionally, you can send a success message to the client.
   char successMessage[1024] = "";
    sprintf(successMessage,"\n************ Faculty added successfully ***************\n                           Login ID is %s\n******************************************************",facul.loginId);
    send(clientSocket, successMessage, strlen(successMessage), 0);
    return 1; // Success
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void modifyStudent(int clientSocket) {
    struct student my_student, temp;
    int openFD = open("student_database.txt", O_RDWR, 0644); // Open in read-only mode

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    bool found = false; // Initialize found to false

    char buffer[1024]; // Declare buffer for sending data

    send(clientSocket, "Enter Student ID: ", strlen("Enter Student ID: "), 0);
    int readResult = read(clientSocket, my_student.loginId, sizeof(my_student.loginId) - 1);

    if (readResult <= 0) {
        send(clientSocket, "Error receiving student ID from server", strlen("Error receiving student ID from server"), 0);
        return;
    }
    my_student.loginId[readResult] = '\0';

    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    // Loop to search for the student in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(my_student.loginId, temp.loginId) == 0) { // Compare the student IDs
            found = true;
            break;
        }
    }

    if (found) {
    
    sprintf(buffer, "...................Previous Student Data..................\nName: %s\nAge: %s\nLogin ID: %s\nPassword: %s\n.............................................\n",
                temp.name, temp.age, temp.loginId, temp.password);
   send(clientSocket, buffer, strlen(buffer), 0);
        // Construct the details message
    struct student stud;
	strcpy(stud.loginId,temp.loginId);
    if (sendPromptAndReceiveResponse(clientSocket, "Enter Student Name: ", stud.name, sizeof(stud.name)) == -1) {
        close(clientSocket);
        return ;
    }


    if (sendPromptAndReceiveResponse(clientSocket, "Enter Student Password: ", stud.password, sizeof(stud.password)) == -1) {
        close(clientSocket);
        return ;
    }

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Student Age: ", stud.age, sizeof(stud.age)) == -1) {
        close(clientSocket);
        return ;
    }

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Student Email Address: ", stud.emailAddress, sizeof(stud.emailAddress)) ==-1) {
        close(clientSocket);
        return ;
    }
    
    lseek(openFD, -sizeof(stud), SEEK_CUR);
    ssize_t bytes_written = write(openFD, &stud, sizeof(stud));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return ;
    }
    
   
       sprintf(buffer, "...................Updated Student Data..................\nName: %s\nAge: %s\nLogin ID: %s\nPassword: %s\n.............................................\n",
                stud.name, stud.age, stud.loginId, stud.password);
   send(clientSocket, buffer, strlen(buffer), 0);

        // Send the details to the client
        send(clientSocket, "Successfully updated\n", strlen("Successfully updated\n"), 0);
    } else {
        send(clientSocket, "Student not found\n", strlen("Student not found\n"), 0);
    }

    close(openFD); // Close the file after use
}


int generateSid(){
int fd = open("generate_sid_file.txt", O_RDWR);
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

int generateFid(){
int fd = open("generate_fid_file.txt", O_RDWR);
if (fd == -1) {
    perror("Error opening generate_sid_file.txt");}
	
	int buf;
	read(fd, &buf, sizeof(buf));
	
	buf = buf+1;
	lseek(fd, 0, SEEK_SET);
	write(fd, &buf, sizeof(buf));
	close(fd);
return buf;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void modifyFaculty(int clientSocket) {
    struct faculty fac, temp;
    int openFD = open("faculty_database.txt", O_RDWR, 0644); // Open in read-only mode

    if (openFD == -1) {
        perror("Error opening file");
        return;
    }

    bool found = false; // Initialize found to false

    char buffer[1024]; // Declare buffer for sending data

    send(clientSocket, "Enter Faculty ID: ", strlen("Enter Faculty ID: "), 0);
    int readResult = read(clientSocket, fac.loginId, sizeof(fac.loginId) - 1);

    if (readResult <= 0) {
        send(clientSocket, "Error receiving Faculty ID from server", strlen("Error receiving Faculty ID from server"), 0);
        return;
    }
    fac.loginId[readResult] = '\0';

    // Reset the file pointer to the beginning of the file
    lseek(openFD, 0, SEEK_SET);

    // Loop to search for the student in the file
    while (read(openFD, &temp, sizeof(temp)) > 0) {
        if (strcmp(fac.loginId, temp.loginId) == 0) { // Compare the student IDs
            found = true;
            break;
        }
    }

    if (found) {
    
    sprintf(buffer, "...................Previous Faculty Data..................\nName: %s\nAge: %s\nLogin ID: %s\nPassword: %s\n.............................................\n",
                temp.name, temp.age, temp.loginId, temp.password);
   send(clientSocket, buffer, strlen(buffer), 0);
        // Construct the details message
    struct faculty fac;
	strcpy(fac.loginId,temp.loginId);
    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Name: ", fac.name, sizeof(fac.name)) == -1) {
        close(clientSocket);
        return ;
    }


    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Password: ", fac.password, sizeof(fac.password)) == -1) {
        close(clientSocket);
        return ;
    }

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Age: ", fac.age, sizeof(fac.age)) == -1) {
        close(clientSocket);
        return ;
    }

    if (sendPromptAndReceiveResponse(clientSocket, "Enter Faculty Email Address: ", fac.emailAddress, sizeof(fac.emailAddress)) ==-1) {
        close(clientSocket);
        return ;
    }
    
    lseek(openFD, -sizeof(fac), SEEK_CUR);
    ssize_t bytes_written = write(openFD, &fac, sizeof(fac));

    if (bytes_written == -1) {
        perror("write");
        close(openFD);
        close(clientSocket);
        return ;
    }
    
   
       sprintf(buffer, "...................Updated Faculty Data..................\nName: %s\nAge: %s\nLogin ID: %s\nPassword: %s\n.............................................\n",
                fac.name, fac.age, fac.loginId, fac.password);
   send(clientSocket, buffer, strlen(buffer), 0);

        // Send the details to the client
        send(clientSocket, "Successfully updated\n", strlen("Successfully updated\n"), 0);
    } else {
        send(clientSocket, "Student not found\n", strlen("Student not found\n"), 0);
    }

    close(openFD); // Close the file after use
}

