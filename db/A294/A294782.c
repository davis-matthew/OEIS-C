/*
 * A294782  Spherical growth of the Lamplighter group: number of elements in
 *          the Lamplighter group Z wr Z of length n with respect to the
 *          standard generating set {a,t}.
 * https://oeis.org/A294782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294782: a(n) satisfies an order‑8 linear recurrence
 * derived from the generating function:
 *   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - 4*a(n-4)
 *             + 4*a(n-5) + 6*a(n-6) + 4*a(n-7) + a(n-8)   for n ≥ 9
 * The first eight terms are given by the OEIS entry and
 * a(8) is obtained from the relation
 *   a(8) = 4*a(7) - 2*a(6) - 4*a(5) - 4*a(4)
 *            + 4*a(3) + 6*a(2) + 4*a(1) + a(0) - 1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values a(0)…a(7) */
    static const seq_t base[8] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)12,
        (seq_t)36,
        (seq_t)100,
        (seq_t)268,
        (seq_t)704,
        (seq_t)1812
    };

    static seq_t *cache = NULL;
    static idx_t cachelen = 0;

    /* Constant coefficients as seq_t */
    const seq_t c1 = (seq_t)1;
    const seq_t c2 = (seq_t)2;
    const seq_t c4 = (seq_t)4;
    const seq_t c6 = (seq_t)6;

    if (n < 8)
        return base[(size_t)n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        idx_t init = 9;                /* hold indices 0…8 */
        cache = (seq_t *)malloc((size_t)init * sizeof(seq_t));
        if (cache == NULL) abort();    /* OOM */

        for (size_t i = 0; i < 8; ++i)
            cache[i] = base[i];

        /* Compute a(8) from the special formula */
        seq_t val = c4 * cache[7];
        val -= c2 * cache[6];
        val -= c4 * cache[5];
        val -= c4 * cache[4];
        val += c4 * cache[3];
        val += c6 * cache[2];
        val += c4 * cache[1];
        val += cache[0];
        val -= c1;
        cache[8] = val;
        cachelen = 9;
    }

    /* Return if already computed */
    if (n < cachelen)
        return cache[(size_t)n];

    /* Extend cache to include n */
    idx_t newlen = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)newlen * sizeof(seq_t));
    if (tmp == NULL) abort();          /* OOM */
    cache = tmp;

    for (idx_t i = cachelen; i <= n; ++i) {
        seq_t val = c4 * cache[i-1];
        val -= c2 * cache[i-2];
        val -= c4 * cache[i-3];
        val -= c4 * cache[i-4];
        val += c4 * cache[i-5];
        val += c6 * cache[i-6];
        val += c4 * cache[i-7];
        val += cache[i-8];
        cache[i] = val;
    }
    cachelen = newlen;
    return cache[(size_t)n];
}
