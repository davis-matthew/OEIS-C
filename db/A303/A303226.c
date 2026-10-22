/*
 * A303226  Number of minimal total dominating sets in the n-gear graph.
 * https://oeis.org/A303226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - 3*a(n-4) - a(n-5) - a(n-6) +
 *            a(n-9) for n > 9 Initial values (n = 1..9): 0, 6, 12, 6, 30, 30,
 *            56, 110, 156
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[10] = { // index 0 unused
    (seq_t)0,              /* dummy for index 0 */
    (seq_t)0,              /* a(1) */
    (seq_t)6,              /* a(2) */
    (seq_t)12,             /* a(3) */
    (seq_t)6,              /* a(4) */
    (seq_t)30,             /* a(5) */
    (seq_t)30,             /* a(6) */
    (seq_t)56,             /* a(7) */
    (seq_t)110,            /* a(8) */
    (seq_t)156             /* a(9) */
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* allocated size, including index 0 */
    static idx_t computed_up_to = 0;      /* highest n already computed */

    if (n < 1) return (seq_t)0;  /* behaviour below offset irrelevant */

    if (n > computed_up_to) {
        /* enlarge cache if necessary */
        if (n + 1 > cache_len) {
            size_t new_len = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failure; terminate */
            cache = tmp;
            cache_len = new_len;
        }

        /* initialise new entries */
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = init[i];
            } else {
                seq_t v1 = cache[i - 1];
                seq_t v2 = cache[i - 2];
                seq_t v3 = cache[i - 3];
                seq_t v4 = cache[i - 4];
                seq_t v5 = cache[i - 5];
                seq_t v6 = cache[i - 6];
                seq_t v9 = cache[i - 9];
                cache[i] = v1 + (seq_t)2 * v2 + v3 - (seq_t)3 * v4 - v5 - v6 + v9;
            }
        }
        computed_up_to = n;
    }
    return cache[n];
}
