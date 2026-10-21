/*
 * A276356  Number of Hamiltonian cycles in the Cartesian product graph K_2
 *          times K_n.
 * https://oeis.org/A276356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} binomial(n,2k) * (2k-1)! *
 *            ((n-k-1)!/(k-1)!)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t maxk = n / 2;
    seq_t res = (seq_t)0;

    /* r = (n-2)!  (for k=1) */
    seq_t r = (seq_t)1;
    for (idx_t i = 2; i <= n - 2; ++i)
        r *= (seq_t)i;

    /* factOdd = (2k-1)!  (starts with 1! for k=1) */
    seq_t factOdd = (seq_t)1;

    for (idx_t k = 1; k <= maxk; ++k)
    {
        seq_t term = binomial(n, 2 * k);
        term *= factOdd;          /* multiply by (2k-1)! */
        seq_t r2 = r;
        r2 *= r;                  /* r^2 */
        term *= r2;

        res += term;

        if (k < maxk) {
            /* update (2k+1)! from (2k-1)! */
            factOdd *= (seq_t)(2 * k);
            factOdd *= (seq_t)(2 * k + 1);

            /* update r: r_{k+1} = r_k / ((n-k-1)*k) */
            seq_t denom = (seq_t)((n - k - 1) * k);
            r /= denom;           /* exact division */
        }
    }

    return res;
}
