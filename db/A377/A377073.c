/*
 * A377073  a(n) = rad(n)^binomial(bigomega(n) + omega(n) - 1, omega(n)),
 *          where rad = A007947, bigomega = A001222, and omega = A001221.
 * https://oeis.org/A377073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   big_pow, bigomega, binomial, count_distinct_primes, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = rad(n)^binomial(bigomega(n) + omega(n) - 1, omega(n))
 * with a(1) = 1 (special case, since the binomial would be undefined).
 */
#include <stdint.h>
#include "../utils/rad.h"
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Special case: a(1) = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* total number of prime factors with multiplicity (bigomega) */
    idx_t bigomega_val = (idx_t)bigomega(n);

    /* number of distinct prime factors (omega) */
    idx_t omega_val = (idx_t)count_distinct_primes((uint64_t)n);

    /* k = bigomega + omega - 1 */
    idx_t k = bigomega_val + omega_val - 1;

    /* binomial coefficient as seq_t */
    seq_t bin = binomial(k, omega_val);

    /* product of distinct prime factors */
    seq_t rad_val = rad(n);

    /* rad(n) ^ bin */
    return big_pow(rad_val, bin);
}
