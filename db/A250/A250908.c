/*
 * A250908  Number of (n+1)X(3+1) 0..2 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 74*a(n-1) -825*a(n-2) +1101*a(n-3) -4926*a(n-4)
 *            +231*a(n-5) +1684*a(n-6) +736*a(n-7) Initial values: a(1)=4909,
 *            a(2)=297873, a(3)=18077319, a(4)=1096982379, a(5)=66566704705,
 *            a(6)=4039362762435, a(7)=245114118899485
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Initialize cache with first seven terms */
    if (cache == NULL) {
        cache_cap = 7;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* Allocation failure */

        cache[0] = 4909;
        cache[1] = 297873;
        cache[2] = 18077319;
        cache[3] = 1096982379;
        cache[4] = 66566704705;
        cache[5] = 4039362762435;
        cache[6] = 245114118899485;
        cache_len = 7;
    }

    /* Extend cache up to requested n */
    while (cache_len < (size_t)n) {
        /* Compute next term a(cache_len + 1) using recurrence */
        seq_t val = 0;
        val +=  (seq_t)74  * cache[cache_len - 1]; /* a(n-1) */
        val -=  (seq_t)825  * cache[cache_len - 2]; /* a(n-2) */
        val +=  (seq_t)1101 * cache[cache_len - 3]; /* a(n-3) */
        val -=  (seq_t)4926 * cache[cache_len - 4]; /* a(n-4) */
        val +=  (seq_t)231  * cache[cache_len - 5]; /* a(n-5) */
        val +=  (seq_t)1684 * cache[cache_len - 6]; /* a(n-6) */
        val +=  (seq_t)736  * cache[cache_len - 7]; /* a(n-7) */

        /* Grow array if necessary */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return 0; /* Allocation failure */
            cache = tmp;
        }
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n - 1];
}
