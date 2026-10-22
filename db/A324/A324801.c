/*
 * A324801  Denominator of Sum_{k=1..n} 1/(k*(prime(k+1)-prime(k))).
 * https://oeis.org/A324801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Denominator of Sum_{k=1..n} 1/(k*(prime(k+1)-prime(k)))
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Obtain the first n+1 primes: primes[0] = 2, primes[1] = 3, … */
    uint32_t *primes = NULL;
    size_t primes_cnt = 0;
    sieve_first_n_primes(n + 1, &primes, &primes_cnt);

    /* Current fraction num/den – initially 0/1. */
    seq_t num = 0;
    seq_t den = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* term denominator: k * (prime(k+1) - prime(k)) */
        seq_t diff = (seq_t)(primes[k] - primes[k - 1]);
        seq_t term_den = (seq_t)k * diff;

        /* Use lcm to minimise intermediate sizes. */
        seq_t g1 = gcd(den, term_den);
        seq_t m1 = term_den / g1;         /* factor from term_den */
        seq_t m2 = den / g1;              /* factor from den */
        seq_t new_num = num * m1 + m2;    /* add fractions */
        seq_t new_den = m2 * term_den;    /* lcm of den and term_den */

        /* Reduce the fraction to lowest terms. */
        seq_t g2 = gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    free(primes);
    return den;   /* the denominator of the sum in lowest terms */
}
