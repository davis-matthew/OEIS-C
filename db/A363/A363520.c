/*
 * A363520  Product of the divisors of n that are < sqrt(n).
 * https://oeis.org/A363520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{d|n, d<sqrt(n)} d (implemented by iterating d up
 *            to floor(sqrt(n-1)) and multiplying the divisors encountered)
 * Range:     u8 n <= 59, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* For n >= 1, floor(sqrt(n-1)) yields the largest integer strictly
       less than sqrt(n).  (n = 1 gives 0, no divisors to multiply.) */
    idx_t limit = isqrt(n - 1);

    seq_t prod = 1;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0)
            prod = prod * (seq_t)d;
    }
    return prod;
}
