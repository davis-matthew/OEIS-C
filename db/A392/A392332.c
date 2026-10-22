/*
 * A392332  a(n) is the excess of deficient numbers over abundant numbers in
 *          the first n positive integers.
 * https://oeis.org/A392332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 496, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n‑1)+1 if n is deficient, a(n‑1)−1 if abundant,
   a(n‑1) if n is perfect.  The implementation uses the
   smallest prime factor sieve to compute σ(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Cache for a(n) and for the SPF table. */
static seq_t *a_cache    = NULL;   /* a_cache[i] holds a(i+1) */
static idx_t  a_cache_sz = 0;      /* how many a(n) values are cached */
static idx_t *spf        = NULL;   /* SPF array for numbers up to spf_lim */
static idx_t  spf_lim    = 0;

/* Ensure the SPF table covers all numbers up to n. */
static void ensure_spf(idx_t n)
{
    if (n <= spf_lim) return;
    idx_t *new_spf = NULL;
    build_spf(n, &new_spf);
    free(spf);
    spf = new_spf;
    spf_lim = n;
}

/* Compute σ(n) using the previously built SPF table. */
static uint64_t sigma_from_spf(idx_t n)
{
    if (n == 1) return 1;           /* σ(1) = 1 */
    idx_t tmp = n;
    uint64_t sigma = 1;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        /* term = (p^(exp+1) - 1)/(p-1) */
        uint64_t powp = 1;
        for (int k = 0; k < exp + 1; ++k) powp *= (uint64_t)p;
        uint64_t term = (powp - 1) / (p - 1);
        sigma *= term;
    }
    return sigma;
}

/* Expand the cached a(n) values up to index n (inclusive). */
static void expand_cache(idx_t n)
{
    if (n <= a_cache_sz) return;

    /* Grow the a_cache array. */
    seq_t *new_cache = (seq_t *)realloc(a_cache, n * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    a_cache = new_cache;

    /* Ensure SPF table is large enough. */
    ensure_spf(n);

    /* Compute a(i) for all new indices. */
    for (idx_t i = a_cache_sz + 1; i <= n; ++i) {
        uint64_t sigma = sigma_from_spf(i);
        uint64_t two_i = 2 * (uint64_t)i;
        int delta = 0;
        if (sigma < two_i)   delta =  1;
        else if (sigma > two_i) delta = -1;
        /* delta == 0 for perfect numbers */

        if (i == 1) {
            a_cache[0] = (seq_t)delta;          /* a(1) = 1 */
        } else {
            long long prev = (long long)a_cache[i - 2];
            a_cache[i - 1] = (seq_t)(prev + delta);
        }
    }
    a_cache_sz = n;
}

/* Public OEIS function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, harmless */
    expand_cache(n);
    return a_cache[n - 1];
}
