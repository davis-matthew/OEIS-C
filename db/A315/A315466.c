/*
 * A315466  Coordination sequence Gal.6.158.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = a(n‑1) + d(n), where d(n) = 6 if
 *            n%11==0 or (n%11==1 && n>1), d(n) = 4 if n%11==6, d(n) = 5
 *            otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for malloc, realloc */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure the cache can hold value for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
        if (cache_len == 0) {
            cache[0] = (seq_t)1; /* a(0) */
            cache_len = 1;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t diff;
            unsigned mod = (unsigned)(i % 11);
            if (mod == 0) {
                diff = (seq_t)6;
            } else if (mod == 1) {
                diff = (i > 1) ? (seq_t)6 : (seq_t)5;
            } else if (mod == 6) {
                diff = (seq_t)4;
            } else {
                diff = (seq_t)5;
            }
            cache[i] = cache[i - 1] + diff;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
