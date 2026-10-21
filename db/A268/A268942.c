/*
 * A268942  Number of length-n 0..6 arrays with no repeated value unequal to
 *          the previous repeated value plus one mod 6+1.
 * https://oeis.org/A268942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 23*a(n-2) - 42*a(n-3) initial terms: a(1)=7,
 *            a(2)=49, a(3)=336 offset 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Do nothing for the (unused) 0‑th term */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of already computed values, 1‑based */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* allocated size (number of slots)   */
    static idx_t  cached = 0;  /* largest index that has been filled */

    /* Ensure we have a slot for index n (plus a dummy 0 slot) */
    idx_t need = n + 1;
    if (need < 4)          /* keep at least indices 0..3 */
        need = 4;

    if (cap < need) {
        /* Allocate or grow the cache */
        seq_t *tmp = NULL;
        if (cap == 0) {
            tmp = (seq_t *)malloc((size_t)need * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, (size_t)need * sizeof(seq_t));
        }
        if (tmp == NULL)      /* allocation failed – unlikely */
            return (seq_t)0;
        cache = tmp;
        cap = need;
    }

    /* Initialise the first three terms if they are not yet set */
    if (cached < 1) {
        cache[1] = (seq_t)7;
        cached = 1;
    }
    if (cached < 2) {
        cache[2] = (seq_t)49;
        cached = 2;
    }
    if (cached < 3) {
        cache[3] = (seq_t)336;
        cached = 3;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (idx_t i = cached + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)11
                 - cache[i - 2] * (seq_t)23
                 - cache[i - 3] * (seq_t)42;
    }
    cached = n;

    return cache[n];
}
