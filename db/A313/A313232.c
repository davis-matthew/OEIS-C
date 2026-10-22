/*
 * A313232  Coordination sequence Gal.3.14.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4; for n>=2, a(n)=a(n-1)+d where d follows the
 *            repeating pattern [5,6,5,4,4] (period 5)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values currently stored */

    /* Ensure cache is allocated and contains a(0)=1 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory guard (should not happen) */
        cache[0] = (seq_t)1;
    }

    /* Expand cache up to index n if required */
    if ((size_t)n >= cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* out‑of‑memory guard (should not happen) */
        cache = tmp;
        for (size_t i = old_len; i < new_len; ++i) {
            seq_t diff;
            if (i == 1) {
                diff = (seq_t)3;
            } else {
                /* Pattern 5,6,5,4,4 repeats for i >= 2 */
                size_t pos = (i - 2) % 5;
                switch (pos) {
                    case 0: diff = (seq_t)5; break;
                    case 1: diff = (seq_t)6; break;
                    case 2: diff = (seq_t)5; break;
                    case 3: diff = (seq_t)4; break;
                    case 4: diff = (seq_t)4; break;
                    default: diff = (seq_t)0;      /* unreachable */
                }
            }
            cache[i] = cache[i - 1] + diff;
        }
        cache_len = new_len;
    }

    return cache[n];
}
