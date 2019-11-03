#include <stdio.h>

#define N 4
#define NUM (N * N + 1) * N / 2 // the sum of numbers in a column or row or diagonal

int sol = 0; // number of solution
void find_ms(int ms[N * N], int blank_position[N * N], int list[N * N], int now_position, int end);
int check(int ms[N * N], int last_fiilied);
void print(int ms[N * N]);

int main(void)
{
    int i, j; // loop index
    int ms[N * N]; // to  read in the initial magic square data
    int blank_position[N * N]; // record the place that is empty at first
    int record[N * N]; // to record the numbers did appear
    int list[N * N]; // the numbers that have to be fillied in

    // assume every number did'nt appear at first
    for (i = 0; i < N * N; i++)
        record[i] = 0;

    // read the data in, and record the blank space and the number appear
    for (i = 0, j = 0; i < N * N; i++) {
        scanf("%d", &ms[i]);
        if (ms[i] == 0) 
            blank_position[j++] = i; // means the ith entries of the matrix is empty
        else
            record[ms[i] - 1] = 1; // means the ms[i] did appear
    }

    // get the list that have to be filled in according to the previous "record" array
    for (i = 0, j = 0; i < N * N; i++)
        if (record[i] == 0) // i did't show up 
            list[j++] = i + 1;
        
    // then j will means the total number of blank space
    find_ms(ms, blank_position, list, 0, j);
    printf("Toltal number of solutions is %d\n", sol);
    return 0;
}

/* For every recursion step, we only fill in numbers in a specific balnk space 
 * After filling in the number in the specific blank , check if the magic square is still valid, 
 * 1.If it is valid then call the next recurrence step to fill number in the next blank.
 * 2.If it is invalid, try to fill in another number(in the loop).
 * 3.If we haved tried every single number that is possible and can still find no answer
 * then it means that the former find_ms called this recursion step filled in the wrong number
 * so that the number filled in be zero(canceling the filling), and go back to the former recursion step .
 * The variable "now_position" means which blank we should fill in in this recurrsion step
 * The variable "end" indicates the total number of the "list"
 */
void find_ms(int ms[N * N], int blank_position[N * N], int list[N * N], int now_position, int end)
{
    int i; // loop index
    int temp;
    
    // means every number in the list have been filled, i.e. found a solution
    if (now_position == end) { 
        printf("Solution: %d\n", ++sol);
        print(ms);
        printf("\n");
    }
    else  { 
        for (i = now_position; i < end; i++) {
            // swap list[i] and list[now_position]
            temp = list[now_position];
            list[now_position] = list[i];
            list[i] = temp;
            // fill in the number
            ms[blank_position[now_position]] = list[now_position];
            // if after filling in is still valid, fill in the next blank
            if (check(ms, blank_position[now_position]) == 1) 
                find_ms(ms, blank_position, list, now_position + 1, end);
            // swap list[i] and list[now_position]
            temp = list[now_position];
            list[now_position] = list[i];
            list[i] = temp;
        }
        // after filling in any possible number in the list and still can't find any valid square
        // then that the number fillid in be zero(cancel this recurrsion step)
        ms[blank_position[now_position]] = 0;
    }
}

// The funtion will check if the row and column and the diagnol is okay.
// We only have to check the row and column the last filled in number loacted.
// If the last filled number is located at diagonal then we should also check the diagonal.
int check(int ms[N * N], int last_filled) 
{
    int i, j;
    int sum;
    int has_zero;
    
    // check row, j means the index of the start of the row that is checking 
    for (i = 0, j = last_filled / N * N, has_zero = 0, sum = 0; i < N; i++) {
        sum += ms[j + i];
        if (ms[j + i] == 0)
            has_zero = 1;
    }
    if ((has_zero == 0 && sum != NUM) || sum > NUM)
        return 0;


    // check column, j means the column that we are checking
    for (i = 0, j = last_filled % N, has_zero = 0, sum = 0; i < N; i++) {
        sum += ms[i * N + j];
         if (ms[i * N + j] == 0)
            has_zero = 1;
    }
    if ((has_zero == 0 && sum != NUM) || sum > NUM)
        return 0;

    // check major diagonal
    if (last_filled / N == last_filled % N) {// means it's on the major diagonal
        for (i = 0, has_zero = 0, sum = 0; i < N; i++) {
            sum += ms[i + i * N];
            if (ms[i + i * N] == 0)
                has_zero = 1;
        }
        if ((has_zero == 0 && sum != NUM) || sum > NUM)
                return 0;
    }

    // check minor diagonal
    if (i / N == (N - 1) - (last_filled % N)) {// means it's on the minor diagonal
        for (i = 0, has_zero = 0, sum = 0; i < N; i++) {
            sum += ms[(N - 1) * (i + 1)];
            if (ms[(N - 1) * (i + 1)] == 0)
                has_zero = 1;
        }
        if ((has_zero == 0 && sum != NUM) || sum > NUM)
                return 0;
    }
    // if col + row + diagonal is okay then is okay
    return 1;
} 

void print(int ms[N * N]) 
{
    int i;
    for (i = 0; i < N * N; i++) {
        printf("%4d", ms[i]);
        if ((i + 1)% N  == 0)
            printf("\n");
    }
}