/*
 * A282415  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 469", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282415:
   a(n) = 111*a(n-2) - 1110*a(n-4) + 1000*a(n-6)  for n>5,
   with initial values:
   a(0)=1, a(1)=10, a(2)=11, a(3)=1100, a(4)=111, a(5)=111000. */

#include <stdlib.h>

/* small helper to compute powers of 10: 10^exp */
static seq_t pow10_u64(idx_t exp)
{
    seq_t r = 1;
    while (exp-- > 0)
        r = r * 10;
    return r;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (!cache) {
        /* Allocate space for the first 6 terms. */
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) exit(1);

        cache[0] = 1;
        cache[1] = 10;
        cache[2] = 11;
        cache[3] = 11 * pow10_u64(2);      /* 1100 */
        cache[4] = 111;
        cache[5] = 111 * pow10_u64(3);     /* 111000 */
        cache_size = 6;
    }

    /* If the requested index is beyond what we have computed, extend the cache. */
    if ((idx_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        for (size_t i = cache_size; i < new_size; ++i) {
            seq_t val = 111 * cache[i-2] - 1110 * cache[i-4] + 1000 * cache[i-6];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
