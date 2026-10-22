/*
 * A384716  The totient of the product of unitary divisors of n.
 * https://oeis.org/A384716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2^(omega(n)-1)-1) * phi(n) (for n > 1) a(1) = 1 where
 *            omega(n) is the number of distinct prime factors of n
 * Range:     u8 n <= 25, u16 n <= 29, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   big_pow, count_distinct_primes, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)            /* a(1) = 1 */
        return (seq_t)1;

    /* Number of distinct prime factors of n */
    unsigned omega = (unsigned)count_distinct_primes((uint64_t)n);

    /* exp = 2^(omega-1) – 1 */
    size_t shift = omega - 1;
    seq_t exp = ((seq_t)1 << shift) - 1;

    /* n^exp */
    seq_t pow_part = big_pow((seq_t)n, exp);

    /* φ(n) */
    seq_t phi_part = phi((idx_t)n);

    return pow_part * phi_part;
}
