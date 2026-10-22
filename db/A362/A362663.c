/*
 * A362663  a(n) is the partial sum of b(n), which is defined to be the
 *          difference between the numbers of primes in (n^2, n^2 + n] and in
 *          (n^2 - n, n^2].
 * https://oeis.org/A362663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1..n} (primepi(i^2+i) + primepi(i^2-i) -
 *            2*primepi(i^2))
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined for n<1, return 0 to avoid crashes */
        return (seq_t)0;

    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* squares and squares ± i */
        idx_t sq   = (idx_t)i * (idx_t)i;      /* i^2  */
        seq_t pi_sq     = prime_pi(sq);        /* π(i^2)   */
        seq_t pi_sq_plus  = prime_pi(sq + i);  /* π(i^2+i) */
        seq_t pi_sq_minus = prime_pi(sq - i);  /* π(i^2-i) */

        seq_t delta = pi_sq_plus + pi_sq_minus - 2 * pi_sq;
        total += delta;
    }
    return total;
}
