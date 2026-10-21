/*
 * A241755  A finite sum of products of binomial coefficients: Sum_(m=0..n)
 *          binomial(-1/4, m)^2*binomial(-1/4, n-m)^2 (C. C. Grosjean's
 *          problem, numerators).
 * https://oeis.org/A241755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = ((2n-1)!!) ^3 / ((n!)^3 * 2^{3n}) ⇒ a(n) is the reduced
 *            numerator of the rational sum Sum_{m=0..n} binomial(-1/4,m)^2 *
 *            binomial(-1/4,n-m)^2. The integer value equals the numerator
 *            when the fraction is fully reduced
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/seq_mul.h"     /* seq_mul(seq_t a, seq_t b) */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return (seq_t)1;

    /* Return early for n = 1; no odd primes ≤ 1 */
    if (n == 1)
        return (seq_t)1;

    /* 2*n-1 < 2*n <= 20000 for n<=10000, fits in idx_t */
    idx_t limit = 2 * n - 1;

    /* Allocate a small sieve array (bool) */
    size_t L = (size_t)limit + 1;
    uint8_t *composite = (uint8_t *)calloc(L, 1);
    if (!composite)
        abort();

    /* Temporary storage of primes */
    uint64_t *primes = NULL;
    size_t primes_cap = 0, primes_len = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            /* Store the new prime */
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    primes_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[primes_len++] = (uint64_t)i;

            /* Mark multiples of i */
            if (i > limit / i)
                continue;
            for (idx_t j = i * i; j <= limit; j += i)
                composite[j] = 1;
        }
    }

    /* Compute the integer numerator */
    seq_t res = (seq_t)1;

    for (size_t pi = 0; pi < primes_len; ++pi) {
        uint64_t p = primes[pi];
        if (p == 2)           /* skip the even prime */
            continue;

        /* exponent of p in product of odd numbers 1,3,…,(2n-1) */
        idx_t e1 = 0;
        idx_t pk = p;
        while (pk <= limit) {
            e1 += limit / pk - limit / (pk * 2);
            if (pk > limit / p) /* avoid overflow */
                break;
            pk *= p;
        }

        /* exponent of p in n! (legendre's formula) */
        idx_t leg = 0;
        idx_t tmp = n;
        while (tmp) {
            tmp /= p;
            leg += tmp;
        }

        /* exponent of p in the reduced numerator: 3*(e1 - leg) */
        idx_t exp = 3 * (e1 - leg);
        if (exp > 0) {
            seq_t pow = seq_pow((seq_t)p, exp);
            res = seq_mul(res, pow);
        }
    }

    free(primes);
    free(composite);
    return res;
}
