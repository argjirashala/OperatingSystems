#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

void Menu();
void NewApplicant();
void ModifyApplicant();
void DeleteApplicant();
void DisplayApplicants();
void SaveWorkersRegistered();
void LoadWorkersRegistered();
void SaveApplicantsByDay();
void bus();

struct applicant{
	char name[10];
    char workdays[100];
}applicant;


int workersNeeded[7] = {10, 6, 3, 5, 2, 8, 4};
int applicantsRegistered[7] = {0, 0, 0, 0, 0, 0 ,0};


void Menu(){

    LoadWorkersRegistered();
    int option = 6;
    do {
        printf("\n0. Exit");
        printf("\n1. Add Applicant");
        printf("\n2. Modify Applicant");
        printf("\n3. Delete Applicant");
        printf("\n4. View the List of All Applicants");
        printf("\n5. Start buses");
        printf("\nChoose a menu option: ");
        scanf("%d", &option);

        switch(option){
            case 0:
                SaveWorkersRegistered();
                exit(0);
                break;
            case 1:
                NewApplicant();
                SaveApplicantsByDay();
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
            case 5:
                bus();
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
        SaveApplicantsByDay();
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
        SaveApplicantsByDay();
        return;
    }else{
        printf("\nThe Applicant with this name does not exist.");
    }
}

//-------------------------------------------------------ASSIGNMENT 2------------------------------------------------------------------------------------------------------


void SaveApplicantsByDay(){
	FILE *f_read, *f_write;
	f_read = fopen("applicants.txt", "rb");
	f_write = fopen("applicants_by_day.txt", "w");

	char *daysOfWeek[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

	for (int i = 0; i < 7; i++){
		fprintf(f_write, "%s", daysOfWeek[i]);
		fseek(f_read, 0, SEEK_SET);
		while (fread(&applicant, sizeof(applicant), 1, f_read)){
			char tempWorkdays[100];
			strcpy(tempWorkdays, applicant.workdays);
			char *day = strtok(tempWorkdays, " ");
			while (day != NULL){
				if (strcmp(day, daysOfWeek[i]) == 0){
					fprintf(f_write, " %s", applicant.name);
					break;
				}
                day = strtok(NULL, " ");
			}
		}
        fprintf(f_write, "\n");
	}
	fclose(f_read);
	fclose(f_write);
}


#define MAX_WORKERS 10
#define MAX_NAME_LENGTH 100
#define MAX_LINE_LENGTH 1024

struct message{
	long mtype;
	int num_workers;
};

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}


void safe_str_copy(char *dest, const char *src, size_t n){
	strncpy(dest, src, n - 1);
	dest[n - 1] = '\0';
}


#define MIN(a, b)((a) < (b) ? (a) : (b))

int get_day_index(char *day){
	if (strcmp(day, "Monday") == 0)
		return 0;
	if (strcmp(day, "Tuesday") == 0)
		return 1;
	if (strcmp(day, "Wednesday") == 0)
		return 2;
	if (strcmp(day, "Thursday") == 0)
		return 3;
	if (strcmp(day, "Friday") == 0)
		return 4;
	if (strcmp(day, "Saturday") == 0)
		return 5;
	if (strcmp(day, "Sunday") == 0)
		return 6;
	return -1;
}

bool validate_worker_counts(char *selected_day){
	
    FILE *file = fopen("applicants_by_day.txt", "r");
	
    if (file == NULL){
		perror("fopen");
		return false;
	}

	char line[MAX_LINE_LENGTH];
	bool valid = true;
	while (fgets(line, sizeof(line), file)){
		char *day = strtok(line, " ");
		if (strcmp(day, selected_day) != 0){
			continue;
		}

		int num_workers = 0;
		char *name = strtok(NULL, " \n");
		while (name){
			num_workers++;
			name = strtok(NULL, " \n");
		}

		if (num_workers == 0){
			printf("\nNo workers have been registered for %s\n", day);
			valid = false;
		}

		int day_index = get_day_index(day);
		if (day_index != -1 && num_workers != workersNeeded[day_index]){
			printf("\nOn %s this amount of workers is needed: %d", day, workersNeeded[day_index]);
			printf("\nYou have registered this amount: %d\n", num_workers);
			valid = false;
		}
	}

	fclose(file);
	return valid;
}


void bus(){
	char dayToProcess[10];
	printf("Enter the day you want to process (Monday, Tuesday,...): ");
	scanf("%s", dayToProcess);

	if (!validate_worker_counts(dayToProcess)){
		printf("\nAdd more applicants on this day.\n");
		return;
	}
	else{
		int pipefd[2];
		if (pipe(pipefd) == -1){
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		signal(SIGUSR1, handler);

		FILE *file = fopen("applicants_by_day.txt", "r");
		if (file == NULL){
			perror("fopen");
			exit(EXIT_FAILURE);
		}

		char line[MAX_LINE_LENGTH];
		while (fgets(line, sizeof(line), file)){
			char *day = strtok(line, " ");
			if (strcmp(day, dayToProcess) != 0){
				continue;
			}

		    char *daysOfWeek[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
            
            for (int i = 0; i < 7; i++) {
                if (strcmp(day, daysOfWeek[i]) == 0) {
                    printf("-----------------------------------------------------------\n");
                    printf("Processing day: %s\n", daysOfWeek[i]);
                    printf("Number of workers needed: %d\n", workersNeeded[i]);
                    break;
                }
            }

			char *names[MAX_WORKERS];
			int num_workers = 0;
			memset(names, 0, sizeof(names));
			char *name = strtok(NULL, " \n");
			while (name && num_workers < MAX_WORKERS){
				names[num_workers] = malloc(MAX_NAME_LENGTH);
				safe_str_copy(names[num_workers], name, MAX_NAME_LENGTH);
				num_workers++;
				name = strtok(NULL, " \n");
			}

			int workers_sent = 0;

			while (workers_sent < num_workers){
				if (pipe(pipefd) == -1){
					perror("pipe");
					exit(EXIT_FAILURE);
				}

				pid_t child = fork();
				if (child < 0){
					perror("fork");
					exit(EXIT_FAILURE);
				}

				if (child == 0){
					pause();
					printf("Child process (minibus) is ready to receive worker names.\n");
					char worker_names[MAX_WORKERS][MAX_NAME_LENGTH];

					int workers_to_receive = MIN(5, num_workers - workers_sent);
					for (int i = 0; i < workers_to_receive; i++){
						read(pipefd[0], worker_names[i], MAX_NAME_LENGTH);
						printf("Child process (minibus) received worker name: %s\n", worker_names[i]);
					}

					key_t key = ftok("applicants_by_day.txt", 1);
					int msg_queue = msgget(key, 0600 | IPC_CREAT);
					if (msg_queue < 0){
						perror("msgget");
						exit(EXIT_FAILURE);
					}

					struct message m;
					m.mtype = 1;
					m.num_workers = workers_to_receive;

					if (msgsnd(msg_queue, &m, sizeof(m), 0) < 0){
						perror("msgsnd");
						exit(EXIT_FAILURE);
					}

					printf("Child process (minibus) sent the number of workers back to the vineyard.\n");
					exit(EXIT_SUCCESS);
				}
				else{
					int workers_to_send = MIN(5, num_workers - workers_sent);
					for (int i = 0; i < workers_to_send; i++){
						write(pipefd[1], names[workers_sent + i], MAX_NAME_LENGTH);
					}

					sleep(1);
					kill(child, SIGUSR1);

					int status;
					wait(&status);

					key_t key = ftok("applicants_by_day.txt", 1);
					int msg_queue = msgget(key, 0600 | IPC_CREAT);
					if (msg_queue < 0){
						perror("msgget");
						exit(EXIT_FAILURE);
					}

					struct message m;
					if (msgrcv(msg_queue, &m, sizeof(m), 0, 0) < 0){
						perror("msgrcv");
						exit(EXIT_FAILURE);
					}

					printf("Parent process (vineyard) received the number of workers in the current minibus: %d\n", m.num_workers);

					workers_sent += workers_to_send;
				}
			}

			for (int i = 0; i < num_workers; i++){
				free(names[i]);
			}

			close(pipefd[0]);
			close(pipefd[1]);

			if (strcmp(day, dayToProcess) == 0){
				break;
			}
		}

		fclose(file);
	}
}





int main(){

    LoadWorkersRegistered();

    Menu();

    return 0;
}