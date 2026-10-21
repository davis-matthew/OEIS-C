/*
 * A244097  a(n) is the smallest k such that the sum of n consecutive values
 *          M(k) + M(k+1) + ... + M(k+n-1) is zero, where M(m) is the Moebius
 *          (or Möbius) function (A008683).
 * https://oeis.org/A244097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that ∑_{i=0}^{n‑1} μ(k+i) = 0 (μ is the
 *            Möbius function). The implementation keeps a sieve of μ up to
 *            the current limit, a prefix‑sum array to test sums in O(1), and
 *            a cache of previously computed a(n)
 * Range:     u8 n <= 546, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_spf.h"    /* builds spf[] : smallest prime factor */

/*------------------------------------------------------------*/
/*   Dynamic array helpers                                     */
/*------------------------------------------------------------*/
static void grow_array(void **arr, size_t *cap, size_t need, size_t elem_size)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    void *p = realloc(*arr, new_cap * elem_size);
    if (!p) abort();              /* out of memory */
    *arr = p;
    *cap = new_cap;
}

/*------------------------------------------------------------*/
/*   Möbius sieve and prefix sums                             */
/*------------------------------------------------------------*/
static idx_t  max_computed = 0;          /* largest m for which data exist   */
static idx_t *spf          = NULL;       /* smallest prime factor array      */
static size_t spf_cap      = 0;

static int8_t *mu          = NULL;       /* μ[0 … max_computed]              */
static size_t  mu_cap      = 0;
static int32_t *pref       = NULL;       /* pref[0 … max_computed]           */
static size_t  pref_cap    = 0;

/* Re‑compute μ[1…limit] and its prefix sums.  The sieving is done
   from scratch each time limit increases. */
static void ensure_mu_upto(idx_t limit)
{
    if (limit <= max_computed) return;

    /* Allocate/resize spf array */
    if (spf != NULL) free(spf);
    build_spf(limit, &spf);
    spf_cap = limit + 1;

    /* Allocate/resize mu and pref arrays */
    grow_array((void **)&mu,   &mu_cap,   limit + 1, sizeof(int8_t));
    grow_array((void **)&pref, &pref_cap, limit + 1, sizeof(int32_t));

    /* Compute μ up to limit.  μ(1)=1, μ(n)=0 if n has a squared prime factor,
       otherwise μ(n)=(-1)^{number of distinct prime factors}. */
    mu[0] = 0;          /* unused */
    mu[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t p = spf[i];
        idx_t j = i / p;
        if (j % p == 0)       /* p² | i   → square factor */
            mu[i] = 0;
        else
            mu[i] = -mu[j];
    }

    /* Build prefix sums */
    pref[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pref[i] = pref[i-1] + mu[i];

    max_computed = limit;
}

/*------------------------------------------------------------*/
/*   Cache of computed a(n)                                   */
/*------------------------------------------------------------*/
static seq_t *cache   = NULL;   /* cache[n-1] holds a(n) for n≥1 */
static size_t cache_cap = 0;

static void cache_init(size_t n)
{
    grow_array((void **)&cache, &cache_cap, n, sizeof(seq_t));
    for (size_t i = cache_cap; i < n; ++i)
        cache[i] = 0;              /* 0 indicates unsolved (a(n)≥1) */
}

/*------------------------------------------------------------*/
/*   Public routine                                           */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 */

    /* Ensure cache big enough */
    if (n > cache_cap) cache_init(n);

    if (cache[n-1] != 0)                /* already known */
        return cache[n-1];

    /* Search for minimal k.  We may need to extend the sieve gradually. */
    idx_t limit = (max_computed < n) ? n : max_computed;
    while (true) {
        ensure_mu_upto(limit);

        bool found = false;
        idx_t best_k = 0;
        for (idx_t k = 1; k + n - 1 <= limit; ++k) {
            if (pref[k + n - 1] == pref[k - 1]) {
                best_k = k;
                found  = true;
                break;
            }
        }

        if (found) {
            cache[n-1] = (seq_t)best_k;
            return cache[n-1];
        }

        /* Not found: extend limit until the required block exists. */
        idx_t new_limit = limit * 2;
        if (new_limit < limit + n) new_limit = limit + n;
        limit = new_limit;
    }
}
