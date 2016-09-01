// Mergesort
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int* recombine(int*, int, int*, int);
void print_array(int*, int);

// Debugging globals
static malloc_ctr = 0;
static free_ctr = 0;

// Implements mergesort.
int* mergesort(int* array, int array_len, int* run_ctr) {
  if (run_ctr != NULL) (*run_ctr)++;
  
  if (array_len <= 1) {
    // Base case: array of length 0 or 1.
    if (array_len == 0) return NULL;
    int* new_array = malloc(sizeof(int)*1);
    malloc_ctr++;
    new_array[0] = array[0];
    return new_array;
  }
  int midpoint = (array_len)/2;
  int left_len = midpoint;
  int right_len = array_len-midpoint;
  
  // Recursive call to decompose this array into left and 
  //  right arrays, because it is still longer than 1.
  int* left = mergesort(array, left_len, run_ctr);
  int* right = mergesort(array+midpoint, right_len, run_ctr);
  
  // Recombine the left and right lists.
  return recombine(left, left_len, right, right_len);
}

// Take two subarrays and combine them into a larger, sorted array.
int* recombine(int* left, int left_len, int* right, int right_len) {
  int left_ptr = 0;
  int right_ptr = 0;
  int new_ptr = 0;
  
  // Walk through both lists and combine into new list.
  int* new_array = malloc(sizeof(int)*(left_len+right_len));
  malloc_ctr++;
  if (!new_array) {
    printf("ERROR: could not malloc memory for array.\n");
    return NULL;
  }
  
  while(1) {
    if (left_ptr < left_len && right_ptr < right_len) {
      // Still items in both lists.
      // Out of the two items at the heads of the lists, choose the 
      //  smaller and append it to the new list.
      if (left[left_ptr] <= right[right_ptr]) {
        new_array[new_ptr] = left[left_ptr];
        left_ptr++;
        new_ptr++;
      } else {
        new_array[new_ptr] = right[right_ptr];
        right_ptr++;
        new_ptr++;
      }
      continue;
    }
    
    if (left_ptr < left_len) {
      // Only items remaining in left list.
      // Just copy the item to the new list.
      new_array[new_ptr] = left[left_ptr];
      left_ptr++;
      new_ptr++;
      continue;
    }
    
    if (right_ptr < right_len) {
      // Only items remaining in right list.
      // Just copy the item to the new list.
      new_array[new_ptr] = right[right_ptr];
      right_ptr++;
      new_ptr++;
      continue;
    }
    
    break;
  }
  
  // Free old lists, if necessary.
  if (left) {
    free(left);
    free_ctr++;
  } 
  if (right) {
    free(right);
    free_ctr++;
  }
  
  return new_array;
}

// Prints out an integer value array.
void print_array(int* array, int len) {
  int i;
  printf("[");
  for (i=0; i<len; i++) {
    printf("%d", array[i]);
    if (i < len-1) {
      printf(", ");
    }
  }
  printf("]\n");
}

// Generates a randomly populated array of integer values, from min to min+range.
int* generate_random_int_array(int len, int min, unsigned int range) {
  int* array = malloc(sizeof(int)*len);
  if (!array) {
    printf("ERROR: Failed to malloc.\n");
    return NULL;
  }
  
  int i;
  for (i=0; i<len; i++) {
    array[i] = rand() % range + min;
  }
  return array;
  
}

// Makes sure an input array is actually in ascending order.
int validate_array(int* array, int len) {
  int i;
  for (i=1; i<len; i++) {
    if (array[i] < array[i-1]) return 0;
  }
  return 1;
}

// Mergesort, implemented in C
int main(int argc, char* argv[]) {
  clock_t start = clock();
  int run_ctr = 0;
  srand(time(NULL));  // Initialize PRNG
  
  if (argc != 2) {
    printf("Usage: mergesort num, where num is the length of the randomly generated array.\n");
    return 0;
  }
  int len = atoi((const char*) argv[1]);
#define MIN -100
#define RANGE 200
  int* test_array = generate_random_int_array(len, MIN, RANGE);
  
  printf("Original array: ");
  print_array(test_array, len);
  
  int* sorted_array = mergesort(test_array, len, &run_ctr);
  printf("Sorted array: ");
  print_array(sorted_array, len);

  if (validate_array(sorted_array, len) == 1) {
    printf("SORT WORKS.\n");
  } else printf("sort broken...\n");
  printf("STATISTICS: ");
  printf("mergesort was called %d times. ", run_ctr);  
  printf("mallocs: %d, frees: %d (difference: %d, desired: 1)\n", malloc_ctr, free_ctr, malloc_ctr-free_ctr);
  
  free(test_array);
  free(sorted_array);
  printf("Tiempo transcurrido: %f", ((double)clock() - start) / CLOCKS_PER_SEC);
  printf("\n");
  return 0;
}
