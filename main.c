#include <stdio.h>
#include <string.h>
#include "social.h"

int main() {
    Network net;
    initializeNetwork(&net);

    printf("=====================================================\n");
    printf("Available commands:\n");
    printf(" print_network\n list_friends <user>\n mutual_friends <u1> <u2>\n");
    printf(" recommend_friends <user> <degree>\n most_popular\n shortest_path <u1> <u2>\n");
    printf("=====================================================\n\n");

    char command[40];

    while (scanf("%39s", command) == 1) {
        if (strcmp(command, "print_network") == 0)
            printNetwork(&net);

        else if (strcmp(command, "list_friends") == 0) {
            char user[MAX_NAME_LEN];
            scanf("%49s", user);
            listFriends(&net, user);
        }

        else if (strcmp(command, "mutual_friends") == 0) {
            char u1[MAX_NAME_LEN], u2[MAX_NAME_LEN];
            scanf("%49s %49s", u1, u2);
            findMutualFriends(&net, u1, u2);
        }

        else if (strcmp(command, "recommend_friends") == 0) {
            char user[MAX_NAME_LEN];
            int deg;
            scanf("%49s %d", user, &deg);
            recommendFriends(&net, user, deg);
        }

        else if (strcmp(command, "most_popular") == 0)
            findMostPopular(&net);

        else if (strcmp(command, "shortest_path") == 0) {
            char u1[MAX_NAME_LEN], u2[MAX_NAME_LEN];
            scanf("%49s %49s", u1, u2);
            findShortestPath(&net, u1, u2);
        }

        else
            printf("Unknown command: %s\n\n", command);
    }

    freeNetwork(&net);
    printf("Session ended. Goodbye!\n");
    return 0;
}
