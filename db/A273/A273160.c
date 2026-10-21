/*
 * A273160  a(n) = Sum_{k=1..n} C(n, floor((n-k)/k)).
 * https://oeis.org/A273160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} C(n, floor((n-k)/k))
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = (n - k) / k;       /* floor((n-k)/k) */
        sum += binomial(n, m);
    }
    return sum;
}
