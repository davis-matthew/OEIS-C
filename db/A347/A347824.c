/*
 * A347824  Number of ways to write n as x^4 + y^4 + (z^2 + 23*w^2)/16, where
 *          x,y,z,w are nonnegative integers with x <= y.
 * https://oeis.org/A347824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A347824
 *  a(n) = number of representations of n as
 *  x^4 + y^4 + (z^2 + 23*w^2)/16,
 *  where x,y,z,w are non‑negative integers and x <= y.
 *
 *  The algorithm precomputes all values of
 *      t = (z^2 + 23*w^2)/16          (t <= n)
 *  together with their multiplicities.
 *  Then it scans all admissible (x,y) pairs
 *  (x^4 + y^4 <= n) and adds the appropriate
 *  multiplicity for the remaining part n - (x^4 + y^4).
 *
 *  This runs in O(N) time for a given n (the bound
 *  coming from the quadratic loops) and
 *  uses O(n) auxiliary memory.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 1. Pre‑compute all admissible t-values and their counts   */
    seq_t *t_counts = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!t_counts) return 0;           /* allocation failed – unlikely for small n   */
    memset(t_counts, 0, (n + 1) * sizeof(seq_t));

    const idx_t limit = 16 * n;        /* upper bound for z^2 + 23*w^2            */

    for (idx_t w = 0; ; ++w) {
        const idx_t wsq = 23 * w * w;
        if (wsq > limit) break;
        const idx_t rem = limit - wsq;            /* remaining part for z^2                */
        for (idx_t z = 0; ; ++z) {
            const idx_t zsq = z * z;
            if (zsq > rem) break;
            const idx_t numerator = zsq + wsq;
            if (numerator % 16 == 0) {
                const idx_t t = numerator / 16;
                if (t <= n) t_counts[(size_t)t]++; /* use array index safely               */
            }
        }
    }

    /* 2. Count all (x,y) pairs with x <= y and x^4 + y^4 <= n */
    seq_t result = 0;

    for (idx_t x = 0; ; ++x) {
        const seq_t x4 = ((seq_t)x * x * x * x);
        if (x4 > (seq_t)n) break;
        for (idx_t y = x; ; ++y) {
            const seq_t y4 = ((seq_t)y * y * y * y);
            const seq_t s   = x4 + y4;
            if (s > (seq_t)n) break;
            const seq_t need = (seq_t)n - s;
            if (need <= (seq_t)n)
                result += t_counts[(size_t)need];
        }
    }

    free(t_counts);
    return result;
}
