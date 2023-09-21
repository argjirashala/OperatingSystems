#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include<stdbool.h> 

void Menu();
void NewApplicant();
void ModifyApplicant();
void DeleteApplicant();
void DisplayApplicants();
void SaveWorkersRegistered();
void LoadWorkersRegistered();

struct applicant{
	char name[10];
    char workdays[100];
}applicant;


int workersNeeded[7] = {7, 5, 8, 6, 2, 3, 4};
int applicantsRegistered[7] = {0, 0, 0, 0, 0,0,0};


void Menu(){

    LoadWorkersRegistered();
    int option = 6;
    do {
        printf("\n0. Exit");
        printf("\n1. Add Applicant");
        printf("\n2. Modify Applicant");
        printf("\n3. Delete Applicant");
        printf("\n4. View the List of All Applicants");
        printf("\nChoose a menu option: ");
        scanf("%d", &option);

        switch(option){
            case 0:
                SaveWorkersRegistered();
                exit(0);
                break;
            case 1:
                NewApplicant();
                break;
            case 2:
                ModifyApplicant();
                break;
            case 3:
                DeleteApplicant();
                break;
            case 4:
                DisplayApplicants();
                break;
            default:
                printf("Invalid option!");
            break;
        }
    } while (option != 0);
}


int IsDayAvailable(char workdays[]) {

    char tempWorkdays[100];
    strcpy(tempWorkdays, workdays);

    char *day;
    int index;
    day = strtok(tempWorkdays, " ");

    while (day != NULL) {
        if (strcmp(day, "Monday") == 0)
            index = 0;
        else if (strcmp(day, "Tuesday") == 0)
            index = 1;
        else if (strcmp(day, "Wednesday") == 0)
            index = 2;
        else if (strcmp(day, "Thursday") == 0)
            index = 3;
        else if (strcmp(day, "Friday") == 0)
            index = 4;
        else if (strcmp(day, "Saturday") == 0)
            index = 5;
        else if (strcmp(day, "Sunday") == 0)
            index = 6;
        else
            return 0; 
        if (applicantsRegistered[index] >= workersNeeded[index])
            return 0;

        day = strtok(NULL, " ");
    }
    return 1;
}


void UpdateAplicantsRegistered(char workdays[]) {
    
    char tempWorkdays[100];
    strcpy(tempWorkdays, workdays);

    char *day;
    int index;
    day = strtok(tempWorkdays, " ");

    while (day != NULL) {
        if (strcmp(day, "Monday") == 0)
            index = 0;
        else if (strcmp(day, "Tuesday") == 0)
            index = 1;
        else if (strcmp(day, "Wednesday") == 0)
            index = 2;
        else if (strcmp(day, "Thursday") == 0)
            index = 3;
        else if (strcmp(day, "Friday") == 0)
            index = 4;
        else if (strcmp(day, "Saturday") == 0)
            index = 5;
        else if (strcmp(day, "Sunday") == 0)
            index = 6;
        else
            return;

        applicantsRegistered[index]++;
        day = strtok(NULL, " ");
    }
}



void SaveWorkersRegistered() {
    FILE *f = fopen("workers.txt", "w");
    for (int i = 0; i < 7; i++) {
        fprintf(f, "%d ", applicantsRegistered[i]);
    }
    fclose(f);
}

void LoadWorkersRegistered() {
    FILE *f = fopen("workers.txt", "r");
    if (f) {
        for (int i = 0; i < 7; i++) {
            fscanf(f, "%d", &applicantsRegistered[i]);
        }
        fclose(f);
    }
}



void NewApplicant() {
    FILE *f;
    f = fopen("applicants.txt", "ab");

    printf("\nAdd New Applicant");
    printf("\n\nPlease enter the following information.\n");

    printf("Name\n: ");
    scanf("%s", &applicant.name);

    printf("The workdays are Monday Tuesday Wednesday Thursday Friday Saturday Sunday\n");
    printf("Please write the names of the days you can work on in the same format as previous line(First letter capital and separated by space)\n");
    scanf(" %[^\n]s ", &applicant.workdays);

    if (IsDayAvailable(applicant.workdays)) {
        char workdaysCopy[100];
        strcpy(workdaysCopy, applicant.workdays);
        UpdateAplicantsRegistered(applicant.workdays);

        fwrite(&applicant, sizeof(applicant), 1, f);
        printf("\nThe new Applicant has been registered successfully.");
    } else {
        printf("\nUnable to register this applicant.The selected day(s) is/are already full.");
    }
    
    fclose(f);
    
    return;
}


void DisplayApplicants(){

    printf("\nThe list of Applicants\n");
    FILE *f;
    f = fopen("applicants.txt", "rb");
    while(fread(&applicant,sizeof(applicant),1,f)){
        printf("\n%s\t",applicant.name);
        fputs(applicant.workdays, stdout);
    }
    fclose(f);
    printf("\n\t");
}


void RemoveApplicantsRegistered(char workdays[]) {
    char tempWorkdays[100];
    strcpy(tempWorkdays, workdays);

    char *day;
    int index;
    day = strtok(tempWorkdays, " ");

    while (day != NULL) {
        if (strcmp(day, "Monday") == 0)
            index = 0;
        else if (strcmp(day, "Tuesday") == 0)
            index = 1;
        else if (strcmp(day, "Wednesday") == 0)
            index = 2;
        else if (strcmp(day, "Thursday") == 0)
            index = 3;
        else if (strcmp(day, "Friday") == 0)
            index = 4;
        else if (strcmp(day, "Saturday") == 0)
            index = 5;
        else if (strcmp(day, "Sunday") == 0)
            index = 6;
        else
            return; 
        
        if (applicantsRegistered[index] > 0)
            applicantsRegistered[index]--;
        day = strtok(NULL, " ");
    }
}


void ModifyApplicant(){
    printf("\nModify an existing Applicant\n");
    char givenName[10];
    FILE *f, *f2;
    f = fopen("applicants.txt", "r");
    f2 = fopen("temp.txt","w");
    int found = 0;

    printf("Enter the name of the applicant you want to modify: ");
    scanf("%s", &givenName);
    while (fread(&applicant, sizeof(applicant), 1, f)){
        if (strcmp(applicant.name, givenName) == 0){
            found = 1;
            fflush(stdin);
            printf("New Name\n: ");
            scanf("%s", &applicant.name);

            printf("The workdays are Monday Tuesday Wednesday Thursday Friday Saturday Sunday\n");
            printf("Please write the names of the days you can work on in the same format as the previous line (First letter capital and separated by space)\n");
            char newWorkdays[100];
            scanf(" %[^\n]s ", newWorkdays);

            RemoveApplicantsRegistered(applicant.workdays);

            if (IsDayAvailable(newWorkdays)) {
                UpdateAplicantsRegistered(newWorkdays);
                strcpy(applicant.workdays, newWorkdays);
            }else {
                printf("\nUnable to modify applicant.The selected day(s) is/are already full.");
                fclose(f);
                fclose(f2);
                return;
            }
        }
        fwrite(&applicant, sizeof(applicant), 1, f2);

    }
    fclose(f);
    fclose(f2);

    if(found){
        f2 = fopen("temp.txt","r");
        f = fopen("applicants.txt","w");
        while(fread(&applicant,sizeof(applicant),1,f2)){
            fwrite(&applicant, sizeof(applicant), 1, f);
        }
        fclose(f);
        fclose(f2);
        printf("\nThe Applicant has been modified succesfully.");
        return;
    }else{
        printf("\nThe Applicant with this name does not exist.");
    }
}


void DeleteApplicant(){
    printf("\nDelete an existing Applicant\n");
    FILE *f, *f2;
    char givenName[10];
    int found = 0;
    f = fopen("applicants.txt","r");
    f2 = fopen("helper.txt","w");

    printf("Enter the name of the applicant you want to delete: ");
    scanf("%s",&givenName);
    while (fread(&applicant,sizeof(applicant),1,f)){
        if(strcmp(applicant.name,givenName) == 0){
            found = 1;
            char temp_workdays[100];
            strcpy(temp_workdays, applicant.workdays);
            RemoveApplicantsRegistered(temp_workdays);
        }else{
            fwrite(&applicant,sizeof(applicant),1,f2);
        }
    }
    fclose(f);
    fclose(f2);

    if(found){
        f2 = fopen("helper.txt","r");
        f = fopen("applicants.txt","w");
        while(fread(&applicant,sizeof(applicant),1,f2)){
            fwrite(&applicant, sizeof(applicant), 1, f);
        }
        fclose(f);
        fclose(f2);
        printf("\nThe Applicant has been deleted succesfully.");
        return;
    }else{
        printf("\nThe Applicant with this name does not exist.");
    }
}

int main(){

    LoadWorkersRegistered();

    Menu();

    return 0;
}