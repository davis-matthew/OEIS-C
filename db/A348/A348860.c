/*
 * A348860  G.f. A(x) satisfies: A(x) = 1 / ((1 + x) * (1 - x * A(2*x))).
 * https://oeis.org/A348860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n + Sum_{k=0..n-1} 2^k * a(k) * a(n-k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* memoized a(n) values      */
static idx_t cache_size = 0;          /* allocated size of cache   */
static idx_t max_cache_computed = 0;  /* largest n whose value is set */

static seq_t *pow2 = NULL;            /* pre‑computed powers of two */
static idx_t pow2_size = 0;           /* allocated size of pow2     */
static idx_t max_pow2_computed = 0;   /* largest k whose 2^k is set */

/* Ensure array size ≥ n+1 for cache */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = cache_size ? cache_size << 1 : 1;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!tmp) abort();
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        if (cache) free(cache);
        cache = tmp;
        cache_size = new_size;
    }
}

/* Ensure array size ≥ n+1 for pow2 */
static void ensure_pow2(idx_t n)
{
    if (pow2_size <= n) {
        idx_t new_size = pow2_size ? pow2_size << 1 : 1;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!tmp) abort();
        for (idx_t i = 0; i < pow2_size; ++i) tmp[i] = pow2[i];
        if (pow2) free(pow2);
        pow2 = tmp;
        pow2_size = new_size;
    }
}

/* Fill missing powers of two up to index n */
static void fill_pow2(idx_t n)
{
    if (max_pow2_computed == 0 && pow2_size > 0) {
        pow2[0] = (seq_t)1;
        max_pow2_computed = 0;
    }
    for (idx_t k = max_pow2_computed + 1; k <= n; ++k) {
        /* 2^k = 2 * 2^(k-1) */
        pow2[k] = pow2[k - 1] * (seq_t)2;
        max_pow2_computed = k;
    }
}

/* Fill missing cache values up to index n */
static void fill_cache(idx_t n)
{
    if (max_cache_computed == 0 && cache_size > 0) {
        cache[0] = (seq_t)1;
        max_cache_computed = 0;
    }
    if (max_cache_computed < 1 && n >= 1) {
        cache[1] = (seq_t)0;
        max_cache_computed = 1;
    }

    for (idx_t i = max_cache_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t t = cache[k] * cache[i - 1 - k];
            t = t * pow2[k];
            sum += t;
        }
        /* add (-1)^i */
        if (i & 1)
            sum -= (seq_t)1;
        else
            sum += (seq_t)1;
        cache[i] = sum;
        max_cache_computed = i;
    }
}

/* Public implementation of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Ensure storage for cache and powers of two */
    ensure_cache(n);
    ensure_pow2(n);

    /* Compute any missing 2^k that may be needed */
    fill_pow2(n);

    /* Compute any missing a(k) values up to n */
    fill_cache(n);

    return cache[n];
}
