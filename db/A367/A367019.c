/*
 * A367019  a(n) is the number of strictly decreasing sequences (w_1, ...,
 *          w_k) such that w_1 = n, for m = 1..k-1, w_{m+1} is obtained by
 *          removing one significant binary digit from w_m, and w_k = 0.
 * https://oeis.org/A367019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of a(child) over distinct children * obtained by
 *            deleting one binary digit from n. * For n = 0, a(0) = 1. * * The
 *            implementation uses memoisation; each integer is * computed once
 *            and stored in a dynamic cache. Children * are deduplicated to
 *            avoid counting identical intermediate * numbe...
 * Range:     u8 n <= 72, u16 n <= 596, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declarations of seq_t arithmetic are assumed to be
 * available via the build system; we use only + and assignments.   */

static seq_t *cached_vals  = NULL;   /* cache of computed a(n)          */
static bool   *cached_seen = NULL;   /* flags whether a(n) is computed  */
static idx_t   cache_cap   = 0;      /* current capacity of the cache   */

/* Ensure that the cache can store at least indices up to n.  */
static void
ensure_cache_capacity(idx_t n)
{
    if (n < cache_cap)
        return;                      /* already big enough          */

    idx_t new_cap = (n < 4 ? 4 : n * 2 + 1);  /* grow enough          */
    seq_t *new_vals = (seq_t *)realloc(cached_vals,
                                       new_cap * sizeof(seq_t));
    bool   *new_seen = (bool   *)realloc(cached_seen,
                                        new_cap * sizeof(bool));
    if (!new_vals || !new_seen) {
        /* In a real system we would handle an out‑of‑memory error. */
        abort();
    }
    cached_vals  = new_vals;
    cached_seen = new_seen;

    for (idx_t i = cache_cap; i < new_cap; ++i)
        cached_seen[i] = false;

    cache_cap = new_cap;
}

/* Helper: floor of log2 for non‑zero unsigned 64‑bit integers. */
static idx_t
floor_log2_u64(idx_t v)
{
    idx_t h = 0;
    while (v >> (h + 1))
        ++h;
    return h;
}

/* Main recursive function.  */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* a(0) = 1 */

    ensure_cache_capacity(n);
    if (cached_seen[n])
        return cached_vals[n];        /* already computed */

    /* Compute a(n) recursively. */
    idx_t h = floor_log2_u64(n);       /* number of bits minus one  */
    seq_t sum = 0;

    /* Store unique children to avoid duplicate counting. */
    seq_t uniq[65];
    idx_t uniq_cnt = 0;

    for (idx_t i = 0; i <= h; ++i) {
        idx_t high = n >> (i + 1);
        idx_t low  = n & (((idx_t)1 << i) - 1);
        idx_t child = (high << i) | low;

        /* Check if child already seen among this n's children. */
        bool seen = false;
        for (idx_t j = 0; j < uniq_cnt; ++j) {
            if (uniq[j] == child) {
                seen = true;
                break;
            }
        }
        if (seen)
            continue;                  /* skip duplicate child   */

        uniq[uniq_cnt++] = child;      /* remember this child    */
        sum += a(child);              /* add its count          */
    }

    cached_vals[n]  = sum;
    cached_seen[n]  = true;
    return sum;
}
