/*
 * A371102  Array A read by upward antidiagonals in which the entry A(n,k) in
 *          row n and column k is defined by A(1, k) = 4*k-1, and A(n+1, k) =
 *          A371094(A(n, k)), n,k >= 1.
 * https://oeis.org/A371102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A(1,k) = 4k-1, A(n+1,k) = A371094(A(n,k)), where A371094(x) =
 *            m*2^e + (4^e-1)/3, m = 3x+1, e = v2(m); read by upward
 *            antidiagonals
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A371094_a(seq_t x)
{
    seq_t m = 3 * x + 1;
    if (m == 0) return 0;
    int e = 0;
    seq_t t = m;
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
    idx_t d = 1;
    idx_t start = 0; /* number of terms before antidiagonal d */
    while (start + d < n) {
        start += d;
        d++;
    }
    idx_t i = n - start;      /* 1..d, column */
    idx_t row = d - i + 1;    /* row index */
    seq_t v = 4 * (seq_t)i - 1;
    for (idx_t r = 1; r < row; r++) {
        v = A371094_a(v);
    }
    return v;
}
