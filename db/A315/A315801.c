/*
 * A315801  Coordination sequence Gal.3.43.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-6) - a(n-8) for n>10 Initial values: a(0)=1,
 *            a(1)=6, a(2)=6, a(3)=12, a(4)=24, a(5)=24, a(6)=24, a(7)=42,
 *            a(8)=36, a(9)=48, a(10)=48
 * Range:     u8 n <= 42, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t last_computed = (idx_t)-1;

/* Compute the nth term of OEIS A315801 */
seq_t a(idx_t n)
{
    /* constant initial values */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)6, (seq_t)6, (seq_t)12, (seq_t)24,
        (seq_t)24, (seq_t)24, (seq_t)42, (seq_t)36, (seq_t)48, (seq_t)48
    };

    /* first call: allocate cache and copy initial values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(11 * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        for (size_t i = 0; i < 11; ++i) cache[i] = init[i];
        last_computed = 10;
    }

    /* trivial cases */
    if (n <= 10) return cache[n];

    /* extend cache if necessary */
    if (n > last_computed) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;

        for (size_t i = (size_t)(last_computed + 1); i <= (size_t)n; ++i) {
            cache[i] = cache[i - 2] + cache[i - 6] - cache[i - 8];
        }
        last_computed = n;
    }

    return cache[n];
}
