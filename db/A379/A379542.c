/*
 * A379542  Second term of the n-th differences of the prime numbers.
 * https://oeis.org/A379542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} (-1)^(n-k) * binomial(n,k) * prime(k+2)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 29, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (prime(2)=3, prime(3)=5, …, where prime(k+2) is the (k+1)-st odd prime) */

#include <stdint.h>                     /* for uint32_t */
#include "../utils/get_prime.h"          /* provides uint32_t get_prime(idx_t) */

seq_t a(idx_t n)
{
    seq_t sum   = 0;                /* running total */
    seq_t binom = 1;                /* C(n,0) = 1 */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* prime(k+2): skip the first prime (2) to get the odd primes */
        seq_t prime = (seq_t)get_prime(k + 2);

        /* term = C(n,k) * prime(k+2) */
        seq_t term = binom * prime;

        /* add or subtract depending on parity of (n-k) */
        if (((n - k) & 1) == 0)
            sum += term;
        else
            sum -= term;

        /* update binomial coefficient for next k, unless we are at the end */
        if (k < n)
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return sum;
}
