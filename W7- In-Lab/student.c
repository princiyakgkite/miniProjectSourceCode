#include <stdio.h>
int binSearch(int arr[], int start, int end, int key){
    int mid = (start + end)/2;
    if(start > end) {
        return 0;
    }
    if(key == arr[mid]){
        return 1;
    }
    if(key > arr[mid]){
        return binSearch(arr, mid+1,end, key);
    }
    return binSearch(arr, start, mid - 1, key);
}
int main() {
    int n, key, found;
    scanf("%d", &n);
    int arr[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    scanf("%d", &key);
    found = binSearch(arr, 0, n-1, key);
    if (found) {
        printf("The search number found in the array.");
    }
    else {
        printf("The search number not found in the array.");
    }
    return 0;
}