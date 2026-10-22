/*
 * A384615  Consecutive states of the linear congruential pseudo-random number
 *          generator for GLIM3 when started at 1.
 * https://oeis.org/A384615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8404997*a(n-1)+1) mod 2^35, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    if (n < 1) return 0;  /* safe guard for out-of-range, not used */

    if (n <= cache_len)
        return cache[n-1];

    /* increase cache size to hold up to n */
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * n);
    if (!tmp) abort();  /* memory allocation failed */
    cache = tmp;

    /* initialise the first element if needed */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    seq_t last = cache[cache_len-1];
    const seq_t M   = (seq_t)34359738368ULL;  /* 2^35 */
    const seq_t MUL = (seq_t)8404997;
    const seq_t INC = (seq_t)1;

    for (idx_t i = cache_len; i < n; ++i) {
        last = ((last * MUL + INC) % M);
        cache[i] = last;
    }
    cache_len = n;
    return cache[n-1];
}
