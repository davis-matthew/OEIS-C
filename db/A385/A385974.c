/*
 * A385974  a(n) = the number of reduced positive proper fractions p/q with
 *          phi(q) <= n.
 * https://oeis.org/A385974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{q\ge 2}\phi(q)\cdot[\,\phi(q)\le n\,]
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1)=1, a(2)=7, a(3)=7, a(4)=23, … */
#include <stdlib.h>

/* Max denominator that can appear for n <= 10000
   (safety factor 200000 is more than enough) */
#define PHI_LIMIT 200000u

static idx_t  phi[PHI_LIMIT + 1] = {0};      /* Euler's totient values   */
static int    phi_computed     = 0;          /* 0=not computed yet      */
static seq_t* cache            = NULL;      /* a(0)…a(n)                 */
static idx_t  cache_len        = 0;          /* number of elements in cache */

/* Computes phi array up to PHI_LIMIT once */
static void ensure_phi()
{
    if (phi_computed) return;
    for (idx_t i = 0; i <= PHI_LIMIT; ++i) phi[i] = i;
    phi[0] = 0;  if (PHI_LIMIT >= 1) phi[1] = 1;   /* phi(1) unused in sum */
    for (idx_t i = 2; i <= PHI_LIMIT; ++i) {
        if (phi[i] == i) {            /* i is prime */
            for (idx_t j = i; j <= PHI_LIMIT; j += i)
                phi[j] -= phi[j] / i;
        }
    }
    phi_computed = 1;
}

/* Rebuilds cache for requested n */
static void rebuild_cache(idx_t n)
{
    if (cache) free(cache);
    /* allocate n+1 elements (a(0) unused but simplifies indices) */
    cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cache) { cache_len = 0; return; }       /* allocation failed */
    for (idx_t i = 0; i <= n; ++i) cache[i] = 0;

    /* accumulate contributions */
    for (idx_t q = 2; q <= PHI_LIMIT; ++q) {
        idx_t ph = phi[q];
        if (ph <= n) {
            cache[ph] += (seq_t)ph;               /* phi(q) fractions */
        }
    }
    /* prefix sum to obtain a(i) */
    for (idx_t i = 1; i <= n; ++i)
        cache[i] += cache[i - 1];

    cache_len = n + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */

    ensure_phi();                        /* compute phi values once */

    /* If cache is not big enough, rebuild it */
    if (cache_len == 0 || n >= cache_len) {
        rebuild_cache(n);
    }

    /* If failed to allocate, return 0 as fallback */
    if (!cache) return 0;
    return cache[n];
}
