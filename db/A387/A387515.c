/*
 * A387515  a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) * binomial(k,n-2*k)^2.
 * https://oeis.org/A387515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 2^(n-2*k) * binomial(k, n-2*k)^2
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n/2; ++k) {
        idx_t m = n - (k << 1);          /* m = n - 2*k   */
        if (m > k) continue;             /* binomial = 0  */
        seq_t b    = binomial(k, m);     /* binomial(k,m) */
        seq_t term = pow2(m) * b * b;    /* 2^m * b^2     */
        sum += term;
    }
    return sum;
}
