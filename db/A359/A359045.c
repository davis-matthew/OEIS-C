/*
 * A359045  a(n) = Sum_{1<=i<j<k<=n} b(i)*b(j)*b(k), where b(m) = A020985(m).
 * https://oeis.org/A359045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{1<=i<j<k<=n} b(i)*b(j)*b(k), b(m) = A020985(m);
 *            computed as running sums: a(n) = a(n-1) + b(n)*S2(n-1), S2(n) =
 *            S2(n-1) + b(n)*S1(n-1), S1(n) = S1(n-1) + b(n), all 0 at n = 0
 * Range:     i8 n <= 41, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A359045_b(idx_t n)
{
    int sign = 1;
    while (n > 1) {
        if ((n & 1) && ((n >> 1) & 1))
            sign = -sign;
        n >>= 1;
    }
    return sign;
}

seq_t a(idx_t n)
{
    seq_t S1 = 0, S2 = 0, total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (A359045_b(k) > 0) {
            total += S2;
            S2 += S1;
            S1 += 1;
        } else {
            total -= S2;
            S2 -= S1;
            S1 -= 1;
        }
    }
    return total;
}
