/*
 * A330888  Irregular triangle read by rows: T(n,k) is the number of parts in
 *          the partition of n into k consecutive parts that differ by 3, n >=
 *          1, k >= 1, and the first element of column k is in the row that is
 *          the k-th pentagonal number (A000326).
 * https://oeis.org/A330888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 33, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A330888
 * The sequence is obtained by reading an irregular triangle.
 * For a given row n (n ≥ 1) and column k (k ≥ 1) with pentagonal
 * number P(k)=k(3k−1)/2 ≤ n:
 *   T(n,k) = k if (n−P(k)) % k == 0
 *   T(n,k) = 0 otherwise
 * a(n) is the flattened triangle: a(1) is the first element of the triangle,
 * a(2) the second, and so on.
 * This code builds the triangle incrementally and memoises the values.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;      /* memoisation buffer */
static size_t cache_len   = 0;       /* number of elements stored */
static size_t cache_cap   = 0;       /* buffer capacity */
static idx_t  last_row    = 0;       /* last row that has been processed */

static void
ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* unrecoverable allocation failure – terminate */
        abort();
    }
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Compute and extend the cache until it contains at least 'to' elements */
static void
extend_cache_to(size_t to)
{
    if (to <= cache_len)
        return;
    ensure_capacity(to);
    for (idx_t row = last_row + 1; cache_len < to; ++row) {
        for (idx_t k = 1;; ++k) {
            idx_t pent = k * (3 * k - 1) / 2;
            if (pent > row)
                break;
            seq_t val = ((row - pent) % k == 0) ? (seq_t)k : (seq_t)0;
            seq_cache[cache_len++] = val;
        }
        last_row = row;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined offset, return 0 */
        return (seq_t)0;
    /* the sequence is 1‑based, cache indices are 0‑based */
    size_t idx = (size_t)n;
    if (idx > cache_len)
        extend_cache_to(idx);
    return seq_cache[idx - 1];
}
