/*
 * A390456  a(n) = Sum_{k=0..n} binomial(3*n+3*k+2,n-k).
 * https://oeis.org/A390456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*n+3*k+2, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 990 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        sum += binomial(3 * n + 3 * k + 2, n - k);
    }
    return sum;
}
