/*
 * A269962  Start with a square; at each stage add a square at each expandable
 *          vertex so that the ratio of the side of the squares at stage n+1
 *          and at stage n is the golden ratio phi=0.618...; a(n) is the
 *          number of squares at n-th stage.
 * https://oeis.org/A269962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A269963(n)+2*A269963(n-1)-1;
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 52, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n≥7: a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) + 2*a(n-4) - 2*a(n-5); */
/* Initial values: a(1)=1, a(2)=5, a(3)=17, a(4)=45, a(5)=105, a(6)=237 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_upto = 0;

    /* Grow cache so that indices 1..n are valid */
    if (n + 1 > cache_size) {
        size_t new_cap = cache_size == 0 ? 8 : cache_size;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cache_size = new_cap;
    }

    /* Initialise base values once */
    if (computed_upto < 6) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)45;
        cache[5] = (seq_t)105;
        cache[6] = (seq_t)237;
        computed_upto = 6;
    }

    /* Compute further terms using the homogeneous recurrence for i≥7 */
    for (idx_t i = (idx_t)(computed_upto + 1); i <= n; ++i) {
        if (i < 7) continue;  /* base values handled already */

        /* pos = 4*a(i-1) + 2*a(i-3) + 2*a(i-4) */
        seq_t pos = 4 * cache[i - 1] + 2 * cache[i - 3] + 2 * cache[i - 4];
        /* neg = 5*a(i-2) + 2*a(i-5) */
        seq_t neg = 5 * cache[i - 2] + 2 * cache[i - 5];
        cache[i] = pos - neg;  /* pos ≥ neg for this sequence */
    }
    if (computed_upto < n) computed_upto = n;

    return cache[n];
}
