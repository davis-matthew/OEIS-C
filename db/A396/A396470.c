/*
 * A396470  Sum of the distinct prime divisors of C(prime(n), n).
 * https://oeis.org/A396470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of C(prime(n), n)
 * Range:     u8 n <= 15, u16 n <= 227, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* p is the n‑th prime (p = prime(n)) */
    idx_t p = get_prime(n);           /* n ≥ 1 */

    /* Cache of primes up to the largest p seen so far */
    static idx_t     max_limit = 0;
    static size_t    prime_cnt = 0;
    static idx_t   *prime_list = NULL;

    if (p > max_limit) {              /* Need a larger sieve range */
        free(prime_list);              /* free any previous allocation */
        build_primes(p, &prime_list, &prime_cnt);
        max_limit = p;
    }

    seq_t sum = 0;                     /* sum of distinct primes in C(p, n) */
    idx_t sub = p - n;                 /* p – n */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t q = prime_list[i];
        if (q > p) break;

        /* Legendre’s formula for the exponent of q in C(p, n) */
        idx_t e = 0;
        idx_t power = q;
        while (power <= p) {
            e += p / power - n / power - sub / power;
            if (power > p / q) break;     /* avoid overflow of power *= q   */
            power *= q;
        }

        if (e > 0) {
            sum += (seq_t)q;              /* q divides the binomial coeff. */
        }
    }

    return sum;
}
