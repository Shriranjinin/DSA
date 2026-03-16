#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "social.h"

void initQueue(Queue *q) {
    q->front = q->rear = 0;
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

int isFull(Queue *q) {
    return (q->rear + 1) % QUEUE_SIZE == q->front;
}

void enqueue(Queue *q, int val) {
    if (isFull(q)) {
        printf("Queue Overflow! Cannot enqueue %d\n", val);
        return;
    }
    q->items[q->rear] = val;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
}

int dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue Underflow! Cannot dequeue.\n");
        return -1;
    }
    int val = q->items[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    return val;
}


static int getUserIndex(const Network *net, const char *name) {
    for (int i = 0; i < net->userCount; i++) {
        if (strcmp(net->users[i].name, name) == 0)
            return i;
    }
    return -1;
}





static void addFriend(Network *net, int u, int v) {

    Node *check = net->users[u].friends;
    while (check) {
        if (check->userIndex == v)
            return;
        check = check->next;
    }

    Node *newNode = malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->userIndex = v;
    newNode->next = net->users[u].friends;
    net->users[u].friends = newNode;
}


void initializeNetwork(Network *net) {
    net->users = NULL;
    net->userCount = 0;
    net->friendCount = 0;

    printf("=== connectNet===\n");
    printf("\n");
    printf("\n");
    printf("Enter number of users (max %d): ", MAX_USERS);

    if (scanf("%d", &net->userCount) != 1) {
        printf("Invalid number of users.\n");
        exit(1);
    }

    if (net->userCount <= 0 || net->userCount > MAX_USERS) {
        printf("Invalid number of users.\n");
        exit(1);
    }

    net->users = malloc(net->userCount * sizeof(User));
    if (!net->users) {
        printf("Memory allocation failed!\n"); //net->users is NULL.
        exit(1);
    }


    printf("Enter each user name:\n");
    for (int i = 0; i < net->userCount; i++) {

    if (scanf("%49s", net->users[i].name) != 1) {
        printf("Invalid user name.\n");
        exit(1);
    }


    for (int j = 0; j < i; j++) {
        if (strcmp(net->users[j].name, net->users[i].name) == 0) {
            printf("Duplicate name '%s' not allowed. Enter a different name:\n", net->users[i].name);
            i--;
            break;
        }
    }

    net->users[i].friends = NULL;
}




    printf("Enter number of friendships: ");
    if (scanf("%d", &net->friendCount) != 1) {
        printf("Invalid number.\n");
        exit(1);
    }

    if (net->friendCount < 0) {
        printf("Invalid number of friendships.\n");
        exit(1);
    }



    printf("Enter friendships (user1 user2):\n");
    for (int i = 0; i < net->friendCount; i++) {
        char uName[MAX_NAME_LEN], vName[MAX_NAME_LEN];
        //Temporary buffers to hold user names for each friendship.

        if (scanf("%49s %49s", uName, vName) != 2) {
            printf("Invalid friendship input.\n");
            continue;
        }

        int u = getUserIndex(net, uName);
        int v = getUserIndex(net, vName);

        if (u != -1 && v != -1 && u != v) {
            addFriend(net, u, v);
            addFriend(net, v, u);
        } else {
            printf("Warning: could not add %s - %s\n", uName, vName);
        }
    }
    printf("Network setup complete!\n\n");
}





void printNetwork(const Network *net) {
    printf("\n=== Friends Network ===\n");
    for (int i = 0; i < net->userCount; i++) {
        printf("%s:", net->users[i].name);
        Node *curr = net->users[i].friends;
        while (curr) {
            printf(" %s", net->users[curr->userIndex].name);
            curr = curr->next;
        }
        printf("\n");
    }
    printf("\n");
}

void listFriends(const Network *net, const char *userName) {
    int idx = getUserIndex(net, userName);
    if (idx == -1) {
        printf("User %s not found.\n\n", userName);
        return;
    }

    printf("\nFriends of %s:\n", userName);// display header
    Node *curr = net->users[idx].friends;

    if (!curr) {
        printf("- No friends found\n\n");
        return;
    }

    while (curr) {
        printf("- %s\n", net->users[curr->userIndex].name);
        curr = curr->next;
    }
    printf("\n");
}

void findMutualFriends(const Network *net, const char *user1, const char *user2) {
    int u1 = getUserIndex(net, user1);
    int u2 = getUserIndex(net, user2);

    if (u1 == -1 || u2 == -1) {
        printf("One or both users not found.\n\n");
        return;
    }

    int friendFlags[MAX_USERS] = {0};
    Node *curr = net->users[u1].friends; // u1 so user 1
    while (curr) {
        friendFlags[curr->userIndex] = 1;
        curr = curr->next;
    }

    printf("\nMutual friends between %s and %s:\n", user1, user2);
    int found = 0; //Tracks if any mutual friend is found.

    curr = net->users[u2].friends; //u2 so user2
    while (curr) {
        if (friendFlags[curr->userIndex]) {
            printf("- %s\n", net->users[curr->userIndex].name);
            found = 1;
        }
        curr = curr->next;
    }

    if (!found) printf("- None\n");
    printf("\n");
}

void recommendFriends(const Network *net, const char *userName, int degree) {
    int start = getUserIndex(net, userName);
    if (start == -1) {
        printf("User %s not found.\n\n", userName);
        return;
    }

    int current_level[MAX_USERS], next_level[MAX_USERS];
    int curr_count = 0, next_count = 0;

    current_level[curr_count++] = start;

    for (int level = 1; level <= degree; level++) {
        next_count = 0;

        for (int i = 0; i < curr_count; i++) {
            int curr = current_level[i];
            Node *temp = net->users[curr].friends;

            while (temp) {
                int nbr = temp->userIndex;

                int exists = 0; // to check if the friend already existed in the list
                for (int j = 0; j < next_count; j++) {
                    if (next_level[j] == nbr) {
                        exists = 1;
                        break;
                    }
                }

                if (!exists) next_level[next_count++] = nbr; //// add if not duplicate
                temp = temp->next;   //move to next friend
            }
        }

        memcpy(current_level, next_level, sizeof(int) * next_count);
        curr_count = next_count;

        if (level == degree) {
            printf("\nFriend recommendations for %s at exactly %d degree(s):\n",
                   userName, degree);

            int found = 0;

            for (int i = 0; i < curr_count; i++) { //skip self
                int idx = current_level[i];
                if (idx == start) continue;

                if (degree == 1) {
                    printf("- %s\n", net->users[idx].name);
                    found = 1;
                } else {
                    int isDirect = 0;
                    Node *temp = net->users[start].friends;
                    while (temp) {
                        if (temp->userIndex == idx) { // compare each friend userindex
                            isDirect = 1;
                            break;
                        }
                        temp = temp->next;
                    }
                    if (!isDirect) {
                        printf("- %s\n", net->users[idx].name);
                        found = 1; // print if not direct friend
                    }
                }
            }
            if (!found) printf("- None\n");
            printf("\n");
            return;
        }
    }
}

void findMostPopular(const Network *net) {
    int max = 0, popular = -1;

    for (int i = 0; i < net->userCount; i++) {
        int count = 0;
        Node *curr = net->users[i].friends;

        while (curr) {
            count++;
            curr = curr->next;
        }

        if (count > max) {
            max = count;
            popular = i;
        }
    }

    if (popular != -1)
        printf("\nMost popular user: %s (%d connections)\n\n",
               net->users[popular].name, max);
    else
        printf("\nNo users found.\n\n");
}

void findShortestPath(const Network *net, const char *user1, const char *user2) {
    int start = getUserIndex(net, user1);
    int end = getUserIndex(net, user2);

    if (start == -1 || end == -1) {
        printf("One or both users not found.\n\n");
        return;
    }

    int visited[MAX_USERS] = {0};
    int parent[MAX_USERS];
    for (int i = 0; i < MAX_USERS; i++) parent[i] = -1;

    Queue q;
    initQueue(&q);

    enqueue(&q, start);
    visited[start] = 1;

    while (!isEmpty(&q)) {
        int u = dequeue(&q); //This is the current user being processed.
        if (u == -1) break;

        Node *temp = net->users[u].friends;
        while (temp) {
            int v = temp->userIndex;
            if (!visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                enqueue(&q, v);
            }
            temp = temp->next;
        }
    }

    if (!visited[end]) {
        printf("\nNo connection path between %s and %s.\n\n", user1, user2);
        return;
    }

    int path[MAX_USERS], count = 0;
    for (int v = end; v != -1; v = parent[v]) // this moves backward
        path[count++] = v;

    printf("\nShortest path from %s to %s:\n", user1, user2);
    for (int i = count - 1; i >= 0; i--) {
        printf("%s", net->users[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\n\n");
}

void freeNetwork(Network *net) {
    if (!net || !net->users) return;

    for (int i = 0; i < net->userCount; i++) {
        Node *curr = net->users[i].friends;  //Pointer to first friend node for user i.
        while (curr) {
            Node *tmp = curr;
            curr = curr->next; //Walk through linked list. and free
            free(tmp);
        }
    }
    free(net->users); //Free main user array.
    net->users = NULL;
    net->userCount = 0;
    net->friendCount = 0;
}

