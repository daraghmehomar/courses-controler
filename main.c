#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//1200162 omar daraghmeh sec3
typedef struct AVLnode *AVLNode;
AVLNode MakeEmpty( AVLNode T );
AVLNode FindMin( AVLNode T );
int Height( AVLNode P );
AVLNode Find( char X[], AVLNode T );
int Max( int Lhs, int Rhs );
int getBalance( AVLNode T);
struct AVLnode
{
    char course[30];
    int CH;
    char code[10];
    char topics[99];
    char department[20];
    AVLNode Left;
    AVLNode Right;
    int Height; //Balance information
};
AVLNode MakeEmpty( AVLNode T )
{
    if( T != NULL )
    {
        MakeEmpty( T->Left );
        MakeEmpty( T->Right );
        free( T );
    }
    return NULL;
}
AVLNode FindMin( AVLNode T )
{
    if( T == NULL )
        return NULL;
    else if( T->Left == NULL )
        return T;
    else
        return FindMin( T->Left );
}
AVLNode Find( char X[], AVLNode T )
{
    if( T == NULL )
        return NULL;
    if( strcmp(X , T->code )<0)
        return Find( X, T->Left );
    else if( strcmp(X , T->code )>0)
        return Find( X, T->Right );
    else
        return T;
}
int Height( AVLNode P )
{
    if( P == NULL )
        return -1;
    else
        return P->Height;
}
int Max( int Lhs, int Rhs )
{
    return Lhs > Rhs ? Lhs : Rhs;
}
int getBalance( AVLNode T){
    if (T==NULL)
        return 0;
    return Height(T->Left)- Height(T->Right);
}
AVLNode SingleRotateWithLeft( AVLNode K2 )
{
    AVLNode  K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
    K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;

    return K1;  /* New root */
}


/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */
/* Update heights, then return new root */

AVLNode SingleRotateWithRight( AVLNode K1 )
{
    AVLNode K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
    K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;

    return K2;
}
/* This function can be called only if K3 has a left */
/* child and K3's left child has a right child */
/* Do the left-right double rotation */
/* Update heights, then return new root */

AVLNode DoubleRotateWithLeft( AVLNode K3 )
{
    /* Rotate between K1 and K2 */
    K3->Left = SingleRotateWithRight( K3->Left );

    /* Rotate between K3 and K2 */
    return SingleRotateWithLeft( K3 );
}
/* This function can be called only if K1 has a right */
/* child and K1's right child has a left child */
/* Do the right-left double rotation */
/* Update heights, then return new root */

AVLNode DoubleRotateWithRight( AVLNode K1 )
{
    /* Rotate between K3 and K2 */
    K1->Right = SingleRotateWithLeft( K1->Right );

    /* Rotate between K1 and K2 */
    return SingleRotateWithRight( K1 );
}

struct AVLnode* deleteNode(struct AVLnode* root, char key[])
{
    if (root == NULL)
        return root;
    if (strcmp(root->code,key)>0)
        root->Left = deleteNode(root->Left, key);

    else if(strcmp(root->code,key)<0)
        root->Right = deleteNode(root->Right, key);

    else
    {
        if( (root->Left == NULL) || (root->Right == NULL) )
        {
            struct AVLnode *temp = root->Left ? root->Left :
                                root->Right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else
        {
            struct AVLnode* temp = FindMin(root->Right);

            strcpy(root->code,temp->code);
            strcpy(root->course,temp->course);
            strcpy(root->topics,temp->topics);
            strcpy(root->department,temp->department);
            root->CH=temp->CH;

            root->Right = deleteNode(root->Right, temp->code);
        }
    }
    if (root == NULL)
        return root;

    root->Height = 1 + Max(Height(root->Left),
                           Height(root->Right));


    int balance = getBalance(root);

    // If  node become unbalanced there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->Left) >= 0)
        return SingleRotateWithRight(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->Left) < 0)
    {
        root->Left =  SingleRotateWithLeft(root->Left);
        return SingleRotateWithRight(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->Right) <= 0)
        return SingleRotateWithLeft(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->Right) > 0)
    {
        root->Right = SingleRotateWithRight(root->Right);
        return SingleRotateWithLeft(root);
    }

    return root;
}
struct AVLnode* Letter(struct AVLnode* root, char key){
    if(root==NULL)
        return NULL;
    if(root->Right!=NULL)
        root->Right= Letter(root->Right,key);
    if(root->Left!=NULL)
        root->Left= Letter(root->Left,key);
    if (root->course[0]==key) {
        printf("%s deleted\n",root->code);
        root = deleteNode(root, root->code);
    }
    return root;
}
struct AVLnode* dep(struct AVLnode* root,char key[]){
    if(root==NULL)
        return NULL;
    if(root->Right!=NULL)
        root->Right= dep(root->Right,key);
    if(root->Left!=NULL)
        root->Left= dep(root->Left,key);
    if (strcmp(key,root->department)==0) {
        printf("%s deleted\n",root->code);
        root = deleteNode(root, root->code);
    }
    return root;
}
void PrintByDep(struct AVLnode* root,char key[]){
    if(root==NULL)
        return;
    if(root->Left!=NULL)
        PrintByDep(root->Left,key);
    if(root->Right!=NULL)
        PrintByDep(root->Right,key);
    if (strcmp(key,root->department)==0) {
        printf("%-10s|%-25s|credit hours:%d|%s\n",root->code,root->course,root->CH,root->topics);
    }
}
AVLNode Insert( char course[],int CH,char code[],char department[],char topics[], AVLNode T )
{
    if( T == NULL )
    {
        T = malloc( sizeof( struct AVLnode ) );
        if( T == NULL )
            printf( "Out of space!!!" );
        else
        {
            strcpy(T->course,course);
            strcpy(T->code,code);
            strcpy(T->department,department);
            strcpy(T->topics,topics);
            T->CH=CH;
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    }
    else if( strcmp(code , T->code )<0)
    {
        T->Left = Insert( course, CH,code,department,topics,T->Left );
        if( Height( T->Left ) - Height( T->Right ) == 2 )
            if( strcmp(code , T->Left->code )<0)
                T = SingleRotateWithLeft( T );
            else
                T = DoubleRotateWithLeft( T );
    }
    else if( strcmp(code , T->code )>0)
    {
        T->Right = Insert( course,CH,code,department,topics, T->Right );
        if( Height( T->Right ) - Height( T->Left ) == 2 )
            if( strcmp(code , T->Right->code )>0)
                T = SingleRotateWithRight( T );
            else
                T = DoubleRotateWithRight( T );
    }
    /* Else X is in the tree already; we'll do nothing */

    T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1;
    return T;
}
AVLNode Insertbycourse( char course[],int CH,char code[],char department[],char topics[], AVLNode T )
{
    if( T == NULL )
    {
        T = malloc( sizeof( struct AVLnode ) );
        if( T == NULL )
            printf( "Out of space!!!" );
        else
        {
            strcpy(T->course,course);
            strcpy(T->code,code);
            strcpy(T->department,department);
            strcpy(T->topics,topics);
            T->CH=CH;
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
    }
    else if( strcmp(course , T->course )<0)
    {
        T->Left = Insertbycourse( course, CH,code,department,topics,T->Left );
        if( Height( T->Left ) - Height( T->Right ) == 2 )
            if( strcmp(course , T->Left->course )<0)
                T = SingleRotateWithLeft( T );
            else
                T = DoubleRotateWithLeft( T );
    }
    else if( strcmp(course , T->course )>0)
    {
        T->Right = Insertbycourse( course,CH,code,department,topics, T->Right );
        if( Height( T->Right ) - Height( T->Left ) == 2 )
            if( strcmp(course , T->Right->course )>0)
                T = SingleRotateWithRight( T );
            else
                T = DoubleRotateWithRight( T );
    }
    /* Else X is in the tree already; we'll do nothing */

    T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1;
    return T;
}
AVLNode TreeByCourse(AVLNode root,AVLNode new){
    if (root==NULL)
        return NULL;
    new= Insertbycourse(root->course,root->CH,root->code,root->department,root->topics,new);
    if(root->Right!=NULL)
        new= TreeByCourse(root->Right,new);
    if(root->Left!=NULL)
        new= TreeByCourse(root->Left,new);
    return new;
}

//in-order print to see the elements
void PrintInOrder( AVLNode t)
{
    if( t != NULL)
    {
        PrintInOrder( t->Left );
        printf("||%-10s| \t", t->code);
        printf("%-30s| \tcredit hours %d|| \n", t->course,t->CH);
        PrintInOrder( t->Right );
    }
}
void printInFile(FILE *out,struct AVLnode* T){
    if (T==NULL)
        return;
    printInFile(out,T->Left);
    fprintf(out,"%s:%d#%s#%s/%s\n",T->course,T->CH,T->code,T->department,T->topics);
    printInFile(out,T->Right);
}
int main() {
    FILE *infile = fopen("courses.txt", "r");
    FILE *outfile = fopen("offered_courses.txt", "w");
    struct AVLnode *T;
    T = MakeEmpty(NULL);
    if (infile == NULL) {
        printf("file was not found!");
    } else {
        char course[30];
        int CH;
        char code[10];
        char topics[99];
        char department[20];
        char line[100];
        while (fgets(line, 200, infile) != NULL) {
            char *token;
            strcpy(course, strtok(line, ":"));
            token = strtok(NULL, "#");
            CH = atoi(token);
            strcpy(code, strtok(NULL, "#"));
            strcpy(department, strtok(NULL, "/"));
            strcpy(topics, strtok(NULL, "\n"));
            T=Insert(course, CH, code,department, topics,T);
        }
        while (1){
            printf("1) Add a new course\n"
                   "2) Update a course info\n"
                   "3) Print topics for spacific course\n"
                   "4) Print all the courses\n"
                   "5) Print in lexicographic order\n"
                   "6) Print in lexicographic order in a department\n"
                   "7) Delete a course \n"
                   "8) Delete all courses that start with a specific letter\n"
                   "9) Delete all courses belong to a given department\n"
                   "else) Save and Exit\n");
            int x;
            scanf("%d",&x);
            switch (x) {
                case 1: {
                    char course[30];
                    int CH;
                    char code[10];
                    char topics[99];
                    char department[20];
                    printf("\nthe course:\n");
                    scanf(" %s",course);
                    printf("\nthe code:\n");
                    scanf(" %s",code);
                    printf("\nthe credit hours:\n");
                    scanf(" %d",&CH);
                    printf("\nthe department:\n");
                    scanf(" %s",department);
                    printf("\nthe topics(separeted by (,) topic1, topic2,.....):\n");
                    scanf(" %s",topics);
                    T=Insert(course, CH, code,department, topics,T);
                    break;
                }
                case 2: {
                    char course[30];
                    int CH;
                    char code[10];
                    char topics[99];
                    char department[20];
                    printf("\nenter the course code to update:");
                    scanf("%s", code);
                    struct AVLnode *temp = Find(code, T);
                    if (temp == NULL)
                        printf("cant find!!\n");
                    else {
                        printf("\nenter the new course name:");
                        scanf("%s", course);
                        strcpy(temp->course, course);
                        printf("\nenter the new department name:");
                        scanf("%s", department);
                        strcpy(temp->department, department);
                        printf("\nenter the new credit hours:");
                        scanf("%d", &CH);
                        temp->CH = CH;
                        printf("\nenter the topics(separeted by (,) topic1, topic2,.....):");
                        scanf("%s", topics);
                        strcpy(temp->topics, topics);
                    }
                    printf("Press ENTER Key to Continue...\n");
                    getchar();
                    getchar();
                    break;
                }
                    case 3:{
                        char code[10];
                        printf("enter the code:\n");
                        scanf("%s",code);
                        struct AVLnode *temp = Find(code, T);
                        if(temp!=NULL)
                            printf("%s\n",temp->topics);
                        else
                            printf("cant find!!!!");
                        printf("Press ENTER Key to Continue...\n");
                        getchar();
                        getchar();
                        break;
                    }
                case 4:{

                    PrintInOrder(T);
                   break;
                }
                case 5:{
                    AVLNode new= MakeEmpty(NULL);
                    new= TreeByCourse(T,new);
                    PrintInOrder(new);

                    break;
                }
                case 6:{
                    AVLNode new= MakeEmpty(NULL);
                    new= TreeByCourse(T,new);
                    char dep[20];
                    printf("enter the department name:");
                    scanf("%s",dep);
                    PrintByDep(new,dep);
                    printf("Press ENTER Key to Continue...\n");
                    getchar();
                    getchar();
                    break;
                }
                case 7:{
                    char code[10];
                    printf("enter the code:\n");
                    scanf("%s",code);
                    T = deleteNode(T, code);
                    printf("Press ENTER Key to Continue...\n");
                    getchar();
                    getchar();
                    break;
                }
                case 8:{
                    char key;
                    printf("enter the letter:\n");
                    scanf(" %c",&key);
                    T=Letter(T,key);
                    printf("Press ENTER Key to Continue...\n");
                    getchar();
                    getchar();
                    break;
                }
                case 9:{
                    char key[20];
                    printf("enter the department:\n");
                    scanf(" %s",key);
                    T=dep(T,key);
                    printf("Press ENTER Key to Continue...\n");
                    getchar();
                    getchar();
                    break;
                }

                case 0:{
                    printInFile(outfile,T);
                    fclose(outfile);
                    return 0;
                }
                default:{
                    printInFile(outfile,T);
                    fclose(outfile);
                    return 0;
                }

                }
            }
        }

    return 0;
}