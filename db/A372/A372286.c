/*
 * A372286  a(n) = number of terms of A086893 that are in the interval [n,
 *          A371094(n)].
 * https://oeis.org/A372286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : n <= A086893(k) <= A371094(n)}, where A086893 = 1,
 *            3, then t(k+2) = 4*t(k)+1, and A371094(n) = m*2^e + (4^e-1)/3
 *            with m = 3n+1 and e = v2(m). The code finds the count by direct
 *            enumeration
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 21844, u64 n <= 21845,
 *            u128 n <= 21845, big any n
 * Validated: standard, pass, 122 terms through n = 9043 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A371094_a(idx_t n)
{
    seq_t m = 3 * (seq_t)n + 1;
    seq_t e = 0;
    seq_t t = m;
    while ((t & 1) == 0) {
        e++;
        t >>= 1;
    }
    seq_t pow2e = 1;
    for (seq_t i = 0; i < e; i++) pow2e *= 2;
    seq_t pow4e = pow2e * pow2e;
    return m * pow2e + (pow4e - 1) / 3;
}

seq_t a(idx_t n)
{
    seq_t B = A371094_a(n);
    seq_t lo = (seq_t)n;
    seq_t x = 1, y = 3;
    seq_t cnt = 0;
    while (x <= B) {
        if (x >= lo) cnt++;
        seq_t ny;
        if (x > B / 4) ny = B + 1;
        else ny = 4 * x + 1;
        x = y;
        y = ny;
    }
    return cnt;
}
