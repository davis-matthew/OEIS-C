/*
 * A389151  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(4*k,n-2*k).
 * https://oeis.org/A389151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(4*k,
 *            n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        seq_t c1 = binomial(n, k);                       /* C(n,k)   */
        seq_t c2 = binomial((idx_t)(4 * k), n - 2 * k);   /* C(4k,n-2k) */
        sum += c1 * c2;
    }
    return sum;
}
