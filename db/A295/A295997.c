/*
 * A295997  Least composite k such that d^k == d (mod k) for every divisor d
 *          of n.
 * https://oeis.org/A295997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least composite k such that for every prime p dividing n,
 *            p^k ≡ p (mod k). The implementation searches k from 4 up to 561
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

/* Global prime table used for factoring */
static uint64_t *s_primes = NULL;
static size_t   s_pcnt   = 0;

/* One‑time prime table initialization */
static void init_primes(void)
{
    if (s_primes != NULL)
        return;
    const idx_t PRIME_LIMIT = 1000000;           /* enough for n ≤ 10^12   */
    build_primes_upto(PRIME_LIMIT, &s_primes, &s_pcnt);
}

/* Factor a number into its distinct prime divisors.
   The distinct primes are written into `fac` and the number of
   distinct primes is returned.  The array must be large enough. */
static size_t distinct_factors(idx_t n, uint64_t *fac)
{
    size_t cnt = 0;
    idx_t t = n;
    for (size_t i = 0; i < s_pcnt && s_primes[i] * s_primes[i] <= t; ++i) {
        uint64_t p = s_primes[i];
        if (t % p == 0) {
            fac[cnt++] = p;
            while (t % p == 0)
                t /= p;
        }
    }
    if (t > 1)                /* remaining prime factor > sqrt(n) */
        fac[cnt++] = (uint64_t)t;
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    /* Compute rad(n) = product of distinct prime divisors of n */
    idx_t rad = 1;
    idx_t t = n;
    for (size_t i = 0; i < s_pcnt && s_primes[i] * s_primes[i] <= t; ++i) {
        uint64_t p = s_primes[i];
        if (t % p == 0) {
            rad *= p;
            while (t % p == 0)
                t /= p;
        }
    }
    if (t > 1)
        rad *= t;

    /* Extract the distinct prime divisors of rad */
    uint64_t pf[64];                   /* rad ≤ n, number of primes is small */
    size_t pf_cnt = distinct_factors(rad, pf);

    /* rad == 1 implies no constraints; the smallest composite is 4 */
    if (rad == 1)
        return (seq_t)4;

    /* Search for the smallest composite k (≤ 561) satisfying p^k ≡ p (mod k) */
    for (uint64_t k = 4; k <= 561; ++k) {
        if (is_prime_small((idx_t)k))        /* skip prime k */
            continue;

        bool ok = true;
        for (size_t j = 0; j < pf_cnt && ok; ++j) {
            uint64_t p = pf[j];
            seq_t lhs = mod_pow((seq_t)p, k, (seq_t)k);
            seq_t rhs = (seq_t)(p % k);
            if (lhs != rhs)
                ok = false;
        }
        if (ok)
            return (seq_t)k;
    }

    /* Theoretically unreachable: a(n) ≤ 561 for all n ≥ 1 */
    return (seq_t)0;
}
