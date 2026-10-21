/*
 * A287834  Number of words of length n over the alphabet {0,1,...,10} such
 *          that no two consecutive terms have distance 3.
 * https://oeis.org/A287834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 2*a(n-2) - 21*a(n-3) + 10*a(n-4); initial
 *            values: a(0)=1, a(1)=11, a(2)=105, a(3)=1005, a(4)=9621
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  length = 0;            /* number of terms stored */

    /* Grow the cache if needed.  Initial size at least 5 to hold base terms. */
    if (n + 1 > cap) {
        size_t new_cap = cap == 0 ? 5 : cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) return (seq_t)0;    /* out‑of‑memory – unlikely in test cases */
        cache = ptr;
        cap   = new_cap;
    }

    /* initialise base values once */
    if (length == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)105;
        cache[3] = (seq_t)1005;
        cache[4] = (seq_t)9621;
        length  = 5;
    }

    /* compute any missing terms up to n */
    for (idx_t i = length; i <= n; ++i) {
        seq_t val;
        if (i <= 4) {
            val = cache[i];
        } else {
            val = (seq_t)10 * cache[i-1];
            val -= (seq_t)2 * cache[i-2];
            val -= (seq_t)21 * cache[i-3];
            val += (seq_t)10 * cache[i-4];
        }
        cache[i] = val;
    }
    length = n + 1;
    return cache[n];
}
