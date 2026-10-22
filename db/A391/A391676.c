/*
 * A391676  a(n) = Sum_{k=0..n} 2^k * binomial(k+2,2) * binomial(k,n-k)^2.
 * https://oeis.org/A391676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(k+2,2) * binomial(k,n-k)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t max_j = n >> 1;          /* floor(n/2) */

    for (idx_t j = 0; j <= max_j; ++j) {
        idx_t k = n - j;            /* k = n - j, so that j = n - k */
        seq_t c   = binom(k, j);    /* C(k, j) = C(k, n-k) */

        seq_t term = pow2(k);       /* 2^k */

        /* multiply by binomial(k+2,2) = (k+2)(k+1)/2 */
        term *= ((seq_t)(k + 2) * (k + 1)) / 2;

        /* multiply by c^2 */
        term *= c;
        term *= c;

        total += term;
    }

    return total;
}
