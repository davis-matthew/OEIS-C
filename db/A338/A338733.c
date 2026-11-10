/*
 * A338733  Partial sums of A054843.
 * https://oeis.org/A338733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{odd d<=n} floor(n/d) + #{t>=1 : t(t+1)/2 <= n}
 *            (partial sums of A054843, where A054843(m) = A001227(m) + [m
 *            triangular] for m>=1, A054843(0)=1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 1;
    for (idx_t d = 1; d <= n; d += 2) {
        total += (seq_t)(n / d);
    }
    /* count triangular numbers T(t) = t(t+1)/2 with 1 <= T(t) <= n */
    idx_t t = 0;
    while (((unsigned __int128)(t + 1) * (t + 2)) / 2 <= (unsigned __int128)n) {
        ++t;
    }
    total += (seq_t)t;
    return total;
}
