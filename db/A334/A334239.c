/*
 * A334239  Number of r X s rectangles with composite side lengths such that r
 *          + s = 2n, r <= s and r | s.
 * https://oeis.org/A334239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ r | 4 <= r <= n, r composite, 2n % r == 0, (2n - r)
 *            composite }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x is a composite integer (i.e., >1, not prime). */
static inline bool is_composite(idx_t x) {
    if (x < 4) return false;            /* 0,1,2,3 are not composite */
    for (idx_t d = 2; d <= x / d; ++d) {
        if (x % d == 0) return true;    /* found a divisor -> composite */
    }
    return false;                       /* no divisor -> prime */
}

seq_t a(idx_t n) {
    if (n < 4) return (seq_t)0;          /* no possible rectangles */
    idx_t two_n = n << 1;                /* 2*n */
    seq_t count = (seq_t)0;              /* result accumulator */

    for (idx_t r = 4; r <= n; ++r) {
        if (two_n % r != 0) continue;    /* r must divide 2n */
        if (!is_composite(r)) continue;  /* r must be composite */
        idx_t s = two_n - r;
        if (!is_composite(s)) continue;  /* s = 2n - r must be composite */
        ++count;                         /* valid rectangle found */
    }
    return count;
}
