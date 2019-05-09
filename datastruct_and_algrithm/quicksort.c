#include <stdio.h>

int partition(int arr[], int left, int right){
  int base = arr[left];
  while(left < right){
    while(left<right && arr[right] >= base){
      right--;
    }
    arr[left] = arr[right];
    while(left<right && arr[left] <= base){
      left++;
    }
    arr[right] = arr[left];
  }
  arr[left] = base;
  return left;
}

void quicksort(int arr[], int left, int right){
  if(left < right){
    int base = partition(arr, left, right);
    quicksort(arr, left, base-1);
    quicksort(arr, base+1,right);
  }
}

int main(){
  int arr[] = {53,66,34,65,43,565,34,55};
  quicksort(arr,0,sizeof(arr)/sizeof(int)-1);
  for(int i=0;i<sizeof(arr)/sizeof(int);i++){
    printf("%d  ",arr[i]);
  }
  printf("\n");
}
