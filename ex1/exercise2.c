#include "common.h"

char **stringToListChar(char buf[], char delim[])
{
    char **list = malloc(sizeof(char *) * 10);
    int i = 0;
    char *p = strtok(buf, delim);

    while (p != NULL)
    {
        list[i] = malloc(sizeof(char) * MAX_STRING_LENGTH);
        strcpy(list[i], p);
        i++;
        p = strtok(NULL, delim);
    }
    return list;
}

typedef struct Student
{
    char *id;
    char *lastName;
    char *firstName;
    float midTermScore;
    float finalTermScore;
    char *characterScore; // A,B,C,D,F
    struct Student *next;
} Student;

Student *getStudentFromInput()
{
    Student *new = malloc(sizeof(Student));
    puts("Enter student ID: ");
    new->id = getInputString();
    puts("Enter student first name: ");
    new->firstName = getInputString();
    puts("Enter student last name: ");
    new->lastName = getInputString();
    puts("Enter student midTermScore: ");
    new->midTermScore = atof(getInputString());
    puts("Enter student finalTermScore: ");
    new->finalTermScore = atof(getInputString());
    return new;
}

int sizeOf(Student *head)
{
    Student *temp;
    int size = 0;
    if (head == NULL)
    {
        return 0;
    }

    temp = head;
    while (temp != NULL)
    {
        size++;
        temp = temp->next;
    }
    return size;
}

int checkDuplicateStudent(Student *root, Student *newStud)
{
    if (root == NULL)
    {
        return 0;
    }
    Student *tmp = root;
    while (tmp->next != NULL)
    {
        if (strcmp(tmp->id, newStud->id) == 0)
        {
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

int addStudent(Student **root, Student *newStud)
{
    if (checkDuplicateStudent(*root, newStud) == 1)
    {
        puts("Duplicate, can not add student");
        return 0;
    }
    if (*root == NULL)
    {
        *root = newStud;
        return 1;
    }
    Student *tmp = *root;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = newStud;
    return 1;
}

int removeStudent(Student **root, char *id)
{
    Student *temp = *root, *prev;
    if (temp != NULL && strcmp(temp->id, id) == 0)
    {
        *root = temp->next;
        return 1;
    }

    while (temp != NULL && strcmp(temp->id, id) != 0)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        puts("No score of student's id found");
        return 0;
    }

    prev->next = temp->next;
    return 1;
}

typedef struct ScoreBoard
{
    char subjectID[MAX_STRING_LENGTH];
    char *subject;
    char semester[MAX_STRING_LENGTH];
    int studentCount;
    int midtermPercentage;
    Student *listStudent;
} ScoreBoard;

char *getFileName(char *subjectID, char *semester)
{
    char *fileName = malloc(sizeof(char));
    strcat(fileName, subjectID);
    strcat(fileName, "_");
    strcat(fileName, semester);
    strcat(fileName, ".txt");
    return fileName;
}

char *getFileNameReport(char *subjectID, char *semester)
{
    char *fileName = malloc(sizeof(char));
    strcat(fileName, subjectID);
    strcat(fileName, "_");
    strcat(fileName, semester);
    strcat(fileName, "_rp");
    strcat(fileName, ".txt");
    return fileName;
}

ScoreBoard *getScoreBoardFromFile(char *subjectID, char *semester)
{
    char *fileName = getFileName(subjectID, semester);
    FILE *f = fopen(fileName, "r");

    ScoreBoard *board = NULL;
    if (f != NULL)
    {
        board = malloc(sizeof(ScoreBoard));
        strcpy(board->subjectID, subjectID);
        strcpy(board->semester, semester);

        char *cTmp = malloc(sizeof(char));
        float fTmp;
        int iTmp;
        fgets(cTmp, MAX_STRING_LENGTH, f);
        board->subject = stringToListChar(fgets(cTmp, MAX_STRING_LENGTH, f), "|")[1];
        board->subject[strlen(board->subject) - 1] = 0;
        cTmp = stringToListChar(fgets(cTmp, MAX_STRING_LENGTH, f), "|")[1];
        board->midtermPercentage = atoi(cTmp);
        fgets(cTmp, MAX_STRING_LENGTH, f);
        cTmp = stringToListChar(fgets(cTmp, MAX_STRING_LENGTH, f), "|")[1];
        board->studentCount = atoi(cTmp);

        while (fgets(cTmp, MAX_STRING_LENGTH, f) != NULL)
        {
            Student *student = malloc(sizeof(Student));
            char **listStudent = stringToListChar(cTmp, "|");
            student->firstName = listStudent[2];
            student->lastName = listStudent[3];
            student->id = listStudent[1];
            student->midTermScore = atoi(listStudent[4]);
            student->finalTermScore = atoi(listStudent[5]);
            student->characterScore = listStudent[6];

            if (addStudent(&board->listStudent, student) == 0)
            {
                puts("Error add");
            }
        }
    }

    return board;
}

void saveScoreBoard(ScoreBoard *board)
{
    FILE *f = fopen(getFileName(board->subjectID, board->semester), "w+");
    fprintf(f, "SubjectID|%s\n", board->subjectID);
    fprintf(f, "Subject|%s\n", board->subject);
    fprintf(f, "F|%d|%d\n", board->midtermPercentage, 100 - board->midtermPercentage);
    fprintf(f, "Semester|%s\n", board->semester);
    fprintf(f, "StudentCount|%d\n", board->studentCount);
    Student *tmp = board->listStudent;

    while (tmp != NULL)
    {
        fprintf(f, "S|%s|%s|%s|%f|%f|%s|\n", tmp->id, tmp->firstName, tmp->lastName, tmp->midTermScore, tmp->finalTermScore, tmp->characterScore);
        tmp = tmp->next;
    }

    fclose(f);
}

Student* copy(Student* old) {
    Student* cop = malloc(sizeof(Student));
    cop->firstName = malloc(sizeof(char)*MAX_STRING_LENGTH);
    strcpy(cop->firstName, old->firstName);
    cop->lastName = malloc(sizeof(char)*MAX_STRING_LENGTH);
    strcpy(cop->lastName, old->lastName);
    return cop;
}

void toFileReport(ScoreBoard *board)
{
    int size = sizeOf(board->listStudent);
    if(size == 0) {
        puts("No data");
        return;    
    }

    char* fileName = getFileNameReport(board->subjectID, board->semester);
    FILE *f = fopen(fileName, "w+");
    int charScore[] = {0, 0, 0, 0, 0};
    float totalMidTerm, totalFinalTerm, maxScore = 0;
    float minScore = 10;
    // Student *highest, *lowest;
    char lowest[MAX_STRING_LENGTH];
    char highest[MAX_STRING_LENGTH];
    Student *tmp = board->listStudent;
    int midPer = board->midtermPercentage;
    while (tmp != NULL)
    {
        float curMid = tmp->midTermScore;
        float curFinal = tmp->finalTermScore;
        totalMidTerm += curMid;
        totalFinalTerm += curFinal;
        printf("SCORE: %f | %f\n", curMid, curFinal);
        float score = (curFinal * (100 - midPer) + curMid * midPer) / 100;
        if (score < 4)
        {
            charScore[4]++;
        }
        else if (score < 5.5)
        {
            charScore[3]++;
        }
        else if (score < 7.0)
        {
            charScore[2]++;
        }
        else if (score < 8.4)
        {
            charScore[1]++;
        }
        else
        {
            charScore[0]++;
        }

        if (score < minScore)
        {
            minScore = score;
            strcpy(lowest, tmp->lastName);
        }

        if (score > maxScore)
        {
            maxScore = score;
            strcpy(highest, tmp->lastName);
        }

        tmp = tmp->next;
    }

    float avgScore = (totalFinalTerm * (100 - midPer) + totalMidTerm * midPer) / (100 * size);

    fprintf(f, "The student with the highest mark is: %s\n", highest);
    fprintf(f, "The student with the lowest mark is: %s\n", lowest);
    fprintf(f, "The average mark is: %.2f\n", avgScore);
    fprintf(f, "A histogram of the subject %s is:\n", board->subject);
    for (int i = 0; i < 5; i++)
    {
        char c = 'A'+i;
        if(i==4) c = 'F';
        fprintf(f, "%c:", c);
        for (int j = 0; j < charScore[i]; j++)
        {
            fputc('*', f);
        }
        fputc('\n', f);
    }
    
    fclose(f);
    printf("You can read report in file %s\n", fileName);
}

void addNewScoreBoard()
{
    ScoreBoard *board = malloc(sizeof(ScoreBoard));
    char* cTmp;
    puts("Enter subject ID: ");
    strcpy(board->subjectID, getInputString());
    puts("Enter subject name: ");
    board->subject = getInputString();
    puts("Enter semester: ");
    strcpy(board->semester, getInputString());
    puts("Enter midterm percentage: ");
    cTmp = getInputString();
    board->midtermPercentage = atoi(cTmp);
    board->studentCount = 0;
    saveScoreBoard(board);
    puts("CREATE SUCCESS");
}

void addScore()
{
    puts("Enter subject ID: ");
    char *subjectId = getInputString();
    puts("Enter semester: ");
    char *semester = getInputString();
    ScoreBoard *board = getScoreBoardFromFile(subjectId, semester);
    if (board == NULL)
    {
        puts("Not found that file");
        return;
    }
    Student *newStudent = getStudentFromInput();
    int midPer = board->midtermPercentage;
    float score = (newStudent->midTermScore * midPer + newStudent->finalTermScore * (100 - midPer)) / 100;
    char *scoreChar = malloc(sizeof(char) * 3);
    if (score < 4)
    {
        scoreChar = "F";
    }
    else if (score < 5.5)
    {
        scoreChar = "D";
    }
    else if (score < 7.0)
    {
        scoreChar = "C";
    }
    else if (score < 8.4)
    {
        scoreChar = "B";
    }
    else
    {
        scoreChar = "A";
    }
    newStudent->characterScore = scoreChar;
    if (addStudent(&board->listStudent, newStudent) == 1)
    {
        board->studentCount++;
    }
    saveScoreBoard(board);
}

void removeScore()
{
    puts("Enter subject ID: ");
    char *subjectId = getInputString();
    puts("Enter semester: ");
    char *semester = getInputString();
    ScoreBoard *board = getScoreBoardFromFile(subjectId, semester);
    if (board == NULL)
    {
        puts("Not found that file");
        return;
    }
    puts("Enter student ID: ");
    char *id = getInputString();
    if (removeStudent(&board->listStudent, id) == 1)
    {
        board->studentCount--;
    }
    saveScoreBoard(board);
}

void search(Student *head, char *id)
{
    Student *temp;

    if (head == NULL)
    {
        printf("Not found");
        return;
    }

    temp = head;
    while (temp != NULL)
    {
        if (strcmp(id, temp->id) == 0)
        {
            printf("RESULT: %s %s: %.2f %.2f\n", temp->firstName, temp->lastName, temp->midTermScore, temp->finalTermScore);
            return;
        }
        temp = temp->next;
    }
}

void searchScore()
{
    puts("Enter subject ID: ");
    char *subjectId = getInputString();
    puts("Enter semester: ");
    char *semester = getInputString();
    ScoreBoard *board = getScoreBoardFromFile(subjectId, semester);
    if (board == NULL)
    {
        puts("Not found that file");
        return;
    }
    puts("Enter student ID: ");
    char *id = getInputString();
    search(board->listStudent, id);
}

void displayBoardAndReport()
{
    puts("Enter subject ID: ");
    char *subjectId = getInputString();
    puts("Enter semester: ");
    char *semester = getInputString();
    ScoreBoard *board = getScoreBoardFromFile(subjectId, semester);

    if (board == NULL)
    {
        puts("Not found file");
    }
    else
    {
        FILE *f = stdout;
        fprintf(f, "SubjectID|%s\n", board->subjectID);
        fprintf(f, "Subject|%s\n", board->subject);
        fprintf(f, "F|%d|%d\n", board->midtermPercentage, 100 - board->midtermPercentage);
        fprintf(f, "Semester|%s\n", board->semester);
        fprintf(f, "StudentCount|%d\n", board->studentCount);
        Student *tmp = board->listStudent;
        if (tmp != NULL)
        {
            while (tmp->next != NULL)
            {
                fprintf(f, "S|%s|%s|%s|%f|%f|%s\n", tmp->id, tmp->firstName, tmp->lastName, tmp->midTermScore, tmp->finalTermScore, tmp->characterScore);
                tmp = tmp->next;
            }
            fprintf(f, "S|%s|%s|%s|%f|%f|%s\n", tmp->id, tmp->firstName, tmp->lastName, tmp->midTermScore, tmp->finalTermScore, tmp->characterScore);
        }
    }

    toFileReport(board);
}

void showMenu()
{
    int choice = -1;
    while (1)
    {
        puts("Learning Management System");
        puts("-------------------------------------");
        puts("1. Add a new score board");
        puts("2. Add score");
        puts("3. Remove score");
        puts("4. Search score");
        puts("5. Display score board and score report");
        
        if(choice != -1) {
            puts("Continue action? y/n: ");
            char* input = getInputString();
            if(strcasecmp(input, "Y") != 0){
                puts("Your choice (1-5, other to quit):");
                scanf("%d%*c", &choice);
            }
        } else {
            puts("Your choice (1-5, other to quit):");
            scanf("%d%*c", &choice);
        }
        
        if (choice < 1 && choice > 5)
        {
            exit(0);
        }

        switch (choice)
        {
        case 1:
            addNewScoreBoard();
            break;
        case 2:
            addScore();
            break;
        case 3:
            removeScore();
            break;

        case 4:
            searchScore();
            break;

        case 5:
            displayBoardAndReport();
            break;

        default:
            exit(0);
            break;
        }
    }
}

int main(int argc, char const *argv[])
{
    showMenu();
    return 0;
}