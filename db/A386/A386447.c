/*
 * A386447  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} k^6 * a(k) * a(n-1-k).
 * https://oeis.org/A386447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = a(n-1) + Σ_{k=0}^{n-1} k^6 * a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Dynamic cache for the sequence values */
static seq_t *a_cache = NULL;
static size_t a_cap  = 0;   /* allocated capacity for a_cache         */
static size_t a_len  = 0;   /* number of elements already computed    */

/* Cache for k^6 values, stored from index 0 up to the highest k used */
static seq_t *pow6_cache = NULL;
static size_t pow6_cap = 0; /* capacity of pow6_cache */
static size_t pow6_len = 0; /* next free slot in pow6_cache */

/* Ensure a_cache has room for indices 0 … n (inclusive) */
static void ensure_a_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (a_cap < need) {
        size_t cap = a_cap ? a_cap * 2 : 8;
        if (cap < need) cap = need;
        seq_t *tmp = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        a_cache = tmp;
        a_cap   = cap;
    }
}

/* Ensure pow6_cache has room for indices 0 … k (inclusive) and set them */
static void ensure_pow6_capacity_and_compute(idx_t k)
{
    size_t need = (size_t)k + 1;
    if (pow6_cap < need) {
        size_t cap = pow6_cap ? pow6_cap * 2 : 8;
        if (cap < need) cap = need;
        seq_t *tmp = (seq_t *)realloc(pow6_cache, cap * sizeof(seq_t));
        if (!tmp) abort();
        pow6_cache = tmp;
        pow6_cap   = cap;
    }
    /* Compute any missing k^6 values */
    for (size_t i = pow6_len; i <= (size_t)k; ++i) {
        seq_t p = 1;
        for (int j = 0; j < 6; ++j) p *= (seq_t)i;
        pow6_cache[i] = p;
    }
    if (pow6_len <= (size_t)k) pow6_len = (size_t)k + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;    /* base case, no need to touch caches */

    /* Grow sequence cache up to index n */
    ensure_a_capacity(n);
    if (a_len == 0) {               /* first time we touch the cache */
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* Compute new terms up to n */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        /* make sure we have k^6 up to i-1 */
        ensure_pow6_capacity_and_compute((idx_t)i - 1);

        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            sum += pow6_cache[k] * a_cache[k] * a_cache[i - 1 - k];
        }
        a_cache[i] = a_cache[i - 1] + sum;
    }

    a_len = (size_t)n + 1;   /* we now have values up to n */
    return a_cache[(size_t)n];
}
