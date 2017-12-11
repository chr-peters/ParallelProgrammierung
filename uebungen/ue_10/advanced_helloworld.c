#include <omp.h>

int main () {
#pragma omp parallel num_threads(4)
  {  
    // get the id
    int id = omp_get_thread_num();

    // get the number of threads
    int threadNum = omp_get_num_threads();

    printf("Hello, I am %d and there are %d threads in my team.\n", id, threadNum);

    if (id == 0) {
      // get the maximum number of threads
      int maxThreads = omp_get_max_threads();

      printf("The maximum number of threads is %d.\n", maxThreads);
    }
  }
}
