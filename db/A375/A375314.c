/*
 * A375314  a(n) = Sum_{k=0..floor(n/2)} binomial(4*k,n-2*k).
 * https://oeis.org/A375314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(4*k, n-2*k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k)
    {
        seq_t term = binomial(4 * k, n - 2 * k);
        sum += term;
    }
    return sum;
}
