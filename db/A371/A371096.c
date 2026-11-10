/*
 * A371096  Array A read by upward antidiagonals in which the entry A(n,k) in
 *          row n and column k is defined by A(1, k) = 8*k-7, and A(n+1, k) =
 *          A371094(A(n, k)), n,k >= 1.
 * https://oeis.org/A371096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A(1,k) = 8*k-7, A(n+1,k) = A371094(A(n,k)) with A371094(x) =
 *            m*2^e + (4^e-1)/3, m = 3x+1, e = v2(m); read by upward
 *            antidiagonals, T(d,i): k=i, n=d-i+1
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
    seq_t t = m;
    seq_t p2 = 1, s = 0;
    if (t == 0) return 0;
    while ((t & 1) == 0) {
        t >>= 1;
        p2 = p2 * 2;
        s = s * 4 + 1;      /* (4^e-1)/3 */
    }
    return m * p2 + s;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) d++;
    idx_t i = n - d * (d - 1) / 2;   /* 1..d, column k */
    idx_t row = d - i + 1;
    seq_t v = 8 * (seq_t)i - 7;
    for (idx_t r = 1; r < row; r++) v = A371094_a(v);
    return v;
}
