/*
 * A330746  Number of values of k, 1 <= k <= n, with A017666(k) = A017666(n),
 *          where A017666(n) = n/gcd(n, sigma(n)).
 * https://oeis.org/A330746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with 1 ≤ k ≤ n such that A017666(k) =
 *            A017666(n), where A017666(k) = k / gcd(k, σ(k))
 * Range:     u8 n <= 22, u16 n <= 2094, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

typedef uint64_t idx_t;

/* Global caches --------------------------------------------------------- */
static idx_t max_n = 0;                     /* largest n processed so far */
static seq_t *a_cache = NULL;              /* a_cache[i] = a(i) */
static idx_t *count_cache = NULL;           /* count_cache[v] = occurrence count of value v */
static idx_t *spf = NULL;                   /* smallest prime factor array */
static idx_t spf_limit = 0;                 /* size of spf array                 */

/* Allocate or extend a cache array of idx_t values. */
static void ensure_idx_cache(idx_t n)
{
    if (n <= max_n) return;
    count_cache = (idx_t*)realloc(count_cache, (n + 1) * sizeof(idx_t));
    for (idx_t i = max_n + 1; i <= n; ++i) count_cache[i] = 0;
}

/* Allocate or extend a cache array of seq_t values. */
static void ensure_seq_cache(idx_t n)
{
    if (n <= max_n) return;
    a_cache = (seq_t*)realloc(a_cache, (n + 1) * sizeof(seq_t));
    /* we will overwrite new entries after they are computed */
}

/* Build smallest prime factor (spf) array up to 'limit'. */
static void init_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    spf = (idx_t*)realloc(spf, (limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0)
            for (idx_t j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
    spf_limit = limit;
}

/* Compute sigma(n) (sum of divisors) using the SPF array. */
static seq_t sigma_of(idx_t n)
{
    seq_t sigma = (seq_t)1;
    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        seq_t power = (seq_t)1;
        for (idx_t t = 0; t < e + 1; ++t) power *= (seq_t)p;
        seq_t term = (power - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= term;
    }
    return sigma;
}

/* Ensure all data up to n is available (compute if necessary). */
static void reserve(idx_t n)
{
    if (n <= max_n) return;

    /* Build spf array as needed. */
    init_spf(n);

    /* Allocate/resize caches. */
    ensure_seq_cache(n);
    ensure_idx_cache(n);

    /* Compute new entries. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t sigma = sigma_of(i);
        seq_t g = gcd((seq_t)i, sigma);
        seq_t v = (seq_t)i / g;          /* A017666(i) */

        /* v fits in idx_t because v ≤ i ≤ n. */
        idx_t idx_v = (idx_t)v;
        count_cache[idx_v]++;                    /* count occurrences up to i */
        a_cache[i] = (seq_t)count_cache[idx_v];  /* store as seq_t for uniformity */
    }

    max_n = n;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1, harmless */
    reserve(n);
    return a_cache[n];
}
