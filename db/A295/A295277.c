/*
 * A295277  a(n) = number of distinct earlier terms that have no common one
 *          bit with n in binary representation.
 * https://oeis.org/A295277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ a(k) : 0 < k < n and (a(k) & n) == 0 }
 * Range:     u8 n <= 9738, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoises results and keeps a list of distinct previous
   terms.  For each new index n, it counts how many distinct previous
   values have no common 1‑bit with n.  This satisfies the O(n²) worst‑case
   bound and is fast enough for n up to about 10 000. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of a(1)..a(m)             */
    static size_t cache_cap = 0;         /* current cache size              */
    static seq_t *distinct = NULL;       /* array of distinct previous a(k) */
    static size_t distinct_cap = 0;      /* capacity of distinct array       */
    static size_t distinct_len = 0;      /* number of distinct values stored */

    if (n == 0)            /* unused: offset starts at 1 */
        return 0;

    /* extend the cache up to n if necessary */
    if ((size_t)n > cache_cap) {
        /* enlarge the cache */
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)   /* allocation failed – abort */
            exit(1);
        cache = new_cache;

        /* compute a(i) for all new indices */
        for (idx_t i = (idx_t)(cache_cap + 1); i <= n; ++i) {
            seq_t nin = (seq_t)i;
            seq_t count = 0;

            /* count distinct earlier a(k) that have no common bit with n */
            for (size_t j = 0; j < distinct_len; ++j)
                if ((distinct[j] & nin) == 0)
                    ++count;

            cache[i - 1] = count;

            /* insert the new value into the distinct set if absent */
            bool present = false;
            for (size_t j = 0; j < distinct_len; ++j)
                if (distinct[j] == count) {
                    present = true;
                    break;
                }

            if (!present) {
                if (distinct_len == distinct_cap) {
                    size_t new_dist_cap = distinct_cap ? distinct_cap * 2 : 4;
                    seq_t *tmp = (seq_t *)realloc(distinct,
                        new_dist_cap * sizeof(seq_t));
                    if (!tmp)
                        exit(1);
                    distinct = tmp;
                    distinct_cap = new_dist_cap;
                }
                distinct[distinct_len++] = count;
            }
        }
        cache_cap = n;
    }

    return cache[n - 1];
}
