/*
 * A392619  a(n) = denominator((2^n*(n!)^2/(1+2*n)!)^2).
 * https://oeis.org/A392619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((2^n*(n!)^2/(1+2*n)!)^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Legendre: exponent of prime p in m! */
static idx_t vp_fact(idx_t m, idx_t p)
{
    idx_t cnt = 0;
    while (m) {
        m /= p;
        cnt += m;
    }
    return cnt;
}

/* Simple dynamic sieve: keep primes up to the current maximum limit. */
static uint32_t *primes = NULL;
static size_t   primes_cnt = 0;
static idx_t    primes_limit = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    /* free old array if any */
    if (primes != NULL)
        free(primes);

    /* Sieve up to limit */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime)
        return; /* allocation failure; computation will abort */

    /* initialize */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    size_t pc = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++pc;

    primes = (uint32_t *)malloc(pc * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[pos++] = (uint32_t)i;

    primes_cnt = pc;
    primes_limit = limit;
    free(is_prime);
}

extern seq_t a(idx_t n)
{
    /* n = 0 returns 1 */
    if (n == 0)
        return (seq_t)1;

    idx_t limit = 2 * n + 1;
    ensure_primes(limit);

    seq_t denom = (seq_t)1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p > limit)
            break;

        idx_t vp_tot = vp_fact(limit, p);   /* vp((2n+1)!) */
        idx_t vp_n   = vp_fact(n, p);       /* vp(n!) */

        /* Compute exponent difference of p in denominator after reduction */
        int64_t diff = (int64_t)2 * (int64_t)vp_tot
                       - (int64_t)4 * (int64_t)vp_n
                       - ((p == 2) ? (int64_t)2 * (int64_t)n : (int64_t)0);

        if (diff > 0) {
            seq_t pow = big_pow((seq_t)p, (seq_t)diff);
            denom *= pow;
        }
    }
    return denom;
}
