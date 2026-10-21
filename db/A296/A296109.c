/*
 * A296109  Number of n X 2 0..1 arrays with each 1 adjacent to 3 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 5*a(n-3) - 2*a(n-4) - 6*a(n-5) -
 *            4*a(n-6) with initial values a(1)=1, a(2)=2, a(3)=3, a(4)=4,
 *            a(5)=8, a(6)=17
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 55, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, free, memcpy */
#ifdef __cplusplus
#include <new>        /* ::new, ::operator delete[] */
#endif

static seq_t *arr = NULL;      /* cached values, arr[0] unused */
static idx_t  max_computed = 0; /* highest index already filled */
static size_t arr_cap      = 0;  /* current allocation size */

/* Base values for indices 1 … 6 */
static const seq_t base_vals[7] = {0, 1, 2, 3, 4, 8, 17};

/* Ensure capacity for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (arr_cap > n)
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr;

#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
    if (arr) {
        /* copy already computed elements */
        for (size_t i = 0; i <= (size_t)max_computed; ++i)
            new_arr[i] = arr[i];
        delete[] arr;
    }
#else
    new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (arr) {
        memcpy(new_arr, arr, (size_t)(max_computed + 1) * sizeof(seq_t));
        free(arr);
    }
#endif
    arr  = new_arr;
    arr_cap = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)    /* offset is 1; undefined for n < 1 */
        return (seq_t)0;

    ensure_capacity(n);

    /* If value already computed, return it */
    if (n <= max_computed)
        return arr[n];

    /* Initialize base terms 1 … 6 (overwrites if already present, harmless) */
    idx_t upto = (n < 6 ? n : 6);
    for (idx_t i = 1; i <= upto; ++i)
        arr[i] = base_vals[i];
    if (max_computed < 6)
        max_computed = 6;

    /* Compute missing terms via recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        arr[i] =  arr[i-1]
                + 2 * arr[i-2]
                + 5 * arr[i-3]
                - 2 * arr[i-4]
                - 6 * arr[i-5]
                - 4 * arr[i-6];
    }
    max_computed = n;
    return arr[n];
}
