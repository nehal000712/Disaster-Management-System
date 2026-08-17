#include <stdio.h>

#include <stdlib.h>

#include <string.h>


#define NAME_SIZE 50
#define NEED_SIZE 100

/* =============================
           STRUCTURES
   ============================= */


 /* Linked List for affected people */

typedef struct Person {

    int id;

    char name[NAME_SIZE];

    char location[NAME_SIZE];

    int severity;

    struct Person *next;

} Person;

 /* Linked List for resources */

typedef struct Resource {

    int id;

    char name[NAME_SIZE];

    char location[NAME_SIZE];

    int quantity;

    struct Resource *next;

} Resource;

 /* Rescue request data */

typedef struct Request {

    int id;

    char personName[NAME_SIZE];

    char location[NAME_SIZE];

    char need[NEED_SIZE];

    int severity;

} Request;

 /* Queue node for rescue requests */

typedef struct QueueNode {

    Request data;

    struct QueueNode *next;

} QueueNode;

 /* Queue structure */

typedef struct Queue {

    QueueNode *front;

    QueueNode *rear;

} Queue;

/* Stack node for rollback dispatch */
typedef struct StackNode {

    int dispatchId;

    Request data;

    struct StackNode *next;

} StackNode;

/* Binary Search Tree for location summary */
typedef struct LocationNode {

    char location[NAME_SIZE];

    int totalAffected;

    int totalResources;

    struct LocationNode *left;

    struct LocationNode *right;

} LocationNode;


/* =============================
   BASIC INPUT FUNCTIONS
   ============================= */
void clearInputBuffer() {

    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF);

}

 void inputText(char message[], char text[], int size) {

    printf("%s", message);

    fgets(text, size, stdin);

    text[strcspn(text, "\n")] = '\0';

}
int inputNumber(char message[]) {

    int number;
    printf("%s", message);

    while (scanf("%d", &number) != 1) {

        clearInputBuffer();

        printf("Invalid input. Enter a number: ");
      }

    clearInputBuffer();

    return number;

}
int inputSeverity() {

    int severity;

    do {
       severity = inputNumber("Enter severity level (1-10): ");
       if (severity < 1 || severity > 10) {
        printf("Severity must be between 1 and 10.\n");
        }
       } while (severity < 1 || severity > 10);

     return severity;
}

/*=============================
  TREE FUNCTIONS
============================= */
LocationNode* createLocationNode(char location[], int affected, int resources) {

    LocationNode *newNode = (LocationNode*) malloc(sizeof(LocationNode));

     if (newNode == NULL) {

      printf("Memory allocation failed.\n");

        exit(1);
        }
    strcpy(newNode->location, location);

    newNode->totalAffected = affected;

    newNode->totalResources = resources;

    newNode->left = NULL;

    newNode->right = NULL;

    return newNode;

}
LocationNode* insertOrUpdateLocation(LocationNode *root, char location[], int affectedAdd, int resourceAdd) {

    if (root == NULL) {

        return createLocationNode(location, affectedAdd, resourceAdd);
        }

    int compare = strcmp(location, root->location);

    if (compare == 0) {

        root->totalAffected += affectedAdd;

        root->totalResources += resourceAdd;

    }

    else if (compare < 0) {

        root->left = insertOrUpdateLocation(root->left, location, affectedAdd, resourceAdd);
        }

    else {

        root->right = insertOrUpdateLocation(root->right, location, affectedAdd, resourceAdd);
        }
        return root;
        }
LocationNode* searchLocation(LocationNode *root, char location[]) {

    if (root == NULL) {

        return NULL;
        }

    int compare = strcmp(location, root->location);

    if (compare == 0) {

        return root;
        }

    else if (compare < 0) {

        return searchLocation(root->left, location);
        }

    else {

        return searchLocation(root->right, location);
        }
    }

 void showAllLocations(LocationNode *root) {

    if (root == NULL) {

        return;
        }
    showAllLocations(root->left);

    printf("\nLocation: %s\n", root->location);

    printf("Total affected people: %d\n", root->totalAffected);

    printf("Total resources: %d\n", root->totalResources);

    showAllLocations(root->right);

}

/* =============================
   LINKED LIST FUNCTIONS
   ============================= */
Person* addPerson(Person *head, LocationNode **locationRoot, int *personId) {

    Person *newPerson = (Person*) malloc(sizeof(Person));

    if (newPerson == NULL) {

        printf("Memory allocation failed.\n");

        return head;
        }

    newPerson->id = (*personId)++;

    inputText("Enter person name: ", newPerson->name, NAME_SIZE);

    inputText("Enter location: ", newPerson->location, NAME_SIZE);

    newPerson->severity = inputSeverity();

    newPerson->next = NULL;

    if (head == NULL) {

        head = newPerson;
        }
    else {

        Person *temp = head;


    while (temp->next != NULL) {

            temp = temp->next;
            }
       temp->next = newPerson;

    }

    *locationRoot = insertOrUpdateLocation(*locationRoot, newPerson->location, 1, 0);

    printf("\nAffected person added successfully.\n");

    return head;

}
void showPeople(Person *head) {

    if (head == NULL) {

        printf("\nNo affected people found.\n");

        return;
        }

        printf("\n===== AFFECTED PEOPLE LIST =====\n");

        while (head != NULL) {

        printf("\nID: %d\n", head->id);

        printf("Name: %s\n", head->name);

        printf("Location: %s\n", head->location);

        printf("Severity: %d\n", head->severity);

        head = head->next;
        }
        }
Resource* addResource(Resource *head, LocationNode **locationRoot, int *resourceId) {

    Resource *newResource = (Resource*) malloc(sizeof(Resource));

    if (newResource == NULL) {

        printf("Memory allocation failed.\n");

        return head;
        }

    newResource->id = (*resourceId)++;

    inputText("Enter resource name: ", newResource->name, NAME_SIZE);

    inputText("Enter location: ", newResource->location, NAME_SIZE);

    newResource->quantity = inputNumber("Enter quantity: ");

    newResource->next = NULL;


    if (head == NULL) {

        head = newResource;
        }

    else {

        Resource *temp = head;

        while (temp->next != NULL) {

            temp = temp->next;

            }

            temp->next = newResource;

    }

    *locationRoot = insertOrUpdateLocation(*locationRoot, newResource->location, 0, newResource->quantity);

    printf("\nResource added successfully.\n");

    return head;

}
void showResources(Resource *head) {

    if (head == NULL) {

        printf("\nNo resources found.\n");

        return;

    }
    printf("\n===== RESOURCE LIST =====\n");

    while (head != NULL) {

        printf("\nID: %d\n", head->id);

        printf("Name: %s\n", head->name);

        printf("Location: %s\n", head->location);

        printf("Quantity: %d\n", head->quantity);

        head = head->next;

        }
   }

/* =============================
   QUEUE FUNCTIONS
   ============================= */
void initializeQueue(Queue *q) {

    q->front = NULL;

    q->rear = NULL;

}
int isQueueEmpty(Queue *q) {

    return q->front == NULL;

}
void enqueue(Queue *q, Request request) {

    QueueNode *newNode = (QueueNode*) malloc(sizeof(QueueNode));

    if (newNode == NULL) {

        printf("Memory allocation failed.\n");

        return;

    }
    newNode->data = request;

    newNode->next = NULL;

    if (q->rear == NULL) {

        q->front = newNode;

        q->rear = newNode;
        }

    else {

        q->rear->next = newNode;

        q->rear = newNode;

    }

}
int dequeue(Queue *q, Request *request) {

    if (isQueueEmpty(q)) {

        return 0;

    }

    QueueNode *temp = q->front;

    *request = temp->data;

    q->front = q->front->next;

    if (q->front == NULL) {

        q->rear = NULL;

    }
    free(temp);

    return 1;

}
void addRequest(Queue *q, int *requestId) {

    Request request;

    request.id = (*requestId)++;

    inputText("Enter person name: ", request.personName, NAME_SIZE);

    inputText("Enter location: ", request.location, NAME_SIZE);

    inputText("Enter need/help type: ", request.need, NEED_SIZE);

    request.severity = inputSeverity();

    enqueue(q, request);

    printf("\nRescue request added to queue.\n");

}
void showRequests(Queue *q) {

    if (isQueueEmpty(q)) {

        printf("\nNo rescue requests found.\n");

        return;

    }

    QueueNode *temp = q->front;

    printf("\n===== RESCUE REQUEST QUEUE =====\n");

    while (temp != NULL) {

        printf("\nRequest ID: %d\n", temp->data.id);

        printf("Person Name: %s\n", temp->data.personName);

        printf("Location: %s\n", temp->data.location);

        printf("Need: %s\n", temp->data.need);

        printf("Severity: %d\n", temp->data.severity);

        temp = temp->next;

    }

}
/* =============================
   SORTING FUNCTION
   ============================= */
void swapRequests(Request *a, Request *b) {

    Request temp = *a;

    *a = *b;

    *b = temp;

}
void sortRequestsBySeverity(Queue *q) {

    if (q->front == NULL || q->front->next == NULL) {

        return;
        }

    QueueNode *i;

    QueueNode *j;

    for (i = q->front; i != NULL; i = i->next) {

        for (j = i->next; j != NULL; j = j->next) {

            if (i->data.severity < j->data.severity) {

                swapRequests(&i->data, &j->data);

                }

            }

        }

        printf("\nRequests sorted by severity.\n");

}
/* =============================
   STACK FUNCTIONS
   ============================= */
void push(StackNode **top, int dispatchId, Request request) {

    StackNode *newNode = (StackNode*) malloc(sizeof(StackNode));

    if (newNode == NULL) {

        printf("Memory allocation failed.\n");

        return;

    }

    newNode->dispatchId = dispatchId;

    newNode->data = request;

    newNode->next = *top;

    *top = newNode;

}
int pop(StackNode **top, int *dispatchId, Request *request) {

    if (*top == NULL) {

        return 0;

    }

    StackNode *temp = *top;

    *dispatchId = temp->dispatchId;

    *request = temp->data;

    *top = (*top)->next;

    free(temp);

        return 1;

}
void dispatchRequest(Queue *q, StackNode **stackTop, int *dispatchId) {

    Request request;

    if (isQueueEmpty(q)) {

        printf("\nNo rescue request available for dispatch.\n");

        return;

    }

    sortRequestsBySeverity(q);

    dequeue(q, &request);

    push(stackTop, *dispatchId, request);

    printf("\n===== DISPATCH SUCCESSFUL =====\n");

    printf("Dispatch ID: %d\n", *dispatchId);

    printf("Request ID: %d\n", request.id);

    printf("Person Name: %s\n", request.personName);

    printf("Location: %s\n", request.location);

    printf("Need: %s\n", request.need);

    printf("Severity: %d\n", request.severity);

    (*dispatchId)++;

}
void rollbackDispatch(Queue *q, StackNode **stackTop) {

    int dispatchId;

    Request request;

    if (pop(stackTop, &dispatchId, &request)) {

        enqueue(q, request);

        sortRequestsBySeverity(q);

        printf("\nRollback successful.\n");

        printf("Dispatch ID %d cancelled.\n", dispatchId);

        printf("Request ID %d returned to queue.\n", request.id);

    }

    else {

        printf("\nNo dispatch history found.\n");

    }

}
/* =============================
   SEARCH FUNCTION
   ============================= */
void searchLocationMenu(LocationNode *root) {

    char location[NAME_SIZE];

    inputText("Enter location to search: ", location, NAME_SIZE);

    LocationNode *result = searchLocation(root, location);

    if (result == NULL) {

        printf("\nLocation not found.\n");

    }

    else {

        printf("\n===== LOCATION FOUND =====\n");

        printf("Location: %s\n", result->location);

        printf("Total affected people: %d\n", result->totalAffected);

        printf("Total resources: %d\n", result->totalResources);

        }
}
/* =============================
   MEMORY CLEANUP FUNCTIONS
   ============================= */
void freePeople(Person *head) {

    while (head != NULL) {

        Person *temp = head;

        head = head->next;

        free(temp);
    }

}
void freeResources(Resource *head) {

    while (head != NULL) {

        Resource *temp = head;

        head = head->next;

        free(temp);
    }

}
void freeQueue(Queue *q) {

    Request temp;

    while (dequeue(q, &temp)) {

        /* Queue nodes are freed inside dequeue */

    }

}
void freeStack(StackNode *top) {

    while (top != NULL) {

        StackNode *temp = top;

        top = top->next;

        free(temp);

    }

}
void freeTree(LocationNode *root) {

    if (root == NULL) {

        return;

    }

    freeTree(root->left);

    freeTree(root->right);

    free(root);

}
/* =============================
   MENU
   ============================= */
void showMenu() {

    printf("\n\n====================================\n");

    printf(" DISASTER RESPONSE MANAGEMENT SYSTEM\n");

    printf(" Emergency Coordination Simulator\n");

    printf("====================================\n");

    printf("1. Add affected person\n");

    printf("2. Show affected people\n");

    printf("3. Add resource\n");

    printf("4. Show resources\n");

    printf("5. Add rescue request\n");

    printf("6. Show rescue requests\n");

    printf("7. Sort requests by severity\n");

    printf("8. Dispatch next request\n");

    printf("9. Rollback last dispatch\n");

    printf("10. Search location/resource\n");

    printf("11. Show all locations\n");

    printf("0. Exit\n");

    printf("====================================\n");

}
int main() {

    Person *personList = NULL;
    Resource *resourceList = NULL;

    Queue rescueQueue;
    StackNode *dispatchStack = NULL;
    LocationNode *locationRoot = NULL;

    int personId = 1;

    int resourceId = 1;

    int requestId = 1;

    int dispatchId = 1;

    int choice;

    initializeQueue(&rescueQueue);
    do {

        showMenu();

        choice = inputNumber("Enter your choice: ");

        switch (choice) {
            case 1:
                personList = addPerson(personList, &locationRoot, &personId);
                break;

            case 2:
                showPeople(personList);
                break;

            case 3:
                resourceList = addResource(resourceList, &locationRoot, &resourceId);
                break;

            case 4:
                showResources(resourceList);
                break;

            case 5:
                addRequest(&rescueQueue, &requestId);
                break;

            case 6:
                showRequests(&rescueQueue);
                break;
            case 7:
                if (isQueueEmpty(&rescueQueue)) {

                    printf("\nNo rescue requests to sort.\n");
                }

                else {

                    sortRequestsBySeverity(&rescueQueue);

                }
               break;

            case 8:
                dispatchRequest(&rescueQueue, &dispatchStack, &dispatchId);
                break;

            case 9:

                rollbackDispatch(&rescueQueue, &dispatchStack);

                break;

            case 10:

                searchLocationMenu(locationRoot);

                break;



            case 11:

                if (locationRoot == NULL) {

                    printf("\nNo location data found.\n");

                }

                else {

                    printf("\n===== LOCATION SUMMARY =====\n");

                    showAllLocations(locationRoot);

                }
                break;
            case 0:

                printf("\nExiting system. Stay safe!\n");
                break;
                default:

                printf("\nInvalid choice. Try again.\n");
                }
} while (choice != 0);

    freePeople(personList);

    freeResources(resourceList);

    freeQueue(&rescueQueue);

    freeStack(dispatchStack);

    freeTree(locationRoot);

    return 0;

}
