/*
 * A321975  6-dimensional Catalan numbers.
 * https://oeis.org/A321975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0!*1!*2!*3!*4!*5! * (6*n)! / (n!*(n+1)!*…*(n+5)!)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   build_primes_upto, gcd, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/pow_int.h"

static inline idx_t fact_prime_exp(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* Handle small n directly to avoid unnecessary prime generation */
    if (n == 0) return (seq_t)1;

    /* Build list of primes up to 6*n */
    idx_t limit = 6 * n;
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* Result */
    seq_t res = (seq_t)1;

    /* Prefactor exponents for 0!*1!*…*5! : 2^8 * 3^3 * 5^1 */
    const struct { unsigned int p; int exp; } prefactors[] = {
        {2, 8}, {3, 3}, {5, 1}
    };

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];

        /* Exponent in (6n)! */
        idx_t e6 = fact_prime_exp(limit, p);

        /* Sum of exponents in n!, (n+1)!, …, (n+5)! */
        idx_t expsum = 0;
        for (int j = 0; j <= 5; ++j)
            expsum += fact_prime_exp(n + j, p);

        /* Prefactor exponent for this prime if present */
        int pref_exp = 0;
        for (size_t k = 0; k < sizeof(prefactors)/sizeof(prefactors[0]); ++k)
            if (prefactors[k].p == p) { pref_exp = prefactors[k].exp; break; }

        idx_t exp = e6 + pref_exp - expsum;
        if (exp > 0) {
            seq_t pow_val = pow_int((seq_t)p, exp);
            res *= pow_val;
        }
    }

    free(primes);
    return res;
}
