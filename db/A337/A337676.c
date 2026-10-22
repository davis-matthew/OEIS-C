/*
 * A337676  a(0) = 1; a(n) = -(n!)^3 * Sum_{k=0..n-1} a(k) / (k! * (n-k))^3.
 * https://oeis.org/A337676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 9, i128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A337676
   a(0) = 1
   a(n) = -(n!)^3 * Sum_{k=0..n-1} a(k) / (k! * (n-k))^3   (n≥1)
   The implementation stores values in a static cache so that calls
   may appear in any order.  Factorials are pre‑computed once per
   requested maximum index.  All arithmetic uses the seq_t type
   and relies on the property that
     (n! / k!) is divisible by (n-k), hence
     ((n! / k!)^3) / (n-k)^3                (1)
   is an exact integer, eliminating the need for rational
   arithmetic.  The algorithm is O(n²) w.r.t. the number of terms
   computed, which is fast enough for the required limits. */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t) */
#include "../utils/free_cache.h"        /* free_cache(seq_t *) */

static seq_t *cache       = NULL;   /* cache for a(k) values             */
static size_t cache_cap   = 0;       /* current capacity of cache         */
static idx_t  max_computed = 0;      /* largest n for which a(n) stored  */

static seq_t *fact_arr    = NULL;   /* factorials n! (n=0..max_n)        */
static size_t fact_cap     = 0;      /* current capacity of factorials    */
static idx_t  fact_last    = 0;      /* highest factorial index computed  */

/* Ensure that cache and factorial arrays are large enough for indices
   up to n and that factorials 0..n are computed.  The arrays are
   enlarged by doubling until sufficient.  */
static void ensure_cache(idx_t n)
{
    /* --- Allocate cache if needed ----------------------------------- */
    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;                /* a(0) = 1                */
        max_computed = 0;
    }
    /* Grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        free_cache(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* --- Allocate factorial array if needed ------------------------ */
    if (fact_arr == NULL) {
        fact_cap = 16;
        fact_arr = alloc_seq_array(fact_cap);
        fact_arr[0] = (seq_t)1;              /* 0! = 1                    */
        fact_last  = 0;
    }
    if ((size_t)n >= fact_cap) {
        size_t new_cap = fact_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *new_fact = alloc_seq_array(new_cap);
        for (size_t i = 0; i < fact_cap; ++i) new_fact[i] = fact_arr[i];
        free_cache(fact_arr);
        fact_arr = new_fact;
        fact_cap = new_cap;
    }
    /* Compute missing factorials up to n */
    for (idx_t i = fact_last + 1; i <= n; ++i) {
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
    }
    fact_last = n;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ensure resources are ready */
    ensure_cache(n);

    /* compute terms up to n if not already done */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t Q   = fact_arr[m];          /* Q = m! */

        /* loop over k = 0 .. m-1 */
        for (idx_t k = 0; k < m; ++k) {
            seq_t divisor = (seq_t)(m - k);          /* (m-k)   */
            seq_t R       = Q / divisor;             /* Q/(m-k) */
            seq_t factor  = R * R * R;               /* R^3     */
            sum += cache[k] * factor;

            /* prepare Q for next k (divide by k+1) */
            Q = Q / (seq_t)(k + 1);
        }
        cache[m] = -sum;   /* a(m) = - sum */
    }
    max_computed = n;
    return cache[n];
}
