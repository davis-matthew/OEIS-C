/*
 * A393749  The total number of at-most-5-furcating labeled histories that can
 *          be constructed for n labeled leaves.
 * https://oeis.org/A393749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(3) = 4, a(4) = 29 for n >= 5: a(n) =
 *            (n*(n-1)/2)*a(n-1) + (n*(n-1)*(n-2)/6)*a(n-2) +
 *            (n*(n-1)*(n-2)*(n-3)/24)*a(n-3) +
 *            (n*(n-1)*(n-2)*(n-3)*(n-4)/120)*a(n-4)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_cap = 0;     /* currently allocated capacity */
    static idx_t  max_computed = 0; /* largest n for which cache[i] is valid */

    if (n == 0u)
        return (seq_t)0;             /* undefined for n = 0 */

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                  /* allocation failed – return 0 */
            return (seq_t)0;
        }
        /* Initialise any new entries to 0 */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms that have not yet been filled in */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1u)
                cache[i] = (seq_t)1;
            else if (i == 2u)
                cache[i] = (seq_t)1;
            else if (i == 3u)
                cache[i] = (seq_t)4;
            else if (i == 4u)
                cache[i] = (seq_t)29;
            else {
                seq_t c1 = ((seq_t)i) * ((seq_t)(i - 1)) / (seq_t)2;
                seq_t c2 = ((seq_t)i) * ((seq_t)(i - 1)) * ((seq_t)(i - 2)) / (seq_t)6;
                seq_t c3 = ((seq_t)i) * ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * ((seq_t)(i - 3)) / (seq_t)24;
                seq_t c4 = ((seq_t)i) * ((seq_t)(i - 1)) * ((seq_t)(i - 2)) * ((seq_t)(i - 3)) * ((seq_t)(i - 4)) / (seq_t)120;
                cache[i] = c1 * cache[i - 1] + c2 * cache[i - 2] + c3 * cache[i - 3] + c4 * cache[i - 4];
            }
        }
        max_computed = n;
    }

    return cache[n];
}
