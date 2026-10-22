/*
 * A373229  Relative of Hofstadter Q-sequence: a(n) = n for 1 <= n <= 9; a(n)
 *          = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 9.
 * https://oeis.org/A373229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑a(n‑1)) + a(n‑a(n‑2)) + a(n‑a(n‑3)), initial values:
 *            a(n) = n for 1 ≤ n ≤ 9
 * Range:     u8 n <= 290, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;    /* array 0..cap-1  (cache[0] unused) */
    static idx_t   cap   = 0;      /* current array capacity minus 1 */
    static idx_t   computed = 0;   /* largest index that has a value */

    if (n == 0)                     /* the offset is 1, but guard anyway */
        return 0;

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap = 9;
        cache = (seq_t*)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;               /* unused */
        for (idx_t i = 1; i <= 9; ++i) cache[i] = (seq_t)i;
        computed = 9;
    }

    /* make sure the array is large enough for *n* */
    if (cap < n) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        for (idx_t i = cap + 1; i <= new_cap; ++i) cache[i] = 0;
        cap = new_cap;
    }

    /* if we already know a(n), just return it */
    if (n <= computed) return cache[n];

    /* compute all new terms up to n in increasing order */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t  idx1 = i - (idx_t)cache[i - 1];
        idx_t  idx2 = i - (idx_t)cache[i - 2];
        idx_t  idx3 = i - (idx_t)cache[i - 3];
        cache[i] = cache[idx1] + cache[idx2] + cache[idx3];
    }
    computed = n;
    return cache[n];
}
