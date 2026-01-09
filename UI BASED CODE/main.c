#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXVOTERS 100
#define PROVINCES 4
#define CANDIDATES 4

char names[MAXVOTERS][100];
char cnic_list[MAXVOTERS][7];
int age_list[MAXVOTERS];
int votercount = 0;
int votes[PROVINCES][CANDIDATES] = {0};

const char province_names[PROVINCES][20] = {"Sindh", "Punjab", "Balochistan", "KPK"};
const char candidate_names[PROVINCES][CANDIDATES][20] = {
    {"Ali","Mohammad","Farooq","Zaid"},
    {"Fahad","Usman","Rayyan","Tamim"},
    {"John","Will","Imran","Jhony"},
    {"Saad","Tariq","Rehan","Naveed"}
};

const char *province_options = "Sindh;Punjab;Balochistan;KPK";
const char *candidate_options[PROVINCES] = {
    "Ali;Mohammad;Farooq;Zaid",
    "Fahad;Usman;Rayyan;Tamim",
    "John;Will;Imran;Jhony",
    "Saad;Tariq;Rehan;Naveed"
};

typedef enum { SCREEN_MENU, SCREEN_REGISTER, SCREEN_VOTE, SCREEN_LIST, SCREEN_RESULTS } Screen;
Screen currentScreen = SCREEN_MENU;

static char name_input[100] = "";
static char cnic_input[7] = "";
static char age_input[4] = "";
static char reg_message[200] = "";

static bool editName = false;
static bool editCNIC = false;
static bool editAge = false;

static int selectedProvince = 0;
static int selectedCandidate = 0;

static bool editProvinceMode = false;
static bool editCandidateMode = false;

static char vote_message[100] = "";

void DrawMenuScreen();
void DrawRegistrationScreen();
void DrawVotingScreen();
void DrawListScreen();
void DrawResultsScreen();
bool isDigits(const char *str);

int main(void)
{
    InitWindow(900, 600, "Election GUI");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case SCREEN_MENU: DrawMenuScreen(); break;
            case SCREEN_REGISTER: DrawRegistrationScreen(); break;
            case SCREEN_VOTE: DrawVotingScreen(); break;
            case SCREEN_LIST: DrawListScreen(); break;
            case SCREEN_RESULTS: DrawResultsScreen(); break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawMenuScreen()
{
    DrawText("ELECTION SYSTEM MENU", 250, 50, 30, BLACK);

    if (GuiButton((Rectangle){730, 10, 150, 30}, "Toggle Full Screen"))
    {
        ToggleFullscreen();
    }

    if (GuiButton((Rectangle){350,150,200,50}, "Register & Vote"))
        currentScreen = SCREEN_REGISTER;

    if (GuiButton((Rectangle){350,220,200,50}, "List Registered Voters"))
        currentScreen = SCREEN_LIST;

    if (GuiButton((Rectangle){350,290,200,50}, "Show Results"))
        currentScreen = SCREEN_RESULTS;

    if (GuiButton((Rectangle){350,360,200,50}, "Exit"))
        CloseWindow();
}

void DrawRegistrationScreen()
{
    DrawText("REGISTER NEW VOTER", 250, 20, 30, BLACK);

    DrawText("Full Name:", 100, 100, 20, BLACK);
    if (GuiTextBox((Rectangle){250,90,300,40}, name_input, 100, editName)) editName = !editName;

    DrawText("CNIC (6 digits):", 100, 170, 20, BLACK);
    if (GuiTextBox((Rectangle){250,160,150,40}, cnic_input, 7, editCNIC)) editCNIC = !editCNIC;

    DrawText("Age:", 100, 240, 20, BLACK);
    if (GuiTextBox((Rectangle){250,230,100,40}, age_input, 4, editAge)) editAge = !editAge;

    if (GuiButton((Rectangle){250,300,150,40}, "Register"))
    {
        if(strlen(name_input)==0 || strlen(cnic_input)==0 || strlen(age_input)==0)
            strcpy(reg_message,"All fields are required.");
        else if(strlen(cnic_input)!=6 || !isDigits(cnic_input))
            strcpy(reg_message,"CNIC must be 6 digits.");
        else
        {
            int duplicate = 0;
            for(int i=0;i<votercount;i++)
                if(strcmp(cnic_list[i],cnic_input)==0) duplicate=1;

            if(duplicate) strcpy(reg_message,"CNIC already registered.");
            else
            {
                int age = atoi(age_input);
                if(age<18) strcpy(reg_message,"Must be 18+.");
                else
                {
                    strcpy(names[votercount],name_input);
                    strcpy(cnic_list[votercount],cnic_input);
                    age_list[votercount] = age;
                    votercount++;

                    strcpy(reg_message,"Registration successful!");

                
                    name_input[0]=0; cnic_input[0]=0; age_input[0]=0;
                    selectedProvince = 0;
                    selectedCandidate = 0;
                    vote_message[0] = 0;
                    editName = false; editCNIC = false; editAge = false;
                    
                    currentScreen = SCREEN_VOTE;
                }
            }
        }
    }

    DrawText(reg_message,100,360,20,RED);

    if(GuiButton((Rectangle){20,20,80,40}, "< Back"))
        currentScreen = SCREEN_MENU;
}
void DrawVotingScreen()
{
    DrawText("CAST YOUR VOTE", 300, 20, 30, BLACK);


    if(GuiButton((Rectangle){250,260,150,40}, "Submit Vote"))
    {
        votes[selectedProvince][selectedCandidate]++;
        strcpy(vote_message,"Vote cast successfully!");
    }

    DrawText(vote_message,250,310,20,RED);

    if(GuiButton((Rectangle){20,20,80,40}, "< Back"))
        currentScreen = SCREEN_MENU;


    DrawText("Select Candidate: ", 80, 180, 20, BLACK);
    if (GuiDropdownBox((Rectangle){250,170,200,40}, candidate_options[selectedProvince], &selectedCandidate, editCandidateMode))
    {
        editCandidateMode = !editCandidateMode;
    }

    DrawText("Select Province:", 80, 100, 20, BLACK);
    if (GuiDropdownBox((Rectangle){250,90,200,40}, province_options, &selectedProvince, editProvinceMode))
    {
        editProvinceMode = !editProvinceMode;

        selectedCandidate = 0; 
    }
}

void DrawListScreen()
{
    DrawText("REGISTERED VOTERS", 280, 20, 30, BLACK);

    int y = 70;
    for(int i=0;i<votercount;i++)
    {
        char buffer[150];
        sprintf(buffer,"%d. %s (CNIC: %s, Age: %d)", i+1, names[i], cnic_list[i], age_list[i]);
        DrawText(buffer,50,y,20,BLACK);
        y += 30;
    }

    if(votercount==0) DrawText("No registered voters.", 50,100,20,RED);

    if(GuiButton((Rectangle){20,20,80,40}, "< Back"))
        currentScreen = SCREEN_MENU;
}

void DrawResultsScreen()
{
    DrawText("ELECTION RESULTS", 280, 20, 30, BLACK);

    int overallMax=-1, winnerProvince=-1, winnerCandidate=-1;

    int y = 70;
    for(int p=0;p<PROVINCES;p++)
    {
        DrawText(province_names[p],50,y,22,BLUE);
        y+=30;

        int provinceMax=-1, provinceWinner=-1;
        for(int c=0;c<CANDIDATES;c++)
        {
            char buffer[100];
            sprintf(buffer,"%s : %d votes", candidate_names[p][c], votes[p][c]);
            DrawText(buffer,70,y,20,BLACK);
            y+=25;

            if(votes[p][c]>provinceMax)
            {
                provinceMax=votes[p][c];
                provinceWinner=c;
            }
            if(votes[p][c]>overallMax)
            {
                overallMax=votes[p][c];
                winnerProvince=p;
                winnerCandidate=c;
            }
        }

        if(provinceWinner>=0)
        {
            char winbuf[100];
            sprintf(winbuf,"Winner: %s (%d votes)", candidate_names[p][provinceWinner], provinceMax);
            DrawText(winbuf,70,y,20,RED);
            y+=30;
        }
    }

    if(winnerCandidate>=0)
    {
        char overall[100];
        sprintf(overall,"Overall Winner: %s from %s (%d votes)", candidate_names[winnerProvince][winnerCandidate],
                province_names[winnerProvince], overallMax);
        DrawText(overall,50,y,22,DARKGREEN);
        y+=30;
    }

    if(GuiButton((Rectangle){20,20,80,40}, "< Back"))
        currentScreen = SCREEN_MENU;
}

bool isDigits(const char *str)
{
    for(int i=0; str[i]; i++)
        if(str[i]<'0' || str[i]>'9') return false;
    return true;
}
