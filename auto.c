#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 256

int prefix(const char *pre, const char *str){
    if (strncmp(pre, str, strlen(pre)) == 0){
        return 1;
    } else{
        return -1;
    }
}

int main(int argc, char **argv) {
    char *inputFilePath = argv[1]; //this keeps the path to input file
    char *queryWord = argv[2]; // this keeps the query word
    char lineInfo[256];
    int lineCount=0; //this variable will tell us how much memory to allocate

    //read input file
    FILE *fp = fopen(inputFilePath, "r");

    if(fp == NULL){
        fprintf(stderr, "Error opening file\n");
        return -1;
    }

    while (fgets(lineInfo, 255, fp) != NULL){
        lineCount++;
    }

    char nameArray [lineCount][50];
    int weightArray [lineCount];

    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    char word[BUFSIZE]; //to be used for reading lines in the loop below
    int weight;

    for(int i = 0; i < lineCount; i++) {
        fscanf(fp, "%s %d\n",word,&weight);
        strcpy(nameArray[i], word);
        weightArray[i] = weight;
    }


    int result;
    char matches[10000][50];
    int matchesWeight [10000];

    int counter = 0;
    int trigger = 0;

    for(int i = 0; i < lineCount; i ++){
        result = prefix(queryWord, nameArray[i]);
        if (result == 1){
            strcpy(matches[counter], nameArray[i]);
            matchesWeight[counter] = weightArray[i];
            trigger = 1;
            counter ++;
        }
    }

    if (trigger == 0){
        printf("No suggestion!");
    }
    else {
        char matchesClean[counter][50];
        int weightClean[counter];

        for (int i = 0; i < counter; i++) {
            strcpy(matchesClean[i], matches[i]);
            weightClean[i] = matchesWeight[i];
        }

        char tempWord[50];
        int tempWeight;

        for (int i = 0; i < counter; ++i) {
            for (int j = i + 1; j < counter; ++j) {
                if (weightClean[i] < weightClean[j]) {
                    tempWeight = weightClean[i];
                    strcpy(tempWord, matchesClean[i]);

                    weightClean[i] = weightClean[j];
                    strcpy(matchesClean[i], matchesClean[j]);

                    weightClean[j] = tempWeight;
                    strcpy(matchesClean[j], tempWord);
                }
            }
        }

        if(counter <= 10){
            for (int i = 0; i < counter; i++) {
                printf("%s %d\n", matchesClean[i], weightClean[i]);
            }
        }
        else{
            for (int i = 0; i < 10; i++) {
                printf("%s %d\n", matchesClean[i], weightClean[i]);
            }
        }
    }

    fclose(fp);

    return 0;
}
