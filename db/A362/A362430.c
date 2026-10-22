/*
 * A362430  E.g.f. satisfies A(x) = exp(x - x^3 * A(x)).
 * https://oeis.org/A362430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (-1)^k * (k+1)^(n-2k-1) / (k! *
 *            (n-3k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;               /* a(0) = 1 */

    const idx_t m = n / 3;                      /* floor(n/3) */
    seq_t sum = 0;
    seq_t cur_binom = 1;                        /* C(n, 0) */
    seq_t cur_fact3 = 1;                        /* (3*k)!  for k=0   */
    seq_t cur_factk = 1;                        /* k!      for k=0   */

    for (idx_t k = 0; k <= m; ++k)
    {
        /* d = n - 2*k - 1 is non‑negative for all k except n = 0,
           which we handled above. */
        const idx_t d = n - 2*k - 1;

        seq_t pow_term;
        if (k == 0) {
            /* (k+1) = 1, so 1^d = 1 for all d */
            pow_term = 1;
        } else {
            pow_term = big_pow((seq_t)(k + 1), (seq_t)d);
        }

        /* prod = (3*k)! / k!  (product from k+1 to 3*k) */
        seq_t prod = cur_fact3 / cur_factk;

        /* term = (-1)^k * pow_term * cur_binom * prod */
        seq_t term = pow_term * cur_binom * prod;
        if (k & 1) term = -term;
        sum += term;

        if (k == m) break;  /* no more terms needed */

        /* Update cur_binom from C(n, 3*k) to C(n, 3*(k+1)) */
        const idx_t r = 3 * k;            /* current r = 3*k */

        /* C(n, r+1) = C(n, r) * (n-r)/(r+1) */
        cur_binom *= (seq_t)(n - r);
        cur_binom /= (seq_t)(r + 1);

        /* C(n, r+2) */
        cur_binom *= (seq_t)(n - r - 1);
        cur_binom /= (seq_t)(r + 2);

        /* C(n, r+3) */
        cur_binom *= (seq_t)(n - r - 2);
        cur_binom /= (seq_t)(r + 3);

        /* Update factorials for next k */
        cur_fact3 *= (seq_t)(3*k + 1);
        cur_fact3 *= (seq_t)(3*k + 2);
        cur_fact3 *= (seq_t)(3*k + 3);

        cur_factk *= (seq_t)(k + 1);
    }

    return sum;
}
