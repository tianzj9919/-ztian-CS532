#include <stdio.h>
//gcc InsertionSort_ztian.c -o InsertionSort
//./InsertionSort_ztian
int main(){
    int N;
    printf("Enter the number of element: ");
    scanf("%d", &N);
    int arr[N];

    for(int i = 0; i < N; i++){
        printf("Please enter element %d of array: ", i+1);
        scanf("%d", &arr[i]);
    }

    for(int i = 1; i < N; i++){
        int temp = arr[i];
        int cur = i - 1;
        while( cur >= 0 && arr[cur] > temp){
            arr[cur + 1] = arr[cur];
            cur--;
        }
        arr[cur + 1] = temp;
    }

    printf("Sorted array: ");
    for(int i = 0; i < N; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}