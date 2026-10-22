/*
 * A332049  a(n) = (1/2) * Sum_{d|n, d > 1} d * phi(d).
 * https://oeis.org/A332049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1}^{n-1} k / gcd(n,k)
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;  /* a(1) = 0 */

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t g = gcd((seq_t)n, (seq_t)k);
        idx_t q = k / (idx_t)g;   /* k and g are small enough for idx_t division */
        sum += (seq_t)q;
    }
    return sum;
}
