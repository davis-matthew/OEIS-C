/*
 * A270263  Integers divisible by the last row of their "upside-down" triangle
 *          (see comments).
 * https://oeis.org/A270263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer >=10 such that the integer is divisible
 *            by the last row of its "upside-down" triangle, where the
 *            triangle is obtained by repeatedly taking absolute differences
 *            of adjacent decimal digits until only one digit remains
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   compute_last_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/compute_last_row.h"

/* global cache storage for the sequence */
static seq_t *seq_cache       = NULL;
static size_t seq_cache_len   = 0;
static size_t seq_cache_cap   = 0;
static seq_t  next_check      = 10;

/* enlarge the cache if necessary */
static void
ensure_cache(size_t needed)
{
    if (needed <= seq_cache_cap)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();              /* insufficient memory */
    seq_cache     = new_arr;
    seq_cache_cap = new_cap;
}

/* fill the cache up to the n‑th term (1‑based) */
static void
build_up_to(size_t n)
{
    if (n <= seq_cache_len)
        return;

    ensure_cache(n);
    for (seq_t cand = next_check; seq_cache_len < n; ++cand) {
        seq_t last = compute_last_row(cand);
        if (last != 0 && cand % last == 0) {
            seq_cache[seq_cache_len++] = cand;
        }
    }
    next_check = seq_cache[seq_cache_len - 1] + 1;
}

/* public function required by the tester */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined; return 0 for safety */
    build_up_to((size_t)n);
    return seq_cache[n - 1];
}
