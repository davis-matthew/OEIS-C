/*
 * A268058  Maximum value of n-th row of A268057.
 * https://oeis.org/A268058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1<=k<=n} T(n,k), where T(n,k) is the number of steps
 *            of k -> n mod k needed to reach 0 (computed by direct iteration)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8764 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t best = 0;
    idx_t k;
    for (k = 1; k <= n; k++) {
        idx_t x = k;
        idx_t cnt = 0;
        while (x != 0) {
            x = n % x;
            cnt++;
        }
        if (cnt > best)
            best = cnt;
    }
    return (seq_t)best;
}
