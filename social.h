#ifndef NETWORK_H
#define NETWORK_H

#define MAX_USERS 100
#define MAX_NAME_LEN 50
#define QUEUE_SIZE 1000

typedef struct Node {
    int userIndex;
    struct Node *next;
} Node;

typedef struct {
    char name[MAX_NAME_LEN];
    Node *friends;
} User;

typedef struct {
    User *users;
    int userCount;
    int friendCount;
} Network;

typedef struct {
    int items[QUEUE_SIZE];
    int front, rear;
} Queue;


void initQueue(Queue *q);
int isEmpty(Queue *q);
int isFull(Queue *q);
void enqueue(Queue *q, int val);
int dequeue(Queue *q);


void initializeNetwork(Network *net);
void printNetwork(const Network *net);
void listFriends(const Network *net, const char *userName);
void findMutualFriends(const Network *net, const char *user1, const char *user2);
void recommendFriends(const Network *net, const char *userName, int degree);
void findMostPopular(const Network *net);
void findShortestPath(const Network *net, const char *user1, const char *user2);
void freeNetwork(Network *net);

#endif
