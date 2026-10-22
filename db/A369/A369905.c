/*
 * A369905  a(n) = Sum_{p|n, p prime} n^omega(n/p).
 * https://oeis.org/A369905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime} n^omega(n/p)
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _smallest_prime_factor, count_distinct_primes, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS sequence A369905.
   Uses helper functions:
      _smallest_prime_factor(uint64_t n)
      count_distinct_primes(uint64_t n)
      pow_int(seq_t base, idx_t exp)
   which are available in the utils directory. */

#include "../utils/_smallest_prime_factor.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, but safe return */

    if (n == 1)
        return (seq_t)0;          /* a(1) = 0, no prime divisors */

    seq_t sum = 0;
    uint64_t tmp = (uint64_t)n;

    /* factor n, collecting distinct prime divisors */
    while (tmp > 1)
    {
        uint64_t p = _smallest_prime_factor(tmp);
        /* skip repeated factors */
        while (tmp % p == 0)
            tmp /= p;

        /* compute exponent: omega(n/p) */
        uint64_t m = (uint64_t)n / p;          /* n/p fits in uint64_t */
        seq_t exp = count_distinct_primes(m);  /* omega(m) */

        /* compute term: n^exp using integer exponentiation */
        seq_t term = pow_int((seq_t)n, (idx_t)exp);
        sum += term;
    }

    return sum;
}
