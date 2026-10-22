/*
 * A319068  a(n) is the greatest k such that A000203(k) divides n where
 *          A000203 is the sum of divisors of n.
 * https://oeis.org/A319068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ k | sigma(k) divides n } (sigma = sum of divisors)
 * Range:     u8 n <= 21, u16 n <= 2230, u32 n <= 9620, u64 n <= 9620,
 *            u128 n <= 9620, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *spf = NULL;          /* smallest prime factor for each number */
static size_t spf_cap = 0;            /* current allocated size for spf */

static seq_t *sigma_cache = NULL;     /* cached sigma values, 0 = not computed */
static size_t sigma_cap = 0;          /* current allocated size for sigma_cache */

/* Ensure spf array covers indices up to limit, recompute from scratch if expanded */
static void ensure_spf(idx_t limit)
{
    if (limit < spf_cap) return;

    uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(1);
    spf = new_spf;
    spf_cap = limit + 1;

    /* Sieve to compute smallest prime factors */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    if (limit >= 0) spf[0] = 0;
    if (limit >= 1) spf[1] = 1;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Ensure sigma_cache array covers indices up to limit */
static void ensure_sigma_cache(idx_t limit)
{
    if (limit < sigma_cap) return;

    seq_t *new_cache = (seq_t *)realloc(sigma_cache, (limit + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);
    sigma_cache = new_cache;
    /* initialise new slots to zero */
    for (idx_t i = sigma_cap; i <= limit; ++i) sigma_cache[i] = 0;
    sigma_cap = limit + 1;
}

/* Compute sigma(k) using the precomputed spf and store in cache if required */
static seq_t sigma_of(idx_t k)
{
    if (sigma_cache[k] != 0) return sigma_cache[k];

    idx_t tmp = k;
    seq_t result = 1;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        idx_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        /* term = (p^(exp+1)-1)/(p-1) */
        seq_t power = 1;
        for (idx_t i = 0; i < exp + 1; ++i)
            power = power * (seq_t)p;
        seq_t term = (power - 1) / ((seq_t)p - 1);
        result = result * term;
    }
    sigma_cache[k] = result;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined, but safe */
    /* prepare necessary tables */
    ensure_spf(n);
    ensure_sigma_cache(n);

    seq_t n_seq = (seq_t)n;
    for (idx_t k = n; k >= 1; --k) {
        seq_t s = sigma_of(k);
        if (n_seq % s == 0) return k;
    }
    return 1; /* always reachable */
}
