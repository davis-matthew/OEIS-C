/*
 * A356343  a(n) = Sum_{k=1..n} binomial(2*n, k) * phi(k), where phi is the
 *          Euler totient function.
 * https://oeis.org/A356343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n, k) * phi(k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t
a (idx_t n)
{
    /* n >= 1 for the OEIS offset; return 0 for n == 0 to keep the
       function safe when called with an out‑of‑range index. */
    if (n == 0)
        return (seq_t)0;

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;   /* binomial(2*n,0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial(2*n,k) = binomial(2*n,k-1) * (2*n - k + 1) / k */
        binom = binom * ((seq_t)(2 * n - k + 1)) / (seq_t)k;
        sum += binom * phi(k);
    }

    return sum;
}
