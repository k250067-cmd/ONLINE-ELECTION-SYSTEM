#include <stdio.h>
#include <string.h>

#define MAXVOTERS 100
#define PROVINCES 4
#define CANDIDATES 4

const char candidate_names[PROVINCES][CANDIDATES][20] = {
    {"Ali", "Mohammad", "Farooq", "Zaid"},
    {"Fahad", "Usman", "Rayyan", "Tamim"},
    {"John", "Will", "Imran", "Jhony"},
    {"Saad", "Tariq", "Rehan", "Naveed"}
};

const char province_names[PROVINCES][20] = {"Sindh", "Punjab", "Balochistan", "KPK"};

int registration(int cnic_array[], char names[][100], int votercount){
    int cnic, age;
    char temp[100];

    printf("Enter your Name: ");
    if (scanf("%s", temp) != 1)
        return 0;

    printf("Enter your CNIC (6 digits): ");
    if (scanf("%d", &cnic) != 1){
        printf("Invalid CNIC input.\n");
        return 0;
    }

    if(cnic < 100000 || cnic > 999999){
        printf("Invalid CNIC! Must be 6 digits.\n");
        return 0;
    }

    for(int i = 0; i < votercount; i++){
        if(cnic_array[i] == cnic){
            printf("CNIC already registered.\n");
            return 0;
        }
    }

    printf("Enter your age: ");
    if (scanf("%d", &age) != 1){
        printf("Invalid age.\n");
        return 0;
    }
    if(age < 18){
        printf("You must be 18+ to vote.\n");
        return 0;
    }

    cnic_array[votercount] = cnic;
    strcpy(names[votercount], temp);
    names[votercount][99] = '\0';

    printf("Registration successful for %s.\n", names[votercount]);
    return age;
}

void voting(int votes[PROVINCES][CANDIDATES]){
    int province, candidate;
    char confirm;

    while(1) {
        printf("Select your province:\n");
        for(int i = 0; i < PROVINCES; i++)
            printf("%d: %s\n", i + 1, province_names[i]);

        printf("Enter province number: ");
        if(scanf("%d", &province) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

        if(province >= 1 && province <= PROVINCES)
            break;
        else
            printf("Invalid province number. Try again.\n");
    }

    while(1) {
        printf("Candidates in %s:\n", province_names[province-1]);
        for(int i = 0; i < CANDIDATES; i++)
            printf("%d: %s\n", i + 1, candidate_names[province-1][i]);

        printf("Enter candidate number: ");
        if(scanf("%d", &candidate) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

        if(candidate >= 1 && candidate <= CANDIDATES)
            break;
        else
            printf("Invalid candidate number. Try again.\n");
    }

    printf("Confirm vote for %s (y/n): ", candidate_names[province-1][candidate-1]);
    scanf(" %c", &confirm);

    if(confirm != 'y' && confirm != 'Y'){
        printf("Vote cancelled.\n");
        return;
    }

    votes[province-1][candidate-1]++;
    printf("Vote cast successfully!\n");
}

void list_voters(int cnic_array[], char names[][100], int votercount){
    if(votercount == 0){
        printf("No registered voters.\n");
        return;
    }

    printf("\nRegistered voters:\n");
    for(int i = 0; i < votercount; i++)
        printf("%d. %s (CNIC: %06d)\n", i+1, names[i], cnic_array[i]);
}

void showresult(int votes[PROVINCES][CANDIDATES]){
    printf("\n========= RESULTS =========\n");

    int overall_max = votes[0][0];
    int overall_p = 0;
    int overall_c = 0;

    for(int i = 0; i < PROVINCES; i++){
        printf("\n%s:\n", province_names[i]);

        int province_max = votes[i][0];
        int province_winner = 0;

        for(int j = 0; j < CANDIDATES; j++){
            printf("%s : %d votes\n", candidate_names[i][j], votes[i][j]);

            if(votes[i][j] > province_max){
                province_max = votes[i][j];
                province_winner = j;
            }

            if(votes[i][j] > overall_max){
                overall_max = votes[i][j];
                overall_p = i;
                overall_c = j;
            }
        }

        printf("Winner of %s: %s (%d votes)\n",
               province_names[i],
               candidate_names[i][province_winner],
               province_max);
    }

    printf("\nOverall winner: %s from %s with %d votes\n",
           candidate_names[overall_p][overall_c],
           province_names[overall_p],
           overall_max);
}

int main(void){
    int cnic_array[MAXVOTERS] = {0};
    char names[MAXVOTERS][100] = {{0}};

    int votercount = 0;
    int votes[PROVINCES][CANDIDATES] = {0};
    int choice;

    do{
        printf("\n===== ELECTION MENU =====\n");
        printf("1. Register & Vote\n");
        printf("2. List Registered Voters\n");
        printf("3. Show Results\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if(scanf("%d", &choice) != 1){
            while(getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }

        switch(choice){
            case 1:{
                int age = registration(cnic_array, names, votercount);
                if(age > 0){
                    votercount++;
                    while(getchar() != '\n');
                    voting(votes);
                }
                break;
            }
            case 2:
                list_voters(cnic_array, names, votercount);
                break;
            case 3:
                showresult(votes);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    }while(choice != 4);

    return 0;
}
