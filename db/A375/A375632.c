/*
 * A375632  Expansion of e.g.f. exp(2*x) / (1 - x^3/6 * exp(x)).
 * https://oeis.org/A375632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (k+2)^(n-3*k)/(6^k*(n-3*k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   pow_seq_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_exp.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* highest k such that 3k <= n */
    idx_t maxk = n / 3U;

    /* fact holds n!/(n-3k)!/6^k for the current k */
    seq_t fact = 1;
    seq_t total = 0;

    for (idx_t k = 0U; k <= maxk; ++k)
    {
        idx_t exp   = n - 3U * k;           /* power exponent */
        seq_t pow   = pow_seq_exp((seq_t)(k + 2U), exp);
        seq_t term  = fact * pow;
        total += term;

        /* Prepare fact for next k (k+1). */
        if (k < maxk)
        {
            idx_t a = n - 3U * k;       /* first new factor   */
            idx_t b = a - 1U;           /* second new factor  */
            idx_t c = a - 2U;           /* third  new factor  */

            /* fact = fact * a * b * c / 6. */
            fact = fact * (seq_t)a * (seq_t)b * (seq_t)c / 6U;
        }
    }

    return total;
}
