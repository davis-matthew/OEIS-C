/*
 * A343327  Start with a(1)=1; thereafter a(n+1) = Sum_{k=1..n such that
 *          d(a(k)) = d(a(n))} d(a(k)) where d = A000005.
 * https://oeis.org/A343327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343327
   Recurrence definition:
   a(1) = 1;
   for n >= 1:  a(n+1) = Σ{d(a(k)) | 1 <= k <= n and d(a(k)) = d(a(n))}
   where d(x) = number of positive divisors of x (A000005).
   The implementation keeps, for each divisor count d, the current
   cumulative sum Σ d(a(k)) of all previously seen terms having that
   divisor count.  a(n+1) is obtained by looking up the sum for
   d(a(n)).  All values are stored in an array which is grown as
   needed.  The divisor count of a term is computed by converting it
   to uint64_t (the sequence values stay well below 2^64 for all
   n <= 10 000).  The routine therefore runs in O(n) time and
   depends only on the standard divisor‐count routine
   `divisor_count_u64`.  The function can be called in any order; it
   memoises all previously computed terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

static seq_t *a_cache = NULL;      /* array of sequence values (1‑based) */
static size_t  a_cap   = 0;        /* allocated capacity of a_cache */
static idx_t   last_computed = 0;  /* greatest n for which a_cache[n‑1] is valid */

static seq_t *sums_arr   = NULL;   /* sums_arr[d] = Σ d(a(k)) for all k seen with d(a(k))==d */
static size_t sums_cap   = 0;      /* allocated capacity of sums_arr */

/* ensure that sums_arr can index at least d_needed */
static void ensure_sums_capacity(uint64_t d_needed)
{
    if (d_needed >= sums_cap) {
        size_t new_cap = sums_cap ? sums_cap : 16;
        while (new_cap <= d_needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(sums_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory – terminate safely */
        sums_arr = tmp;
        /* initialise new entries to zero */
        for (size_t i = sums_cap; i < new_cap; ++i) sums_arr[i] = (seq_t)0;
        sums_cap = new_cap;
    }
}

/* ensure that a_cache can store at least n terms */
static void ensure_a_capacity(idx_t n)
{
    if (n > (idx_t)a_cap) {
        size_t new_cap = a_cap ? a_cap : 16;
        while ((idx_t)new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap = new_cap;
    }
}

/* compute a(n) for arbitrary n (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* unused, defensive */

    /* initialise static storage on first call */
    if (!a_cache) {
        a_cache = (seq_t*)malloc(16 * sizeof(seq_t));
        a_cap   = 16;
    }
    if (!sums_arr) {
        sums_cap = 16;
        sums_arr = (seq_t*)calloc(sums_cap, sizeof(seq_t));
    }

    /* grow the cache if needed */
    ensure_a_capacity(n);

    /* build terms up to n if we have not yet done so */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i == 1) {
                /* base case a(1) = 1 */
                a_cache[0] = (seq_t)1;
                uint64_t d_val = 1;  /* d(1) = 1 */
                ensure_sums_capacity(d_val);
                sums_arr[d_val] = (seq_t)d_val;  /* sums[1] = 1 after a(1) */
            } else {
                /* compute a(i) from a(i‑1) */
                uint64_t d_prev = divisor_count_u64((uint64_t)a_cache[(size_t)(i-2)]);
                ensure_sums_capacity(d_prev);
                seq_t val = sums_arr[d_prev];
                a_cache[(size_t)(i-1)] = val;

                /* update sums for the newly added term */
                uint64_t d_curr = divisor_count_u64((uint64_t)val);
                ensure_sums_capacity(d_curr);
                sums_arr[d_curr] += (seq_t)d_curr;
            }
        }
        last_computed = n;
    }

    return a_cache[(size_t)(n-1)];
}
