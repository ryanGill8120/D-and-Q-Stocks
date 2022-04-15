
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void divideAndConquer(int, int, float[], float*);
void toTheMoon(float*, int, char*);

int main(int argc, char *argv[]){

    char c;
    int i, n = 0;
    FILE *countFile, *inputFile;

    //determines value of n from file (assumes all values end in newline char ('\n'))
    countFile = fopen(argv[1], "r");
    while((c = fgetc(countFile)) != EOF)
        if (c == '\n') {n++;}
    fclose(countFile);

    //loads data into memory
    //I struggled with formating prices correctly, I believe this should work for all inputs though
    float *priceArray = malloc(sizeof(float) * n);
    inputFile = fopen(argv[1], "r");
    char val[100];
    for (i = 0; i < n; i++){
        
        fscanf(inputFile, "%s", val);
        priceArray[i] = strtof(val, NULL);

    }

    fclose(inputFile);

    //execution and output
    toTheMoon(priceArray, n, argv[1]);

}

//finds the maximum profit of an array of prices where we can buy and sell once in O(nlogn) time
void divideAndConquer(int start, int end, float* priceArray, float* profitPtr){
    
    float profit1, min1 = 1000000000, max1 = 0;
    float profit2, min2 = 1000000000, max2 = 0;
    float finalProfit;
    int i, mid, minDay1, maxDay1, minDay2, maxDay2;
    if (start == end)
        return;
    
    mid = (start + end)/2;

    //Divide step (log n)
    divideAndConquer(start, mid, priceArray, profitPtr);
    divideAndConquer((mid + 1), end, priceArray, profitPtr);

    //combine steps, at most 4n at the lowest height of the tree, h = 1 , otherwise 4 * (n/h). sum of 4n for each level of tree

    //makes two passes of segmented data, one that prioritizes finding the maximum value of the set, and one that prioritizes
    //finding the minimum value of the set. After which, each pass attempts to find the opposite value in a way that is appropriate
    //to the confines of buying before selling

    //Maximum priority, finds the max of the set and its respective day (index)
    for (i = start; i < end + 1; i++){
            if (priceArray[i] > max1){
                max1 = priceArray[i];
                maxDay1 = i;
            }
    }

    //now iterates to find the lowest value before the max, and its day
    for (i = start; i < maxDay1 + 1; i++){
        if (priceArray[i] < min1){
            min1 = priceArray[i];
            minDay1 = i;
        }
    }

    //finds the min of the set
    for (i = start; i < end + 1; i++){
        if (priceArray[i] < min2){
            min2 = priceArray[i];
            minDay2 = i;
        }
    }

    //finds largest value after the min
    for (i = minDay2; i < end + 1; i++){
        if (priceArray[i] > max2){
            max2 = priceArray[i];
            maxDay2 = i;
        }
    }

    //compares the profits of both situations and takes the largest
    profit1 = max1 - min1;
    profit2 = max2 - min2;
    finalProfit = (profit1 > profit2) ? profit1 : profit2;

    //compares the profit to the largest profit recorded from all recursions, recording the largest profit determined within a set.
    //Since sets as small as 2 or 3 are considered at lowest level of the tree, the algorithm will find the greatest profit at some
    //point, even as the stack unwinding makes the set closer to n, the largest profit will be considered even if it happened at a 
    //low level or only on one side of the tree.    
    if (finalProfit > *profitPtr)
        *profitPtr = finalProfit;
    
    return;
}

//Output function whose primary role is to maintain the value of the maximum profit outside the scope of the recursive calls
//I find this approach easier to work with rather than handling the value as the return of the recursive functions
void toTheMoon(float *priceArray, int n, char* fileName){

    float maxProfit = 0;
    divideAndConquer(0, n-1, priceArray, &maxProfit);
    printf("The optimal solution for %s is: %.2f\n", fileName, maxProfit);

}
