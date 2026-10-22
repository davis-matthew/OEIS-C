/*
 * A322801  Number of compositions (ordered partitions) of n into centered
 *          pentagonal numbers (A005891).
 * https://oeis.org/A322801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A322801: Number of compositions of n into centered pentagonal numbers (A005891).
 * Recurrence: a(0) = 1
 *             a(n) = Σ_{p≤n, p is centered pentagonal} a(n-p)
 *             where p = 1 + 5*k*(k+1)/2  (k ≥ 0).
 */

#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static dynamic programming cache */
    static seq_t *dp   = NULL;
    static size_t cap  = 0;   /* allocated size of dp array */
    static size_t len  = 0;   /* number of entries that have been computed (dp[0]..dp[len-1]) */

    /* Ensure the cache can hold up to index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        dp  = tmp;
        cap = new_cap;
    }

    /* Base case: initialise a(0) the first time */
    if (len == 0) {
        dp[0] = 1;
        len   = 1;
    }

    /* Compute all entries from len up to n inclusively */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;
        /* iterate over all centered pentagonal numbers ≤ i */
        for (idx_t k = 0; ; ++k) {
            idx_t part = 1 + (5 * k * (k + 1)) / 2;
            if (part > i) break;
            sum += dp[(size_t)(i - part)];
        }
        dp[(size_t)i] = sum;
    }

    len = (size_t)(n + 1);
    return dp[(size_t)n];
}
