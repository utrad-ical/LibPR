

#define QSORT_MODE_ASCENDING 1
#define QSORT_MODE_DESCENDING 2


// pivotting
unsigned long PR_Pivot(float *a, char mode, unsigned long i, unsigned long j);

// data swap
unsigned long PR_DataSwapAndSprit(float* Dat, char mode, unsigned long top, unsigned long end, float ref);

// regression
void PR_QSortRegression(float* Dat, char mode, unsigned long top, unsigned long end);

// partition
unsigned long PR_Partition(float *a, unsigned long *b, unsigned long i, unsigned long j, float target);

// quick sort recurrence edition
void PR_QuickSortRecurrenceWithRank(float *a, unsigned long *b, unsigned long i, unsigned long j);

