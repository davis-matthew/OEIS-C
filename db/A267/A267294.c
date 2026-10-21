/*
 * A267294  Decimal representation of the n-th iteration of the "Rule 121"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 256*a(n-4) - 256*a(n-6) for n > 12 Initial
 *            values (OEIS A267294): a(0) = 1, a(1) = 1, a(2) = 26, a(3) = 59,
 *            a(4) = 476, a(5) = 751, a(6) = 7024, a(7) = 16319, a(8) =
 *            115136, a(9) = 195327, a(10) = 1776384, a(11) = 4177919, a(12) =
 *            29474816
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..12 */
    static const seq_t init_vals[13] = {
        (seq_t)1,    (seq_t)1,    (seq_t)26,    (seq_t)59,
        (seq_t)476,  (seq_t)751,  (seq_t)7024,  (seq_t)16319,
        (seq_t)115136, (seq_t)195327, (seq_t)1776384,
        (seq_t)4177919, (seq_t)29474816
    };

    /* Handle small n directly */
    if (n < 13)
        return init_vals[n];

    /* Cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (cache_len = last_index+1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Allocate or grow cache if needed */
    if (cache_len <= n) {
        size_t new_cap = cache_cap;
        if (new_cap < n + 1)
            new_cap = n + 1;

        if (cache == NULL) {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!cache)
                return (seq_t)0; /* allocation failure, should never happen for small n */
            /* Initialize first 13 terms */
            for (idx_t i = 0; i < 13; ++i)
                cache[i] = init_vals[i];
            cache_len = 13;
        } else if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to index n */
        for (size_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i - 2];
            val += (seq_t)256 * cache[i - 4];
            val -= (seq_t)256 * cache[i - 6];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
