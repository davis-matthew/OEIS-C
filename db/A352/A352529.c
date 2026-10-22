/*
 * A352529  Expansion of 1/Sum_{k>=0} x^(k^4).
 * https://oeis.org/A352529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(n) = -∑_{k≥1, k⁴≤n} a(n−k⁴) (for n≥1)
 * Range:     i8 n <= 48, i16 n <= 106, i32 n <= 199, i64 n <= 401,
 *            i128 n <= 807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1/(∑_{k≥0} x^(k⁴)) = 1 + a(1)x + a(2)x² + …   */

#include <stdlib.h>
#include <stdint.h>

/* Cached values of the sequence */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache can hold terms up to index n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;                       /* already enough */

    /* Save the old cache size before growing */
    idx_t old_cap = cache_size;
    idx_t new_cap = n + 1;

    /* Reallocate the cache array */
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr)                      /* allocation failed */
        abort();

    seq_cache = new_arr;
    cache_size = new_cap;

    /* Compute any new entries, starting from old_cap */
    for (idx_t m = old_cap; m < new_cap; ++m) {
        if (m == 0) {                  /* a(0) = 1 */
            seq_cache[m] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t t = 1;; ++t) {
            idx_t t4 = t * t * t * t;  /* t⁴, fits in idx_t for n ≤ 10000 */
            if (t4 > m)
                break;
            sum -= seq_cache[m - t4];
        }
        seq_cache[m] = sum;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
