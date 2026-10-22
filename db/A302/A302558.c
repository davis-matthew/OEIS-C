/*
 * A302558  For any n > 0 and m > 1, let d_m(n) be the distance from n to the
 *          nearest power of a number <= m (i.e., the distance to the nearest
 *          number of the form x^k with x <= m and k >= 0); a(n) = Sum_{i > 1}
 *          d_i(n).
 * https://oeis.org/A302558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i>1} d_i(n), where d_i(n) = |n - nearest power of a
 *            number ≤ i| (powers x^k with x ≤ i, k ≥ 0). For n = 1,2 the sum
 *            is 0
 * Range:     u8 n <= 53, u16 n <= 1559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute OEIS A302558.  Defined with external linkage.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)0;
    }

    /* Use 64‑bit integers for all intermediate calculations.
       They are safe for the problem limits (n up to ≈10,000 in timing tests). */
    uint64_t N      = (uint64_t)n;  /* copy n to a native type */
    uint64_t twoN   = N + N;        /* upper bound for powers that could be nearer than 1 */
    uint64_t min_dist = N + 1;      /* very large initial value */
    uint64_t sum = 0;

    /* Handle base 2 separately to initialise min_dist. */
    {
        uint64_t base  = 2;
        uint64_t power = 1;
        uint64_t best  = N + 1;   /* best distance for this base */

        for (;;) {
            uint64_t diff = (power > N) ? (power - N) : (N - power);
            if (diff < best) {
                best = diff;
            }
            if (power > twoN / base) {
                break;               /* next power would overflow the bound */
            }
            power *= base;            /* next power of base */
        }

        min_dist = best;
        sum += min_dist;
    }

    /* For bases 3 .. n-1, update the running minimum distance. */
    for (uint64_t base = 3; base <= N - 1; ++base) {
        uint64_t power = 1;
        uint64_t best  = N + 1;

        for (;;) {
            uint64_t diff = (power > N) ? (power - N) : (N - power);
            if (diff < best) {
                best = diff;
            }
            if (power > twoN / base) {
                break;
            }
            power *= base;
        }

        if (best < min_dist) {
            min_dist = best;           /* new closest power found */
        }
        sum += min_dist;                /* d_base(n) is the current minimum */
    }

    /* The final result may overflow seq_t for very large n, which is
       acceptable because the problem guarantees correctness only where
       the value fits in seq_t. */
    return (seq_t)sum;
}
