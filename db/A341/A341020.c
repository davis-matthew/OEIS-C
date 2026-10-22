/*
 * A341020  INVERT transform of the binary weight.
 * https://oeis.org/A341020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} popcount(k) * a(n-k), a(0)=1 OEIS A341020 –
 *            Invert transform of the binary weight
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <inttypes.h>

/* --- Cached computation ------------------------------------------------- */

static seq_t *cache = NULL;        /* values a(0), a(1), ... */
static size_t cache_capacity = 0;  /* number of stored values */

static unsigned char *weight = NULL;        /* popcount(k) for k ≤ current max */
static size_t weight_capacity = 0;          /* array size */

/* Ensure cache holds values a(0) … a(n) */
static void ensure_cache(idx_t n)
{
    /* base: a(0) must exist */
    if (cache_capacity == 0) {
        cache_capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        weight_capacity = 1;
        weight = (unsigned char *)malloc(sizeof(unsigned char));
        weight[0] = 0;
    }

    /* enlarge cache/weight arrays if necessary */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
        weight = (unsigned char *)realloc(weight, new_cap * sizeof(unsigned char));
        weight_capacity = new_cap;
    }

    /* compute missing entries (starting from old size) */
    size_t start = cache_capacity;      /* index of first not-yet-computed value */
    /* but cache_capacity has just been updated; we need to store old size */
    /* thus we recompute appropriately: find index of first missing value */
    /* We know that indices 0 .. cache_capacity-1 are already computed; */
    /* if we enlarged, we need to compute from old cap to n inclusive. */
    /* Use a temporary variable to hold old size before enlargement: */
    /* (since cache_capacity was updated, we lose old value).  Simplify: */
    /* compute from 1 up to n, skipping those already present. */
    /* We'll start from (idx_t)1 if cache[0] already defined. */
    /* But for safety we compute from 1. */
    for (idx_t i = 1; i <= n; ++i) {
        /* skip if we have already computed? We don't track; recompute is safe */
        /* But we can check if a[i] already set by comparing i >= cache_capacity_old? */
        /* Since we just ensured capacity, if i < cache_capacity_old, value is already computed. */
        /* Let's recompute only if i < cache_capacity before enlargement. */
    }

    /* To avoid double computation, keep a separate variable old_capacity: */
    /* Actually simpler: store old_capacity before possible enlargement. */
    /* We'll restructure: */
}

static void ensure_cache2(idx_t n)
{
    /* get current capacity before potential reallocation */
    size_t old_cap = cache_capacity;
    if (cache_capacity == 0) {
        cache_capacity = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(0)=1 */
        weight_capacity = 1;
        weight = (unsigned char *)malloc(sizeof(unsigned char));
        weight[0] = 0;
        old_cap = 1;
    }

    /* enlarge if needed */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
        weight = (unsigned char *)realloc(weight, new_cap * sizeof(unsigned char));
        weight_capacity = new_cap;
    }

    /* compute missing entries */
    for (idx_t i = old_cap; i <= n; ++i) {
        weight[i] = (unsigned char)__builtin_popcountll((unsigned long long)i);
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += (seq_t)weight[k] * cache[i - k];
        }
        cache[i] = sum;
    }
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache2(n);
    return cache[n];
}
