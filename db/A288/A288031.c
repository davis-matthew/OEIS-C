/*
 * A288031  Number of edge covers in the grid graph P_3 X P_n.
 * https://oeis.org/A288031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) + 100*a(n-2) + 24*a(n-3) - 95*a(n-4) +
 *            10*a(n-5) + 8*a(n-6) for n >= 6 a(0) = 1, a(1) = 1, a(2) = 43,
 *            a(3) = 969, a(4) = 23663, a(5) = 571099
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed values.  The cache is expanded as needed
   and keeps all previously computed terms so that repeated calls
   with arbitrary n values are O(1) after the first computation of
   the maximum n requested. */
static seq_t *cache = NULL;
static idx_t cache_size = 0; /* next free index; number of stored terms */
static idx_t cache_cap = 0;  /* allocated capacity */

/* Ensure the cache can hold terms up to index n inclusively. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = (n + 1 > 2 * cache_cap + 1) ? (n + 1) : (2 * cache_cap + 1);
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A288031. */
seq_t a(idx_t n)
{
    if (n >= cache_size) {
        ensure_cache(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)43;
            } else if (i == 3) {
                cache[i] = (seq_t)969;
            } else if (i == 4) {
                cache[i] = (seq_t)23663;
            } else if (i == 5) {
                cache[i] = (seq_t)571099;
            } else {
                cache[i] = 20 * cache[i - 1]
                         + 100 * cache[i - 2]
                         + 24 * cache[i - 3]
                         - 95 * cache[i - 4]
                         + 10 * cache[i - 5]
                         + 8 * cache[i - 6];
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
