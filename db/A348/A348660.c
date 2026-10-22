/*
 * A348660  a(n) = Sum_{d|n, d <= sqrt(n)} (-1)^(n/d + 1) * d.
 * https://oeis.org/A348660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{d|n, d <= sqrt(n)} (-1)^(n/d + 1) * d
 * Range:     i8 n <= 863, i16 n <= 9666, i32 n <= 9666, i64 n <= 9666,
 *            i128 n <= 9666, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t int_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = (idx_t)1 << ((sizeof(idx_t) * 8 + 1) / 2);
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {
            lo = mid + 1;
            continue;
        }
        if (mid > n / mid) {         // mid*mid > n
            hi = mid - 1;
        } else {
            if (mid + 1 > n / (mid + 1)) {
                return mid;           // mid is floor(sqrt(n))
            }
            lo = mid + 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n == 0) return sum;          // n>=1 by definition, guard anyway
    idx_t sqrt_n = int_sqrt_u64(n);
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d != 0) continue;
        idx_t q = n / d;
        /* sign: +1 if q is odd, -1 if even */
        if (q & 1) {
            sum += (seq_t)d;
        } else {
            sum -= (seq_t)d;
        }
    }
    return sum;
}
