/*
 * A364360  a(n) = dpf(n) ^ tpf(n), where dpf(n) is the number of distinct
 *          prime factors of n if n >= 2 and otherwise = 0; tpf(n) is the
 *          number of all prime factors of n if n >= 2 and otherwise = 0.
 * https://oeis.org/A364360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = dpf(n)^tpf(n)
 * Range:     u8 n <= 209, u16 n <= 419, u32 n <= 419, u64 n <= 419,
 *            u128 n <= 419, big any n
 * Helpers:   big_pow, count_distinct_primes, count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  dpf(n) = number of distinct prime factors of n (if n>=2, else 0)  */
/*  tpf(n) = total number of prime factors of n with multiplicity (if n>=2, else 0)  */

#include <stdint.h>                 /* for uint64_t */
#include "../utils/count_distinct_primes.h"
#include "../utils/count_prime_factors.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* OEIS offset 0, base cases */
    if (n < 2)                /* n = 0 or 1 -> a(n) = 1 */
        return (seq_t)1;

    /* number of distinct prime factors of n */
    seq_t d = (seq_t)count_distinct_primes((uint64_t)n);

    /* number of prime factors of n (with multiplicity) */
    seq_t t = count_prime_factors((seq_t)n);

    /* compute d ^ t */
    return big_pow(d, t);
}
