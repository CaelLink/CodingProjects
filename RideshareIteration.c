/*
LinkCaelIteration03.c
CS2060Project

Created by Cael Link on 4/16/24.

 Description: In this iteraton, we will be taking our current ride share system and implimenting it further. 
 To achieve this, we will be adding the ability to have multiple rideshares accsessed by a linked list.
 We will then put the contents of these different rideshares including the setup and ratings into a file named
 the rideshare for each rideshare created by the admin.
 
 *Using Xcode on macOS*
 */


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "LinkCaelIteration03.h"

#define MIN_RAND_MINUTES_FACTOR 1.2
#define MAX_RAND_MINUTES_FACTOR 1.5
#define SENTINEL_VALUE -1
//Maximum length of a string
#define  STRING_LENGTH 80
//Two dimensional array storage amounts for rows and columns of survey data
#define SURVEY_RIDER_ROWS 5
#define SURVEY_CATEGORIES 3
// login and sentinel values
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "ABCD"
#define LOGIN_MAX_ATTEMPTS 3
#define SENTINAL_NEG1 -1
#define RATINGS_MIN 1
#define RATINGS_MAX 5
#define MIN_MILES 1
#define MAX_MILES 100
#define ADMIN_MIN 0.1
#define ADMIN_MAX 50
//file path for you to change
#define FILE_PATH "/Users/caellink13/Desktop/CodingProjects/TestFolder/"


//Structure for rideshare
typedef struct {
    double riders;
    double baseFare;
    double costPerMinute;
    double costPerMile;
    double minFlatRate;
    char organizationName[STRING_LENGTH];
    int surveyCount;
    double totalMiles;
    double totalMinutes;
    double totalFares;
    struct RideShare *nextRideSharePtr;
    unsigned int survey[][SURVEY_CATEGORIES];
    
} RideShare;


//function prototypes
bool adminLogIn(const char* correctId, const char* correctPass);
void setUpaRideShare(RideShare **headPtr, RideShare *rideSharePtr, int min, int max);
void displayRideShareRatings(unsigned int *surveyCount, const int survey[][SURVEY_CATEGORIES]);
void getRideShareRatings( unsigned int *surveyCount, RideShare *head, char *rideShareName);
void ridersMode(RideShare **headPtr, RideShare *rideSharePtr);
//iter01 end
bool scanDouble(const char *buffer, double *output);
double getValidDouble(int min, int max);
double getValidDoubleSentinel(int min, int max, int sentinel);
char getYorN(void);
void fgetsRemoveNewLine(char *stringPtr);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
int calculateRandomNumber(int min, int max);
void printFare(double riders, double miles, double minutes, double fare);
void printAvgs(RideShare *RideSharePtr);
void displayRideshare(RideShare *RideSharePtr);
//iter02 end
void addRideShare(RideShare** headPtr, RideShare *newRideShare);
void fileForTotals(RideShare *head);
void displayAllRideshares(RideShare *head);
void displayAllRidesharesRM(RideShare *head);
bool getValidRideShare(RideShare *head, char *rideShareName);
void printFareTotals(RideShare *head);
void printAvgsToFile(RideShare *RideSharePtr, FILE *filePtr);
//iter03 end

int main(void) {
    RideShare rideShare;
    RideShare *rideSharePtr = &rideShare;
    
    //initialize address to head to null until things are added to the linked list
    RideShare* head = NULL;
    
    bool loggedIn = adminLogIn(CORRECT_ID, CORRECT_PASSCODE);
        //stay logged in and go trough rider mode and setup
        if(loggedIn == true){
            setUpaRideShare(&head, rideSharePtr, ADMIN_MIN, ADMIN_MAX );
            ridersMode(&head, &rideShare);
        }
}

bool adminLogIn(const char* correctId, const char* correctPass) {
    int attempts = 1;
    char id[STRING_LENGTH];
    char password[STRING_LENGTH];
    bool flag = false;
    puts("Admin Login\n");
    
    // Allow max LOGIN_MAX_ATTEMPTS to login
    while(attempts <= LOGIN_MAX_ATTEMPTS && flag == false) {
        
        //get the id and remove the new line
        puts("Enter your id: ");
        fgets(id, STRING_LENGTH, stdin);
        fgetsRemoveNewLine(id);
        
        //get the password and remove new line
        puts("Enter your passcode: ");
        fgets(password, STRING_LENGTH, stdin);
        fgetsRemoveNewLine(password);
        
        //check if entered id and password match correct id and password
        if((strcmp(id, correctId) == 0) && (strcmp(password, correctPass) == 0)) {
            puts("Login Successful");
            puts("----------------");
            //if both correct, return true and the login is successful
            flag = true;
        }
        else{
            //if incorrect, add to the attempts and relogin
            puts("Incorrect id or passcode ot id");
            attempts++;
        }
    }
    // If maximum attempts reached, return the flag and shut down.
    if (attempts > LOGIN_MAX_ATTEMPTS) {
        puts("Exiting rideshare");
    }
    return flag;
}

void setUpaRideShare(RideShare **headPtr, RideShare *rideSharePtr, int min, int max) {
   
    //first initialize values in the structure to 0
    rideSharePtr->riders = 0;
    rideSharePtr->surveyCount = 0;
    rideSharePtr->totalMiles = 0;
    rideSharePtr->totalMinutes = 0;
    rideSharePtr->totalFares = 0;
    
    //set a bool to false while the admin wants to add rideshares
    bool setUp = false;
    
    do {
        
        puts("Setup rideshare info");
        puts("--------------------");
        //get the basefare
        puts("Enter the base fare: ");
        rideSharePtr->baseFare = getValidDouble(min, max);
        
        // Get cost per minute
        puts("Enter the cost per minute: ");
        rideSharePtr->costPerMinute = getValidDouble(min, max);
        
        // Get cost per mile
        puts("Enter the cost per mile: ");
        rideSharePtr->costPerMile = getValidDouble(min, max);
        
        // Get minimum flat rate
        puts("Enter the minimum flat rate: ");
        rideSharePtr->minFlatRate = getValidDouble(min, max);
        
        // Get the RideShare Organization name
        puts("Enter the RideShare Organization name: ");
        fgets(rideSharePtr->organizationName, STRING_LENGTH, stdin);
        fgetsRemoveNewLine(rideSharePtr->organizationName);
        
        //call the addRideShare function which will add the rideshare info to a linked list alphabetically allocated on the heap
        addRideShare(headPtr, rideSharePtr);
        
        //prompt to setup another rideshare
        puts("Do you want to setup another Rideshare?");
        char response = getYorN();
        
        if(response == 'n') {
            //if no,break out of the do, so all of the rideshares are set and go on to display
            setUp = true;
        }
   
    } while(setUp == false);
    
    puts("Rideshare details: ");
    //call to display all of the rideshares in the linked list
    displayAllRideshares(*headPtr);
    
    //after all rideshares display, exit the admin mode and go onto rider mode
    puts("\nExiting Admin Mode\n");
    
    
}

void displayRideShareRatings(unsigned int *surveyCount, const int survey[][SURVEY_CATEGORIES]) {
    puts("Survey Results:\n");
    if (*surveyCount == 0) {
        puts("There are no ratings\n");
    }
    else {
        puts("Rating Categories:\t1. Safety\t2. Cleanliness\t3. Comfort\n");
        
        // Loop through surveys and print ratings
        for (size_t i = 0; i < *surveyCount; i++) {
            printf("Survey %zu:\t", i + 1);
            for (size_t j = 0; j < SURVEY_CATEGORIES; j++) {
                printf("%d\t\t\t", (survey[i][j]));
            }
            puts("");
        }
        puts("");
    }
}

void getRideShareRatings(unsigned int *surveyCount, RideShare *head, char *rideShareName) {
    RideShare *current = head;
    
    while(current != NULL && strcmp(current->organizationName, rideShareName) != 0) {
        current = current->nextRideSharePtr;
    }
    if (current != NULL) {
        //go throuh categories and add rating to each
        for (size_t j = 0; j < SURVEY_CATEGORIES; j++) {
            printf("Enter the ratings for Category %zu (1 - Safety, 2 - Cleanliness, 3 - Comfort):\n", j + 1);
            int input = 0;
            input = getValidDouble(RATINGS_MIN, RATINGS_MAX);
            current->survey[*surveyCount][j] = input;
            
        }
        //add to survey count
        (*surveyCount)++;
    }
}

void ridersMode(RideShare **headPtr, RideShare *rideSharePtr) {
    
    //set status to false to run the whole rider mode
    bool loginStatus = false;

    do {
        puts("Rider Mode");
        puts("----------");
       
        //call display all rideshares in rider mode
        displayAllRidesharesRM(*headPtr);

        //get the valid ridshare the user would like to use
        puts("Enter the name of the rideshare you would like to ride with above.");
        char rideShareName[STRING_LENGTH];
        fgets(rideShareName, STRING_LENGTH, stdin);
        //remove new line in input
        fgetsRemoveNewLine(rideShareName);
        
        //get true or false for a valid rideshare
        bool response = getValidRideShare(*headPtr, rideShareName);
        
        //if vaild ridshare go on
        if(response == true) {
            printf("Welcome to %s. We can only provide services for rides from %d to %d miles. Enter the number of miles you would like to ride.\n", rideShareName, MIN_MILES, MAX_MILES);
            double miles = getValidDoubleSentinel(MIN_MILES,MAX_MILES ,SENTINAL_NEG1 );
            
            //if -1 then prompt login again
            if(miles == SENTINAL_NEG1) {
                bool loginStatus1 = adminLogIn(CORRECT_ID, CORRECT_PASSCODE);
                if(loginStatus1 == true) {
                    // Print totals for all ride share
                    RideShare *current = *headPtr;
                    //iterate through the LL to display totals for each organization
                    while (current != NULL && strcmp(current->organizationName, rideShareName) != 0) {
                        current = current->nextRideSharePtr;
                    }
                    printFareTotals(*headPtr);
                    puts("");
                    fileForTotals(*headPtr);
                    //break out of do while ending the program
                    loginStatus = true;
                }
            }
            //go on through rideshare if not sentinel entered
            else {
               
                double minMinutes = miles * MIN_RAND_MINUTES_FACTOR;
                double maxMinutes = miles * MAX_RAND_MINUTES_FACTOR;
                //find the minutes
                int minutes = calculateRandomNumber(minMinutes, maxMinutes);
                printf("Duration of ride: %d minutes\n", minutes);
                 
                //once again, iterate through the LL for each org. to add the totals and find the fare for the 'current' organization
                RideShare *current = *headPtr;
                while (current != NULL && strcmp(current->organizationName, rideShareName) != 0) {
                    current = current->nextRideSharePtr;
                }
                if(current != NULL) {
                    double fare = calculateFare(current->baseFare, current->costPerMinute, current->costPerMile, current->minFlatRate, miles, minutes);
                    
                    current->totalMiles += miles;
                    current->totalMinutes += minutes;
                    current->totalFares += fare;
                    current->riders++;
                    
                    // Print fare for the current ride share
                    puts("Current ride info:\n");
                    printFare(current->riders, miles, minutes, fare);
                }
                
                //after fare is printed, ask to rate
                puts("We would like to know how your ride went with us. Do you want to rate your rideshare experience?");
                char ratingResponse = getYorN();
                if (ratingResponse == 'y') {
                    //set the current again to the org name the user is riding with to properly set up ratings for that individual organization
                    RideShare *current = *headPtr;
                    while (current != NULL && strcmp(current->organizationName, rideShareName) != 0) {
                        current = current->nextRideSharePtr;
                    }
                    if (current != NULL) {
                        // Call the function to get ratings for the selected ride share
                        getRideShareRatings(&(current->surveyCount), *headPtr, rideShareName);
                    }
                }
                else {
                    puts("\nThanks for riding");
                }
            }
        }
        
    } while (loginStatus == false);
    
    puts("\nThanks for riding");
}

bool scanDouble(const char* buffer, double* output) {
    char* end;
    errno = 0;
    double testNum = strtod(buffer, &end);
    bool isValid = false;
    //Checking if nothing at all was read
    if (end == buffer){
        puts("Nothing was entered");
    }
    //Checking if the string was too long and didn't have a null character
    else if (*end != '\0') {
        puts("Invalid sring");
    }
    //Checking if the input was out of bounds
    else if (ERANGE == errno) {
        puts("Out of bounds");
    }
    //vaild input
    else {
        *output = testNum;
        isValid = true;
    }
    return isValid;
}


double getValidDouble(int min, int max) {
    bool flag = false;
    double validDouble = 0;
    char input[STRING_LENGTH];
    
    
    while(flag == false) {
        //get input
        fgets(input, STRING_LENGTH, stdin);
        //remone new line charag=cter
        fgetsRemoveNewLine(input);
        
        //check if within bounds
        if((scanDouble(input, &validDouble)) && (validDouble >= min) && (validDouble <= max)) {
            flag = true;
        }
        //error if not within bounds
        else {
            printf("Not within %d and %d. Try again", min, max);
        }
    }
    return validDouble;
}

//function very similar to getValidDouble, just adding a parameter if the sentinel value is inputted
double getValidDoubleSentinel(int min, int max, int sentinel) {
    bool flag = false;
    double validDouble = 0;
    char input[STRING_LENGTH];
    
    
    while(flag == false) {
        fgets(input, STRING_LENGTH, stdin);
        fgetsRemoveNewLine(input);
        

        if(((scanDouble(input, &validDouble)) && (validDouble >= min) && (validDouble <= max)) || (validDouble == SENTINEL_VALUE)) {
            flag = true;
        }
        else {
            printf("Not within %d and %d. Try again", min, max);
        }
    }
    return validDouble;
    
}
    

char getYorN(void) {
    char input[2];
    fgets(input, STRING_LENGTH, stdin);
    fgetsRemoveNewLine(input);

    if (strcmp(input, "y") == 0) {
        return 'y';
        // Return 'y' for yes
    }
    else if (strcmp(input, "n") == 0) {
        return 'n';
        // Return 'n' for no
    }
    else {
        printf("Invalid input. Please enter 'y' or 'n'.\n");
        return getYorN();
        // Retry until valid input is received
    }
}

void fgetsRemoveNewLine(char *stringPtr) {
    
    //iterate thorugh the sting until a \n or \0 is found
    while(*stringPtr != '\n' && *stringPtr != '\0') {
        stringPtr++;
    }
    //replace with the proper null character
    if(*stringPtr == '\n') {
        *stringPtr = '\0';
    }
}

double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes) {
    //formula
    double fare = base + (minuteCost * minutes) + (mileCost * miles);
    
    //account for minrate value
    if(fare <= minRate) {
        fare = minRate;
    }
    return fare;
}

//random number generator function
int calculateRandomNumber(int min, int max) {
    int randomNum = rand() % (max-min+1) +min;
    return randomNum;
}


void printFare(double riders, double miles, double minutes, double fare) {

   //check for no rides
    if(riders == 0){
        
        puts("There were no riders.");
    }
    //print the fare for the rider
    else {
        puts("Rider\tNumber of Miles\tNumber of Minutes\tRide Fare Amount");
        printf("%.2lf\t%.1lf\t\t\t%.2lf\t\t\t\t$%.2lf\n\n", riders, miles, minutes, fare);
    }
}

void printAvgs(RideShare *RideSharePtr) {
    if (RideSharePtr->surveyCount == 0) {
        puts("There are no averages to display\n");
    }
    else {
        //make a new array for averages
        double averages[SURVEY_CATEGORIES];

        for (int i = 0; i < SURVEY_CATEGORIES; i++) {
            double sum = 0;
            for (int j = 0; j < RideSharePtr->surveyCount; j++) {
                sum += RideSharePtr->survey[j][i];
            }
            //calculate the average and fill the array
            averages[i] = sum / RideSharePtr->surveyCount;
        }
        //print each average for each category
        puts("Averages:");
        puts("Rating Categories:\t1. Safety\t2. Cleanliness\t3. Comfort\n");
        for (int i = 0; i < SURVEY_CATEGORIES; i++) {
            printf("%d:\t%.1lf\t", i + 1, averages[i]);
        }
        puts("");
    }
}

//display details of rideshare for setup
void displayRideshare(RideShare *rideSharePtr) {
    
    printf("%s\n", rideSharePtr-> organizationName);
    puts("-------------");
    printf("Base Fare: $%.2lf\n", rideSharePtr-> baseFare);
    printf("Cost Per Minute: $%.2lf\n", rideSharePtr-> costPerMinute);
    printf("Cost Per Mile: $%.2lf\n", rideSharePtr-> costPerMile);
    printf("Minimum Flat Rate: $%.2lf\n\n", rideSharePtr-> minFlatRate);
}
 
//create a linked list on the heap for each rideshare added
void addRideShare(RideShare** headPtr, RideShare *newRideShare) {
    //allocate memory for an address of a rideshare structure on the heap
    RideShare* newNodePtr = malloc(sizeof(RideShare));

    if (newNodePtr != NULL) {
        //check for memory allocation then go on to make linked list
        *newNodePtr = *newRideShare;
        newNodePtr->nextRideSharePtr = NULL;
        
        //allocte memory for an address for a current and head to work with LL
        RideShare *currentPtr = *headPtr;
        RideShare *previousPtr = NULL;
        
        //iterate through and find the place for the new node to go alphabetically
        while (currentPtr != NULL && strcmp(currentPtr->organizationName, newRideShare->organizationName) < 0) {
            previousPtr = currentPtr;
            currentPtr = currentPtr->nextRideSharePtr;
        }
        if (previousPtr == NULL) {
            //insert at the beginning if nothing else is there
            *headPtr = newNodePtr;
        }
        else {
            //set the prevous to the new node in the correct place
            previousPtr->nextRideSharePtr = newNodePtr;
        }
        //iterate through LL till the end
        newNodePtr->nextRideSharePtr = currentPtr;
    }
    else {
        puts("No memory to create node for rideshare");
    }
}


void fileForTotals(RideShare *head) {
    FILE *cfPtr = NULL;
    RideShare *current = head;
    while (current != NULL) {
        
        char rideShare[STRING_LENGTH] = FILE_PATH;
        char *rideShareName = current->organizationName;
        //C library- uses the rideshare name to make a new file in the folder
        strcat(rideShare, rideShareName);
        
        //check if path is good
        if((cfPtr = fopen(rideShare, "w")) == NULL) {
            puts("File could not be opened");
        }
        //write details of totals and avgs function to each file
        else {
            fprintf(cfPtr, "Totals for %s ride share:\n", current->organizationName);
            fprintf(cfPtr, "Riders\tMiles\tMinutes\tFares\n");
            fprintf(cfPtr, "%.2lf\t%.1lf\t%.2lf\t$%.2lf\n\n", current->riders, current->totalMiles, current->totalMinutes, current->totalFares);
            printAvgsToFile(current, cfPtr);
            //close the file
            fclose(cfPtr);
        }
        //move on to next organization if there and repeat
        current = current->nextRideSharePtr;
    }
}

// calling the display rideshre it iterate through LL and display details of each rideshare in setup mode
void displayAllRideshares(RideShare *head) {
    RideShare *current = head;
    
    while (current != NULL) {
        displayRideshare(current);
        current = current->nextRideSharePtr;
    }
}

//this function displays all rideshare organizations for the rider mode specifically being the averages for each rideshare in the linked list
void displayAllRidesharesRM(RideShare *head) {
    RideShare *current = head;
    
    while (current != NULL) {
        printf("%s\n", current-> organizationName);
        puts("---------------");
        displayRideShareRatings(&(current->surveyCount), current->survey);
        puts("");
        current = current->nextRideSharePtr;
    }
}

//checks if the organization name entered matches any of the current organizations in a linked list and return t or f if found
bool getValidRideShare(RideShare *head, char *rideShareName) {
    RideShare *current = head;
    
    while (current != NULL) {
        if (strcmp(current->organizationName, rideShareName) == 0) {
            return true;
        }
        current = current->nextRideSharePtr;
    }
    printf("Rideshare %s is not listed above.\n", rideShareName);
    return false;
}

//totals function to grab the totals and avgs from before and display them neatly iterating through the linked list for each rideshare
void printFareTotals(RideShare *head) {
    RideShare *current = head;
    while (current != NULL) {
        printf("\n%s Summary Report:\n\n", current->organizationName);
        puts("Totals:");
        printFare(current->riders, current->totalMiles, current->totalMinutes, current->totalFares);

        printAvgs(current);
        puts("-----------------------------------------------------------");
        current = current->nextRideSharePtr;
    }
}

//Very similar function to display avgs however, all print statements are now fprinf to write to a file
void printAvgsToFile(RideShare *RideSharePtr, FILE *filePtr) {
    if (RideSharePtr->surveyCount == 0) {
        fprintf(filePtr, "There are no averages to display\n");
    }
    else {
        //make a new array for averages
        double averages[SURVEY_CATEGORIES];

        for (int i = 0; i < SURVEY_CATEGORIES; i++) {
            double sum = 0;
            for (int j = 0; j < RideSharePtr->surveyCount; j++) {
                sum += RideSharePtr->survey[j][i];
            }
            //calculate the average and fill the array
            averages[i] = sum / RideSharePtr->surveyCount;
        }
        //print each average for each category
        fprintf(filePtr, "\n");
        fprintf(filePtr, "1. Safety\t2. Cleanliness\t3. Comfort\n");
        for (int i = 0; i < SURVEY_CATEGORIES; i++) {
            fprintf(filePtr, "%d:\t%.1lf\t", i + 1, averages[i]);
        }
        fprintf(filePtr, "");
    }
}


