/*
 * A297795  a(n) = Sum_{d|n} min(d, n/d)^5.
 * https://oeis.org/A297795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} min(d,n/d)^5
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 4535, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n >= 1 using a simple divisor enumeration. */
#include <stdbool.h>

/* Integer square‑root (floor) for idx_t values.
   Uses only integer arithmetic, compatible with C11 and C++17. */
static idx_t isqrt(idx_t x)
{
    idx_t low = 0, high = x, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid <= x / mid) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t sq = isqrt(n);
    seq_t sum = 0;

    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            seq_t d_seq = d;
            seq_t d2 = d_seq * d_seq;
            seq_t d3 = d2 * d_seq;
            seq_t d4 = d3 * d_seq;
            seq_t d5 = d4 * d_seq;

            if (d == e) {
                sum += d5;
            } else {
                sum += (seq_t)2 * d5;
            }
        }
    }
    return sum;
}
