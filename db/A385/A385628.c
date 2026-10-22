/*
 * A385628  Sum of the divisors d of n with an even number of primes not
 *          exceeding d.
 * https://oeis.org/A385628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * ((pi(d)+1) mod 2) = Sum_{d|n, pi(d) even} d
 *            The code computes this value for any n ≥ 1. It uses a simple
 *            prime sieve to precalculate all primes up to the largest n seen
 *            in the current run and builds the divisor set by back‑tracking
 *            over the prime factorisation of n
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"

/* Static cache for the prime table */
static size_t   primes_cnt   = 0;
static idx_t   *primes       = NULL;
static idx_t    primes_limit = 0;

/* Ensure all primes up to 'limit' are available */
static void
ensure_primes(idx_t limit)
{
    if (limit > primes_limit) {
        /* If we already had a table, free it */
        free(primes);
        size_t  new_cnt;
        idx_t *new_primes;
        build_primes(limit, &new_primes, &new_cnt);
        primes       = new_primes;
        primes_cnt   = new_cnt;
        primes_limit = limit;
    }
}

/* Count the number of primes ≤ x via binary search */
static inline size_t
pi_leq(idx_t x)
{
    size_t l = 0, r = primes_cnt;
    while (l < r) {
        size_t m = (l + r) >> 1;
        if (primes[m] <= x)
            l = m + 1;
        else
            r = m;
    }
    return l;          /* number of primes ≤ x */
}

/* Factorise n = ∏ p_i ^ e_i, storing prime factors in pf[] and exponents in ex[].
   primes[] must contain all primes up to sqrt(n). */
static void
factor_n(idx_t n, idx_t *pf, unsigned *ex, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n; ++i) {
        if (n % primes[i] == 0) {
            unsigned e = 0;
            while (n % primes[i] == 0) {
                n /= primes[i];
                ++e;
            }
            pf[(*cnt)] = primes[i];
            ex[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pf[(*cnt)]   = n;
        ex[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/* Recursively generate all divisors of n, storing them in out[0..*len-1] */
static void
gen_divisors_rec(idx_t idx, idx_t cur,
                 idx_t *pf, unsigned *ex, size_t cnt,
                 seq_t *out, size_t *len)
{
    if (idx == cnt) {
        out[(*len)++] = (seq_t)cur;
        return;
    }
    idx_t p = pf[idx];
    idx_t x = cur;
    for (unsigned e = 0; e <= ex[idx]; ++e) {
        gen_divisors_rec(idx + 1, x, pf, ex, cnt, out, len);
        x *= p;
    }
}

/* Public sequence function required by the build system */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Ensure primes up to n exist for pi_leq and factorisation */
    ensure_primes(n);

    /* Factor n */
    idx_t pf[64];
    unsigned ex[64];
    size_t cnt;
    factor_n(n, pf, ex, &cnt);

    /* Alloc buffer large enough for all divisors (≤ d(n) ≤ 128 for n ≤ 10000) */
    seq_t *divs = (seq_t *)malloc(sizeof(seq_t) * 256);
    if (divs == NULL)
        return (seq_t)0;   /* out of memory – return 0 */

    size_t tot = 0;
    gen_divisors_rec(0, 1, pf, ex, cnt, divs, &tot);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < tot; ++i) {
        idx_t d   = (idx_t)divs[i];
        size_t pi = pi_leq(d);
        if ((pi & 1) == 0)          /* pi(d) even */
            sum += (seq_t)d;
    }

    free(divs);   /* clean up */
    return sum;
}
