/*
 * A380441  Sum of the nonprimes dividing n and the number of distinct primes
 *          dividing n.
 * https://oeis.org/A380441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sigma(n) - sum_{p|n} p + omega(n)
 * (i.e., sum over divisors d of n: add 1 if d is prime, else add d)
 *
 * Implementation uses prime factorization achieved by repeated calls to
 * _smallest_prime_factor.  sigma(n) is computed multiplicatively from the
 * prime powers, while the sum of distinct primes and the number of
 * distinct primes are accumulated directly.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (outside OEIS offset) */
    if (n == 0) return 0;

    uint64_t tmp = (uint64_t)n;
    seq_t sigma = 1;          /* Product over prime power terms */
    seq_t prime_sum = 0;      /* Sum of distinct prime factors */
    seq_t omega = 0;          /* Number of distinct prime factors */

    while (tmp > 1) {
        uint64_t p = _smallest_prime_factor(tmp);
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }

        seq_t p_seq = (seq_t)p;

        /* Compute (p^(exp+1) - 1) / (p - 1) */
        seq_t p_power = 1;
        for (int i = 0; i < exp + 1; i++) {
            p_power *= p_seq;
        }
        seq_t term = (p_power - 1) / (p_seq - 1);

        sigma *= term;

        prime_sum += p_seq;
        omega += 1;
    }

    return sigma - prime_sum + omega;
}
