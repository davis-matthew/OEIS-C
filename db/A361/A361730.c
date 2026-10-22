/*
 * A361730  Diagonal of rational function 1/(1 - (1 + x*y*z) * (x^3 + y^3 +
 *          z^3)).
 * https://oeis.org/A361730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (3*k)!/(k!^3) * binomial(3*k,
 *            n-3*k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / 3;
    seq_t total = (seq_t)0;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t t1 = binomial(3 * k, k);          /* (3k)!/(k!*(2k)!)  */
        seq_t t2 = binomial(2 * k, k);          /* (2k)!/(k!*k!)      */
        seq_t t3 = binomial(3 * k, n - 3 * k);  /* binomial component */
        seq_t term = t1;
        term = term * t2;
        term = term * t3;
        total += term;
    }

    return total;
}
