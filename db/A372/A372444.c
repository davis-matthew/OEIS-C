/*
 * A372444  The n-th iterate of 27 with A371094.
 * https://oeis.org/A372444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 27; a(n) = A371094(a(n-1)), where A371094(k) = m*2^e +
 *            (4^e-1)/3, m = 3k+1, e = v2(m)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 11 terms through n = 10 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A371094_a(seq_t k)
{
    seq_t m = 3 * k + 1;
    int e = 0;
    seq_t t = m;
    if (t == 0) return 0;                  /* only reachable after overflow wrap */
    while ((t & 1) == 0) {
        e++;
        t >>= 1;
    }
    seq_t pow2e = ((seq_t)1) << e;
    seq_t pow4e = pow2e * pow2e;
    return m * pow2e + (pow4e - 1) / 3;
}

seq_t a(idx_t n)
{
    seq_t x = 27;
    for (idx_t i = 0; i < n; i++) {
        x = A371094_a(x);
    }
    return x;
}
