#include "common.h"

typedef struct Student{
    
} Student;

void addNewScoreBoard(){

}


void addScore(){

}

void removeScore(){

}

void searchScore() {

}

void displayBoardAndReport() {

}

void showMenu()
{
    while (1)
    {
        int choice = -1;
        puts("Learning Management System");
        puts("-------------------------------------");
        puts("1. Add a new score board");
        puts("2. Add score");
        puts("3. Remove score");
        puts("4. Search score");
        puts("5. Display score board and score report");
        puts("Your choice (1-5, other to quit):");
        scanf("%d%*c", choice);
        if (choice < 1 && choice > 5)
        {
            exit(0);
        }

        switch (choice)
        {
        case 1:
            
            break;

        default:
        exit(0);
            break;
        }
    }
}

int main(int argc, char const *argv[])
{

    return 0;
}