/*
 * A367840  Expansion of e.g.f. 1/(2 + x - exp(4*x)).
 * https://oeis.org/A367840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The general recurrence for the sequence A367840
   (Expansion of e.g.f. 1/(2 + x - exp(4*x))) is
   a(0) = 1
   a(n) = -n * a(n-1) +
          Σ_{k=1..n} 4^k * binomial(n,k) * a(n-k)   (n ≥ 1)   */
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* cache of already computed terms */
static seq_t *cache = NULL;      /* storage of a(i) */
static size_t cache_len = 0;     /* number of cached entries (valid)*/
static size_t cache_cap = 0;     /* allocated capacity */

/* cache of powers of 4: pow4[i] = 4^i */
static seq_t *pow4 = NULL;       /* storage of 4^i */
static size_t pow4_len = 0;      /* number of cached powers (valid)*/
static size_t pow4_cap = 0;      /* allocated capacity */

/* ensure capacity for both caches */
static void
ensure_capacity(idx_t n)
{
    idx_t need = n + 1;
    if ((size_t)need > cache_cap) {
        size_t new_cap = cache_cap < 16 ? 16 : cache_cap * 2;
        while (new_cap < (size_t)need)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        pow4  = (seq_t *)realloc(pow4,  new_cap * sizeof(seq_t));
        if (!cache || !pow4)
            return; /* out of memory – caller will see garbage */
        cache_cap = new_cap;
        pow4_cap  = new_cap;
    }
}

/* Initialise the first terms of the caches */
static void
init_cache(void)
{
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        pow4[0]  = (seq_t)1;   /* 4^0 */
        cache_len = 1;
        pow4_len  = 1;
    }
}

seq_t a(idx_t n)
{
    /* Make sure we have room for n */
    ensure_capacity(n);
    init_cache();

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Make sure powers of 4 up to i are available */
        while (pow4_len <= (size_t)i) {
            pow4[pow4_len] = pow4[pow4_len - 1] * (seq_t)4;
            pow4_len++;
        }

        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = pow4[k];                           /* 4^k       */
            term = term * binomial(i, k);                   /* * C(i,k)  */
            term = term * cache[i - k];                     /* * a(i-k)  */
            sum += term;
        }

        seq_t prod = (seq_t)i * cache[i - 1];               /* n * a(n-1)*/
        cache[i] = sum - prod;                              /* -n*a(n-1)+sum */
    }

    cache_len = n + 1;  /* all entries up to n are now valid */
    return cache[n];
}
