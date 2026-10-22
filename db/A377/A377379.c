/*
 * A377379  a(n) = rad(n)^binomial(bigomega(n) + omega(n), omega(n) + 1),
 *          where rad = A007947, bigomega = A001222, and omega = A001221.
 * https://oeis.org/A377379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(n)^binomial(bigomega(n) + omega(n), omega(n) + 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   big_pow, bigomega, binomial, count_distinct_primes, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/rad.h"
#include "../utils/bigomega.h"
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* radical of n */
    seq_t rad_val = rad(n);

    /* number of distinct prime factors (omega) */
    idx_t omega = (idx_t)count_distinct_primes(n);

    /* total number of prime factors counted with multiplicity (bigomega) */
    seq_t bigomega_val = bigomega(n);
    idx_t bigomega = (idx_t)bigomega_val;

    /* compute the binomial exponent */
    seq_t exp_binom = binomial(bigomega + omega, omega + 1);

    /* compute the result */
    seq_t result = big_pow(rad_val, exp_binom);

    return result;
}
