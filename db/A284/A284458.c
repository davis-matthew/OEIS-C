/*
 * A284458  Number of pairs (f,g) of endofunctions on [n] such that the
 *          composite function gf has no fixed point.
 * https://oeis.org/A284458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n,k)^2 * k! * n^(2*(n-k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t sn = (seq_t)n;

    /* Square of n for dividing the power term */
    seq_t n_sq = sn * sn;

    /* Initial power term n^(2*n) */
    int exp = (int)(2 * n);
    seq_t pow_n = pow_seq(sn, exp);

    seq_t binom = 1; /* C(n,0) */
    seq_t fact  = 1; /* 0! */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = n^(2*(n-k)) * k! * C(n,k)^2 */
        seq_t term = pow_n * fact;
        term *= binom;
        term *= binom;

        if (k & 1)
            result -= term;
        else
            result += term;

        if (k == n) break;

        /* Update binomial coefficient for next k */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update factorial for next k */
        fact *= (seq_t)(k + 1);

        /* Update power term: divide by n^2 */
        pow_n /= n_sq;
    }

    return result;
}
