/*
 * A248330  The product of the first n Catalan numbers and the number of
 *          standard Young tableaux of shape(1,2,...,n).
 * https://oeis.org/A248330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (product_{k=0..n} C_k) * A003046(n) where C_k is the k-th
 *            Catalan number and A003046(n) = ( n(n+1)/2 )! / \prod_{j=1..n}
 *            (2j-1)^{\,n-j+1}
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   catalan, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/catalan.h"   // provides seq_t catalan(idx_t)
#include "../utils/seq_alloc_new_array.h"

/* Helper to allocate/reallocate seq_t arrays */
static seq_t *seq_alloc_new_array(size_t n);
static void seq_realloc_ptr(seq_t **p, size_t *cap, size_t new_cap);

static seq_t *cache = NULL;          /* will hold a(0..max_cached) */
static size_t cache_cap = 0;         /* allocated number of slots */
static idx_t  max_cached = 0;        /* highest index stored */

/* state kept between calls for incremental construction */
static seq_t cat_prod  = 1;          /* product_{k=0..i} C_k for current i   */
static seq_t odd_prod  = 1;          /* product_{k=1..i} (2k-1)              */
static seq_t aSYT      = 1;          /* A003046(i) for current i             */
static idx_t  T_cur    = 0;          /* T(i) = i(i+1)/2 for current i       */

/* Allocate or grow cache */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = seq_alloc_new_array(new_cap);
        } else {
            seq_realloc_ptr((seq_t**)&cache, &cache_cap, new_cap);
        }
        cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) is always 1 */

    ensure_cache_cap(n);

    /* if we already computed up to n, return cached value */
    if (max_cached >= n)
        return cache[n];

    /* if this is the first request, initialise the state */
    if (cache[0] == 0) {
        cache[0] = (seq_t)1;    /* A005118(0) = 1, A003046(0) = 1 */
        cat_prod  = 1;          /* product_{k=0..0} C_k = C_0 = 1   */
        odd_prod  = 1;          /* product of odd numbers up to -1 is 1 */
        aSYT      = 1;          /* A003046(0) = 1                   */
        T_cur     = 0;          /* T(0) = 0                           */
        max_cached = 0;
    }

    /* Build values from max_cached+1 up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* ------------------------------------------------------------------
           1. Compute A003046(i) from A003046(i-1)
           ------------------------------------------------------------------ */
        /* Product of numbers T_cur+1 .. T_cur+i  = i numbers */
        seq_t mult = 1;
        for (idx_t t = T_cur + 1; t <= T_cur + i; ++t)
            mult *= (seq_t)t;

        /* Denominator increment: odd_prod * (2*i-1) */
        seq_t denom_inc = odd_prod * (seq_t)(2*i - 1);

        /* Update aSYT to aSYT(i) */
        aSYT *= mult;
        aSYT /= denom_inc;

        /* Update odd_prod and triangular number for next iteration */
        odd_prod *= (seq_t)(2*i - 1);
        T_cur += i;

        /* ------------------------------------------------------------------
           2. Compute product of Catalan numbers up to C_i
           ------------------------------------------------------------------ */
        cat_prod *= catalan(i);

        /* ------------------------------------------------------------------
           3. Compute a(i) and store
           ------------------------------------------------------------------ */
        seq_t val = cat_prod * aSYT;
        cache[i] = val;
    }

    max_cached = n;
    return cache[n];
}

/* Utility wrappers for allocation (simple malloc wrappers) */

static void seq_realloc_ptr(seq_t **p, size_t *cap, size_t new_cap)
{
    *p = (seq_t*)realloc(*p, new_cap * sizeof(seq_t));
}
