/*
 * A384973  Consecutive states of the linear congruential pseudo-random number
 *          generator (101*s+1) mod 2^13 when started at s=1.
 * https://oeis.org/A384973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (101*a(n-1) + 1) mod 2^13, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    const idx_t MOD = (idx_t)1 << 13;           /* 8192 */
    const idx_t MASK = MOD - (idx_t)1;          /* 8191 (for modulo 8192) */
    static seq_t *cache = NULL;
    static const idx_t CACHE_SIZE = MOD;        /* 8192 elements */

    /* Initialize cache on first call */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * CACHE_SIZE);
        if (!cache) { /* allocation failure, return 0 as fallback */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i < CACHE_SIZE; ++i) {
            cache[i] = ((seq_t)101 * cache[i - 1] + (seq_t)1) & (seq_t)MASK;
        }
    }

    /* Compute 0-based index modulo 8192 */
    idx_t idx = (n - (idx_t)1) & MASK;
    return cache[idx];
}
