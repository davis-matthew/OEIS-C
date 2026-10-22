/*
 * A308278  Take all the integer-sided triangles with perimeter n and square
 *          number sides a, b, and c such that a <= b <= c. a(n) is the sum of
 *          all the b's.
 * https://oeis.org/A308278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum of the second side b over all integer‑sided triangles
   whose side lengths a ≤ b ≤ c are perfect squares and a + b + c = n.
   The function enumerates all triples (a,b,c) of squares with this
   perimeter and adds b in each case. */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned value, without floating point. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = (n >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid != 0 && mid > n / mid)  /* mid*mid > n ? */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n < 3)               /* no triangle with perimeter 1 or 2 */
        return 0;

    idx_t sqmax = isqrt_u64(n);   /* maximal square side ≤ n */
    if (sqmax == 0)
        return 0;

    /* Allocate array of squares 1²,2²,…,sqmax². */
    idx_t *sq = (idx_t *)malloc(sqmax * sizeof(idx_t));
    if (!sq)
        return 0;              /* fallback on allocation failure */

    for (idx_t i = 1; i <= sqmax; ++i)
        sq[i - 1] = i * i;

    seq_t sum = 0;

    for (idx_t i = 0; i < sqmax; ++i) {
        idx_t a_val = sq[i];
        for (idx_t j = i; j < sqmax; ++j) {
            idx_t b_val = sq[j];
            idx_t ab = a_val + b_val;
            if (ab > n)               /* further b will only increase ab */
                break;

            idx_t c_val = n - ab;
            if (c_val < b_val)        /* enforce b ≤ c */
                continue;

            /* Verify that c is a perfect square. */
            idx_t sqrt_c = isqrt_u64(c_val);
            if (sqrt_c * sqrt_c != c_val)
                continue;

            /* Triangle inequality: a + b > c.  The other two are
               automatically true because a ≤ b ≤ c. */
            if (ab <= c_val)
                continue;

            sum += (seq_t)b_val;
        }
    }

    free(sq);
    return sum;
}
