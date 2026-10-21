/*
 * A292301  p-INVERT of A010892, where p(S) = 1 + S - S^2.
 * https://oeis.org/A292301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) (n ≥ 5) with a(0) = -1,
 *            a(1) = 1, a(2) = 1, a(3) = -1, a(4) = 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function keeps a static cache that is expanded on demand.
   For C++ big‑integer types constructors are called via new[],
   for C built‑in types malloc works.               */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache state */
    static seq_t *cache   = NULL;   /* array of computed terms      */
    static idx_t capacity = 0;      /* current array capacity       */
    static idx_t count    = 0;      /* number of terms stored (last valid index+1) */

    /* Ensure the array is large enough */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity * 2 : 8;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_cap];                 /* constructs elements for big‑int types */
#else
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        if (!tmp) return 0;                      /* out of memory – caller will not use */

        if (cache) {
            for (idx_t i = 0; i < count; ++i) tmp[i] = cache[i];
            /* free old storage */
#ifdef __cplusplus
            delete [] cache;
#else
            free(cache);
#endif
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first five terms on first use */
    if (count == 0) {
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-1;
        cache[4] = (seq_t)0;
        count = 5;
    }

    /* Compute any missing terms up to n (if requested beyond current count) */
    for (idx_t i = count; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) - a(i-2) + a(i-3) - a(i-4) */
        cache[i] = cache[i-1] - cache[i-2] + cache[i-3] - cache[i-4];
    }
    if (n + 1 > count) count = n + 1;

    return cache[n];
}
