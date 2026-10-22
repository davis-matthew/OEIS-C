/*
 * A398190  a(n) = exp(2 * n) * Gamma(1 + n, 2 * n). Central terms of A398187.
 * https://oeis.org/A398190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{j=0..n} (2*n)^j / j!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t term = factorial(n);
    seq_t sum  = term;
    seq_t two_n = (seq_t)2 * (seq_t)n;

    for (idx_t j = 1; j <= n; ++j)
    {
        /* term_{j} = (term_{j-1} / j) * (2*n)  – division is exact */
        term = (term / (seq_t)j) * two_n;
        sum += term;
    }

    return sum;
}
