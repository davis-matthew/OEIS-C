/*
 * A366764  The sum of divisors of n that have no exponent 2 in their prime
 *          factorization.
 * https://oeis.org/A366764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} (p+1 if e==1 else (p^(e+1)-1)/(p-1) -
 *            p^2)
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the sum of divisors of n that have no exponent 2 in their prime
    factorization.  */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* work on a 64‑bit copy of n for factorisation */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* build list of primes up to sqrt(m) */
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    size_t limit = (size_t)int_sqrt(n);
    build_primes_upto(limit, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt && m > 1; ++i) {
        uint64_t p = primes[i];
        if (m % p != 0) continue;

        /* count exponent of p in m */
        uint64_t exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }

        if (exp == 1) {
            result *= ((seq_t)p + 1);
        } else {
            /* compute (p^(exp+1)-1)/(p-1) - p^2 */
            seq_t pow = 1;
            for (size_t k = 0; k < exp + 1; ++k)
                pow *= (seq_t)p;          /* pow = p^(exp+1) */

            seq_t sum_all = (pow - 1) / ((seq_t)p - 1);   /* geometric sum */
            seq_t p2  = (seq_t)p * (seq_t)p;              /* p^2 */
            seq_t term = sum_all - p2;
            result *= term;
        }
    }

    /* if remaining part of m is a prime > sqrt(original n) */
    if (m > 1) {
        result *= ((seq_t)m + 1);
    }

    free(primes);
    return result;
}
