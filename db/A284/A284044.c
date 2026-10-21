/*
 * A284044  Largest positive k among all primes p < n such that n^(p-1) == 1
 *          (mod p^k).
 * https://oeis.org/A284044
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum exponent k such that n^(p-1) ≡ 1 (mod p^k) over
 *            all primes p < n
 * Range:     u8 n <= 17, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   mod_pow, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A284044 */
#include <stddef.h>
#include <stdlib.h>

#include "../utils/sieve_primes.h"   /* void sieve_primes(idx_t limit, idx_t **out, size_t *count_out); */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3. Return 0 for smaller indices. */
    if (n < 3) return 0;

    /* Static cache of primes up to the current maximum n-1. */
    static idx_t *primes   = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    if (n - 1 > primes_limit) {          /* need more primes */
        free(primes);                    /* free old array if any */
        sieve_primes(n - 1, &primes, &primes_cnt);
        primes_limit = n - 1;
    }

    seq_t max_k = 0;                      /* maximum exponent found */

    seq_t base = (seq_t)n;                /* base for exponentiation */

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (n % p == 0) continue;         /* skip primes dividing n (v_p = 0) */

        seq_t mod = (seq_t)p;             /* modulus p^k; start with k = 1 */
        idx_t exp = p - 1;                /* exponent in n^(p-1) */

        seq_t k = 1;                     /* current exponent */
        while (1) {
            seq_t r = mod_pow(base, exp, mod);
            if (r == 1) {
                ++k;                    /* p^(k) divides n^(p-1)-1   */
                mod *= (seq_t)p;         /* increase modulus to p^(k+1) */
            } else {
                break;                  /* stop when congruence fails */
            }
        }

        seq_t ord = k - 1;                /* v_p(n^(p-1)-1) */
        if (ord > max_k) max_k = ord;
    }

    return max_k;
}
