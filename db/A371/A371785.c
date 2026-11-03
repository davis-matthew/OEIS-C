/*
 * A371785  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(3*n-k,n-2*k).
 * https://oeis.org/A371785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(3*n-k, n-2*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; 2 * k <= n; ++k) {
        seq_t term = binomial(3 * n - k, n - 2 * k);
        if (k & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
