/*
 * A353048  a(n) = (number of primes p in the first n primes such that 2*p+1
 *          is also prime) - (number of primes p in the first n primes such
 *          that 2*p-1 is also prime).
 * https://oeis.org/A353048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (# of primes p among the first n primes such that 2p+1 is
 *            prime) – (# of primes p among the first n primes such that 2p-1
 *            is prime)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for free */
#include <stdbool.h>  /* for bool */
#include "../utils/sieve_first_n_primes.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    size_t count = 0;
    uint32_t *primes = NULL;

    /* Obtain the first n primes.  The routine allocates the array. */
    sieve_first_n_primes(n, &primes, &count);

    seq_t plus = 0, minus = 0;
    for (idx_t i = 0; i < count; ++i) {
        idx_t p = (idx_t)primes[i];
        idx_t lp1 = 2 * p + 1;   /* 2p+1 */
        idx_t lm1 = 2 * p - 1;   /* 2p-1 */

        if (is_prime(lp1)) plus++;
        if (is_prime(lm1)) minus++;
    }

    free(primes);
    return plus - minus;
}
