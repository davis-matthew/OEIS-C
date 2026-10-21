/*
 * A251364  Difference between average of two consecutive odd primes and the
 *          sum of all prime factors of the average.
 * https://oeis.org/A251364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n+1)+prime(n+2))/2) -
 *            sopfr((prime(n+1)+prime(n+2))/2) where sopfr(x) is the sum of
 *            prime factors of x counted with multiplicity
 * Range:     u8 n <= 52, u16 n <= 6556, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   get_prime, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Offset: sequence starts at n = 1 */
    if (n == 0) return 0; /* undefined offset; safe fallback */

    /* Get the (n+1)-th and (n+2)-th odd primes */
    uint32_t p1 = get_prime(n + 1);
    uint32_t p2 = get_prime(n + 2);

    /* Compute the integer average of the two primes */
    seq_t avg = ((seq_t)p1 + (seq_t)p2) / 2;      /* always integer */

    /* Sum of prime factors (with multiplicity) of avg */
    seq_t spf_sum = 0;
    uint64_t m = (uint64_t)avg;
    while (m > 1)
    {
        uint64_t spf = smallest_prime_factor(m); /* static helper in header */
        spf_sum += (seq_t)spf;
        m /= spf;
    }

    return avg - spf_sum;
}
