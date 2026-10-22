/*
 * A330436  a(n) = n * n!! - Sum_{k=1..n-1} k!! * a(n-k).
 * https://oeis.org/A330436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * n!! - Sum_{k=1..n-1} k!! * a(n-k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1) = 1, a(2) = 3, a(3) = 4, … (OEIS A330436) */

#include <stddef.h>
#include <stdlib.h>

/* Lazy cache for a(n) and for double factorials n!! (stored as seq_t) */
static seq_t *a_cache      = NULL;   /* a[0] unused, a[1] is first term */
static idx_t  a_cache_cap  = 0;      /* allocated capacity (in terms) */
static idx_t  a_cache_len  = 0;      /* highest n that has been computed */

static seq_t *dfact_cache  = NULL;   /* double factorials: dfact[n] = n!! */
static idx_t  dfact_cap    = 0;
static idx_t  dfact_len    = 0;      /* highest n that has been computed */

/* Ensure that the caches have room for indices up to n (inclusive).  If
   the capacity has to grow, use realloc and zero‑initialize new slots.  */
static void
ensure_cache(idx_t n)
{
    if (n < a_cache_cap) return;          /* already large enough */

    /* Grow to at least (n+1) slots, doubling each time. */
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap *= 2;

    seq_t *new_arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();                /* allocation failure */
    /* Zero‑initialize new slots (integer or POD types). */
    for (size_t i = a_cache_cap; i < new_cap; ++i)
        new_arr[i] = (seq_t)0;
    a_cache     = new_arr;
    a_cache_cap = new_cap;
}

static void
ensure_dfact_cache(idx_t n)
{
    if (n < dfact_cap) return;
    /* Reuse the same growth logic as before. */
    size_t new_cap = dfact_cap ? dfact_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap *= 2;

    seq_t *new_arr = (seq_t *)realloc(dfact_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    for (size_t i = dfact_cap; i < new_cap; ++i)
        new_arr[i] = (seq_t)0;
    dfact_cache  = new_arr;
    dfact_cap    = new_cap;
}

/* Precompute double factorials up to n (stored in dfact_cache).  */
static void
ensure_dfact(idx_t n)
{
    if (dfact_len >= n) return;           /* already computed */

    /* Make sure the backing array is large enough. */
    ensure_dfact_cache(n);

    /* Initialise the first two entries if necessary. */
    if (dfact_len < 0) dfact_len = 0;
    if (dfact_len == 0) {
        dfact_cache[0] = (seq_t)1;
        dfact_len = 0;
    }
    if (dfact_len < 1 && n >= 1) {
        dfact_cache[1] = (seq_t)1;
        dfact_len = 1;
    }

    /* Compute remaining double factorials iteratively. */
    for (idx_t i = dfact_len + 1; i <= n; ++i) {
        if (i == 0) {
            dfact_cache[i] = (seq_t)1;
        } else if (i == 1) {
            dfact_cache[i] = (seq_t)1;
        } else {
            dfact_cache[i] = (seq_t)i * dfact_cache[i - 2];
        }
    }
    if (dfact_len < n) dfact_len = n;
}

/* Main function required by the build.  Computes a(n) lazily and
   memoises results for arbitrary ordering of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */

    /* Ensure caches can hold value up to n. */
    ensure_cache(n);
    ensure_dfact(n);

    /* Compute terms up to n if not already computed. */
    for (idx_t k = a_cache_len + 1; k <= n; ++k) {
        /* sum_{j=1}^{k-1} j!! * a(k-j) */
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j < k; ++j) {
            sum += dfact_cache[j] * a_cache[k - j];
        }
        /* a(k) = k * k!! - sum */
        a_cache[k] = (seq_t)k * dfact_cache[k] - sum;
    }
    a_cache_len = n;
    return a_cache[n];
}
