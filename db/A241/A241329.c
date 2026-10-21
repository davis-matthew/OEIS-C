/*
 * A241329  Number of 2 X n 0..3 arrays with no element equal to fewer
 *          vertical neighbors than horizontal neighbors, with new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A241329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a recurrence a(n) = 12*a(n-1) - 26*a(n-2) + 2*a(n-3) + 28*a(n-4)
 *            - 6*a(n-5) - 9*a(n-6) holds for n > 8. Initial values (n = 1 …
 *            8) are hard‑coded from the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n < 1)               /* offset is 1 – undefined for n<1 */
        return (seq_t)0;

    /* Unrealistically large n would overflow array indices; guard against it */
    if (n > (size_t)-1) /* TODO: proper error handling if desired */
        return (seq_t)0;

    /* Ensure cache can hold up to n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)calloc(new_cap + 1, sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0; /* allocation failure: return zero */
        for (size_t i = 1; i <= cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the first eight terms (offset 1) */
    if (cache[1] == 0) {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)59;
        cache[4] = (seq_t)530;
        cache[5] = (seq_t)4877;
        cache[6] = (seq_t)45057;
        cache[7] = (seq_t)416533;
        cache[8] = (seq_t)3851085;
    }

    /* Compute values up to n on demand */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        seq_t val = 12 * cache[i-1];
        val -= 26 * cache[i-2];
        val += 2 * cache[i-3];
        val += 28 * cache[i-4];
        val -= 6 * cache[i-5];
        val -= 9 * cache[i-6];
        cache[i] = val;
    }

    return cache[n];
}
