/*
 * A368449  Expansion of e.g.f. exp(x) / (1 + log(1 - 2*x)/2).
 * https://oeis.org/A368449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=1..n} 2^{k-1} * (k-1)! * binomial(n,k) * a(n-k)
 *            a(0) = 1 The implementation memoises values to answer queries in
 *            any order with an O(n²) overall running time for all calls up to
 *            `n`
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard C/ C++ includes */
#include <stdlib.h>  /* malloc, realloc, abort, size_t */

/* Utility functions from the OEIS library */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */
#include "../utils/mul_small.h"  /* seq_t mul_small(seq_t x, unsigned int m) */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b) */

/* Memoisation arrays */
static seq_t   *cache  = NULL;   /* a(n) values */
static size_t  cache_cap = 0;
static idx_t   cache_max  = 0;   /* largest n computed so far */

/* Pre‑computed factorials (k! for k>=0) and powers of two (2^k) */
static seq_t   *fact_arr  = NULL; /* k! */
static size_t  fact_cap  = 0;
static idx_t   fact_max  = 0;     /* largest k for which fact_arr is filled */

static seq_t   *pow2_arr  = NULL; /* 2^k */
static size_t  pow2_cap  = 0;
static idx_t   pow2_max  = 0;     /* largest k for which pow2_arr is filled */

/* --------------------------------------------------------------------- */
/* Ensure that `cache` has space for indices up to `n` (inclusive).      */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Ensure that factorial and power‑of‑two arrays are computed up to `n`. */
static void ensure_fact_pow(idx_t n)
{
    if (n <= fact_max && n <= pow2_max) return;

    size_t new_cap = fact_cap ? fact_cap << 1 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    if (new_cap > fact_cap) {
        seq_t *tmp = (seq_t *)realloc(fact_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fact_arr = tmp;
        fact_cap = new_cap;
    }
    if (new_cap > pow2_cap) {
        seq_t *tmp = (seq_t *)realloc(pow2_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow2_arr = tmp;
        pow2_cap = new_cap;
    }

    /* Compute missing entries */
    for (idx_t i = fact_max + 1; i <= n; ++i) {
        /* factorial: (i)! = (i-1)! * i  (use mul_small for small factor) */
        fact_arr[i] = mul_small(fact_arr[i - 1], (unsigned int)i);
    }
    for (idx_t i = pow2_max + 1; i <= n; ++i) {
        pow2_arr[i] = mul_small(pow2_arr[i - 1], 2);
    }

    if (n > fact_max) fact_max = n;
    if (n > pow2_max) pow2_max = n;
}

/* --------------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n)
{
    /* First call: initialise memoisation arrays */
    if (!cache) {
        /* a(0) = 1 */
        size_t init_cap = 16;
        cache     = (seq_t *)malloc(init_cap * sizeof(seq_t));
        fact_arr  = (seq_t *)malloc(init_cap * sizeof(seq_t));
        pow2_arr  = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache || !fact_arr || !pow2_arr) abort();

        cache[0]   = (seq_t)1;   /* a(0) */
        cache_cap  = init_cap;
        cache_max  = 0;

        fact_arr[0] = (seq_t)1;  /* 0! = 1 */
        pow2_arr[0] = (seq_t)1;  /* 2^0 = 1 */
        fact_cap   = init_cap;
        pow2_cap   = init_cap;
        fact_max   = 0;
        pow2_max   = 0;
    }

    /* Ensure we can hold n */
    ensure_cache(n);
    /* Ensure factorials and powers of two up to n are ready */
    ensure_fact_pow(n);

    /* Compute all missing terms up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;

        for (idx_t k = 1; k <= m; ++k) {
            seq_t binom = binomial(m, k);          /* binomial(m,k)   */
            seq_t part  = seq_mul(binom, fact_arr[k - 1]);   /* * (k-1)! */
            part = seq_mul(part, pow2_arr[k - 1]);            /* * 2^(k-1) */
            part = seq_mul(part, cache[m - k]);              /* * a(n-k)  */
            sum += part;
        }
        cache[m] = sum + (seq_t)1;                 /* + leading 1 */
    }

    cache_max = n > cache_max ? n : cache_max;
    return cache[n];
}
