/*
 * A378996  Denominator of sigma(2*n)/(2*n).
 * https://oeis.org/A378996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2*n / gcd(sigma(2*n), 2*n) (denominator of σ(2*n)/(2*n))
 * Range:     u8 n <= 48, u16 n <= 8772, u32 n <= 8772, u64 n <= 8772,
 *            u128 n <= 8772, big any n
 * Helpers:   build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/build_primes.h"

/* static storage for primes */
static idx_t  *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t  prime_limit = 0;

/* ensure we have primes up to at least 'need' */
static void ensure_primes(idx_t need)
{
    if (prime_limit >= need)
        return;

    /* free old list if any */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* build primes up to 'need' (inclusive) */
    build_primes(need, (idx_t **)&prime_list, &prime_cnt);
    prime_limit = need;
}

seq_t a(idx_t n)
{
    /* m = 2*n as seq_t */
    seq_t m = (seq_t)n * (seq_t)2U;

    /* factor 2*n using 64‑bit trial division */
    uint64_t copy = (uint64_t)m;                    /* m fits 64‑bit for n ≤ 10^9 */
    idx_t limit = (idx_t)(sqrt((double)copy)) + 1U; /* primes up to sqrt(copy) */
    ensure_primes(limit);

    seq_t sigma = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
        uint64_t p = prime_list[i];
        if (copy % p != 0U)
            continue;

        /* count exponent of p in m */
        int exp = 0;
        while (copy % p == 0U) {
            copy /= p;
            ++exp;
        }

        /* term = (p^(exp+1) - 1) / (p - 1) */
        seq_t pow_val = (seq_t)1;
        for (int j = 0; j < exp + 1; ++j) {
            pow_val *= (seq_t)p;
        }
        seq_t term = (pow_val - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= term;
    }

    if (copy > 1U) {
        /* remaining prime factor */
        seq_t p = (seq_t)copy;
        seq_t term = p + (seq_t)1;            /* σ(p) = p + 1 */
        sigma *= term;
    }

    /* denominator = m / gcd(sigma, m) */
    seq_t g = gcd(sigma, m);
    seq_t denom = m / g;

    return denom;
}
