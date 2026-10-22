/*
 * A398291  Diagonal of rational function 1/(1 - (1 + x*y*z)*(x*(1 + x*y*z) +
 *          y + z)).
 * https://oeis.org/A398291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (3*k)!/(k!)^3 * binomial(4*k, n–k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* declares seq_t binom(idx_t, idx_t) */

/* Compute the n‑th term of OEIS A398291.  The calculation is carried
 * out in seq_t arithmetic, allowing arbitrary precision when the
 * build uses a big‑integer type. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* (3k)!/(k!^3) = binom(3k,k) * binom(2k,k) */
        seq_t t1 = binom(3 * k, k);
        seq_t t2 = binom(2 * k, k);
        /* binomial(4k, n-k) */
        seq_t t3 = binom(4 * k, n - k);

        /* term = t1 * t2 * t3 */
        seq_t term = t1 * t2 * t3;

        sum += term;
    }
    return sum;
}
