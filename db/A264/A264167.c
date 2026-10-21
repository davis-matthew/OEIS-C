/*
 * A264167  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change 0,0 0,1 1,0 -2,-1 or
 *          -1,-2.
 * https://oeis.org/A264167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 7*a(n-2) + 23*a(n-3) + 35*a(n-4) + 18*a(n-5)
 *            with a(1)=4, a(2)=25, a(3)=98, a(4)=418, a(5)=1862, a(6)=8007,
 *            a(7)=34539
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache to store computed values of the sequence up to the largest n requested.
   The sequence is memoized; calls can arrive in any order. */
static seq_t *cache = NULL;
static idx_t last_computed = 0;  /* The largest n for which cache[n] is valid */

/* Returns the n-th term a(n) (n>=1).  For n==0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0U) return 0;

    /* Initialise cache with the first 7 values if necessary */
    if (!cache) {
        cache = (seq_t *)malloc((8U) * sizeof(seq_t));  /* indices 0..7 used */
        if (!cache) return 0;
        cache[0] = 0;                 /* unused */
        cache[1] = 4;
        cache[2] = 25;
        cache[3] = 98;
        cache[4] = 418;
        cache[5] = 1862;
        cache[6] = 8007;
        cache[7] = 34539;
        last_computed = 7U;
    }

    /* If requested n is larger than any computed value, extend cache */
    if (n > last_computed) {
        idx_t new_cap = n + 1U;  /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failed */
        cache = tmp;

        /* Compute new terms using the empirical recurrence */
        for (idx_t i = last_computed + 1U; i <= n; ++i) {
            cache[i] = cache[i - 1U]
                     + (seq_t)7U  * cache[i - 2U]
                     + (seq_t)23U * cache[i - 3U]
                     + (seq_t)35U * cache[i - 4U]
                     + (seq_t)18U * cache[i - 5U];
        }
        last_computed = n;
    }

    return cache[n];
}
