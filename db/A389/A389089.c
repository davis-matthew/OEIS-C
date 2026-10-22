/*
 * A389089  Denominator of ratio sigma(n) / A048250(n), where sigma is the sum
 *          of divisors and A048250 is the sum of squarefree divisors.
 * https://oeis.org/A389089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A048250(n)/gcd(sigma(n), A048250(n)) where A048250(n) is
 *            the sum of squarefree divisors of n, sigma(n) is the sum of all
 *            divisors of n
 * Range:     u8 n <= 1232, u16 n <= 9297, u32 n <= 9297, u64 n <= 9297,
 *            u128 n <= 9297, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

static bool init_primes = false;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_prime_sieve(void) {
    if (!init_primes) {
        /* Sieve primes up to 100000 – more than enough for n ≤ 10000 */
        build_primes_upto(100000, &prime_list, &prime_count);
        init_primes = true;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0 – avoid UB */
    init_prime_sieve();

    idx_t tmp = n;
    seq_t A = 1;          /* sum of squarefree divisors */
    seq_t sigma = 1;      /* sum of all divisors */

    for (size_t i = 0; i < prime_count && prime_list[i]*prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p != 0) continue;

        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }

        /* Update A: multiply by (1 + p) */
        A *= ((seq_t)p + 1);

        /* Compute term = 1 + p + p^2 + ... + p^exp */
        seq_t term = 1;          /* p^0 */
        seq_t pp = 1;            /* current p^k */
        seq_t p_seq = (seq_t)p;
        for (int k = 0; k < exp; ++k) {
            pp *= p_seq;
            term += pp;
        }
        sigma *= term;
    }

    if (tmp > 1) { /* remaining prime factor with exponent 1 */
        uint64_t p = tmp;
        A *= ((seq_t)p + 1);
        sigma *= ((seq_t)p + 1);
    }

    seq_t d = gcd(sigma, A);
    return A / d;
}
