/*
 * A279114  Number of non-equivalent ways to place 5 non-attacking kings on an
 *          n X n board.
 * https://oeis.org/A279114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n>=1, non-equivalent ways to place 5 non-attacking
 *            kings) It is computed using the polynomial for 5 <= n <= 20 and
 *            a 17‑term linear recurrence for n >= 21. Indexing follows the
 *            OEIS convention: a(1) is the first term in the list. The
 *            sequence is memoised so calls in any order are efficien...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 18, u64 n <= 167, u128 n <= 1000,
 *            big any n
 * Helpers:   poly_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/poly_term.h"

/* ----- main function ----- */
seq_t a(idx_t n)
{
    /* memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored (a(1)..a(cache_len)) */

    /* helper: compute term for any n (using formula or recursion) */
    static const int coeff[17] =
        { 5, -4, -20, 40, 16, -100, 44, 110, -110, -44,
          100, -16, -40, 20, 4, -5, 1 };

    /* ensure we have enough space */
    if (n > cache_len) {
        /* allocate new array of size n */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        if (!new_cache) return 0;        /* allocation failure: return 0 */

        /* copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i < 5) {
                new_cache[i - 1] = 0;
            } else if (i <= 20) {
                new_cache[i - 1] = poly_term(i);
            } else {
                signed __int128 sum = 0;
                for (int m = 1; m <= 17; ++m) {
                    sum += (__int128)coeff[m - 1] *
                           (__int128)new_cache[i - m - 1];
                }
                new_cache[i - 1] = (seq_t)sum;
            }
        }

        /* free old cache and replace */
        free(cache);
        cache = new_cache;
        cache_len = n;
    }

    return cache[n - 1];
}
