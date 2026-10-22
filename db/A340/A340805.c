/*
 * A340805  a(n) is the number of solutions of the equation x*(y - 1) + (2*x -
 *          y - 1)*(x mod 2) = 2*n for 0 < x <= y.
 * https://oeis.org/A340805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} ([d <= floor((1 + sqrt(1 + 8*n))/4)] + * [d <=
 *            floor((sqrt(1 + 2*n) - 1)/2)])
 * Range:     u8 n <= 1591, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* Integer square‑root for idx_t using binary search (floor sqrt) */
static inline idx_t _isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Bound A = floor((1 + sqrt(1 + 8*n))/4) */
    idx_t sqrt1 = _isqrt_idx(1 + (idx_t)8 * n);
    idx_t A = (1 + sqrt1) / 4;

    /* Bound B = floor((sqrt(1 + 2*n) - 1)/2) */
    idx_t sqrt2 = _isqrt_idx(1 + (idx_t)2 * n);
    idx_t B = (sqrt2 - 1) / 2;

    seq_t total = 0;
    idx_t limit = _isqrt_idx(n);

    for (idx_t d1 = 1; d1 <= limit; ++d1) {
        if (n % d1 == 0) {
            idx_t d2 = n / d1;

            /* divisor d1 */
            if ((seq_t)d1 <= (seq_t)A) ++total;
            if ((seq_t)d1 <= (seq_t)B) ++total;

            if (d1 != d2) {          /* divisor d2 unique */
                if ((seq_t)d2 <= (seq_t)A) ++total;
                if ((seq_t)d2 <= (seq_t)B) ++total;
            }
        }
    }
    return total;
}
