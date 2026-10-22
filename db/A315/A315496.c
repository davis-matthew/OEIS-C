/*
 * A315496  Coordination sequence Gal.5.306.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=6, a(2)=11, a(3)=16, a(4)=22, a(5)=28,
   a(6)=34, a(7)=40, a(8)=45, a(9)=50, a(10)=56.
   For n > 10:
   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5)
          - 2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10).
*/

#include <stdlib.h>

/* Initial terms of the sequence. */
static const seq_t init_seq[11] = {
    1, 6, 11, 16, 22, 28, 34, 40, 45, 50, 56
};

/* Cache for computed values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate base terms. */
        cache = (seq_t*)malloc(11 * sizeof(seq_t));
        if (!cache) return;                /* allocation failure – caller gets garbage */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = init_seq[i];
        cache_len = 11;
    }
    if (n < cache_len) return;             /* already computed */

    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return;                      /* allocation failure */
    cache = tmp;
    for (idx_t i = cache_len; i < new_len; ++i) {
        seq_t val = 2 * cache[i-1] - 2 * cache[i-2] + 2 * cache[i-3]
                  - 2 * cache[i-4] + 2 * cache[i-5]
                  - 2 * cache[i-6] + 2 * cache[i-7]
                  - 2 * cache[i-8] + 2 * cache[i-9]
                  - cache[i-10];
        cache[i] = val;
    }
    cache_len = new_len;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 11) return init_seq[n];
    ensure_cache(n);
    return cache[n];
}
