/*
 * A321547  a(n) = Sum_{d|n} (-1)^(d-1)*d^8.
 * https://oeis.org/A321547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1)*d^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 14, i64 n <= 234,
 *            i128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation iterates over all divisors d of n (using the
 * symmetry d ↔ n/d) and accumulates the signed eighth powers.
 * It uses the helper pow_uint from the utilities for exact integer
 * exponentiation.  The algorithm runs in O(sqrt(n)) time and requires
 * only constant extra space. */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* 0 is outside the offset but handled safely */

    seq_t sum = 0;

    /* iterate d from 1 to sqrt(n); use n/d to avoid overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* first divisor d */
        seq_t term = pow_uint(d, 8);
        if ((d & 1ULL) == 0) term = -term;
        sum += term;

        /* second divisor e = n/d, skip if same as d */
        idx_t e = n / d;
        if (e != d) {
            seq_t term2 = pow_uint(e, 8);
            if ((e & 1ULL) == 0) term2 = -term2;
            sum += term2;
        }
    }

    return sum;
}
