/*
 * A363366  Antidiagonal sums of A363365.
 * https://oeis.org/A363366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i+j=n+1, i,j>=1} A(i,j), where A(1,k) = k and A(m,k)
 *            = (k+1)*A(m-1,k) + k + 1 - m (A363365)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    if (n < 1)
        return sum;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t rows = n + 1 - k;
        seq_t v = (seq_t)k;
        seq_t kp1 = (seq_t)k + 1;
        for (idx_t m = 2; m <= rows; ++m) {
            v = kp1 * v + kp1;
            v = v - (seq_t)m;
        }
        sum += v;
    }
    return sum;
}
