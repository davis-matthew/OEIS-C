/*
 * A266008  Number of n X 4 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors equal to itself.
 * https://oeis.org/A266008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 86, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4, a(3)=4, a(4)=9;
   a(n) = -a(n-1) + 3*a(n-2) + 3*a(n-3) - a(n-4) for n >= 5. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  known = 0;   /* number of terms currently stored */
    static size_t capacity = 0;/* allocated size of cache array */

    /* Allocate initial storage if needed */
    if (cache == NULL) {
        capacity = (n > 4 ? n : 4);
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failure */
        known = 0;
    }

    /* Initialize the first four terms once */
    if (known < 4) {
        cache[0] = 1;   /* a(1) */
        cache[1] = 4;   /* a(2) */
        cache[2] = 4;   /* a(3) */
        cache[3] = 9;   /* a(4) */
        known = 4;
    }

    /* If requested term is already known, return it */
    if (n <= known) {
        return cache[n - 1];
    }

    /* Grow the cache if necessary */
    if (n > capacity) {
        size_t new_cap = capacity;
        while (new_cap < n) new_cap <<= 1;  /* double until sufficient */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;    /* reallocation failure */
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute new terms using the recurrence relation */
    for (idx_t i = known + 1; i <= n; ++i) {
        /* Indices are one‑based in the formula, zero‑based in array */
        seq_t prev1 = cache[i - 2]; /* a(i-1) */
        seq_t prev2 = cache[i - 3]; /* a(i-2) */
        seq_t prev3 = cache[i - 4]; /* a(i-3) */
        seq_t prev4 = cache[i - 5]; /* a(i-4) */

        seq_t tmp = 3 * prev2 + 3 * prev3; /* 3*a(i-2)+3*a(i-3) */
        tmp -= prev1;                      /* subtract a(i-1)   */
        tmp -= prev4;                      /* subtract a(i-4)   */
        cache[i - 1] = tmp;                /* store a(i)        */
    }

    known = n;
    return cache[n - 1];
}
