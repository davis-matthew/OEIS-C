/*
 * A331944  a(n)/ceiling(6^(n-7)) is the expected number of rolls of a fair
 *          6-sided die in a game where the player starts at 0, advances the
 *          position by the outcome of the die's roll until exactly position n
 *          is reached. Positions beyond n are avoided by staying at the last
 *          visited position, but counting the...
 * https://oeis.org/A331944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 30, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) … a(13) = 6,6,6,6,6,6,7,43,265,1639,10177,63463,397585
 *   For n > 13:
 *     a(n) = 7 * a(n-1) - 46656 * a(n-7)
 *
 * The function memoizes computed values so that any order of calls is
 * supported in O(1) time after the first pass.
 */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* initial values 1 … 13 */
    static const seq_t init[13] = {
        6, 6, 6, 6, 6, 6, 7, 43, 265, 1639,
        10177, 63463, 397585
    };

    /* cache of all computed terms */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* highest n for which cache[n-1] is valid */

    /* handle small indices directly */
    if (n <= 13u)
        return init[n - 1u];

    /* allocate cache for initial 13 terms on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(13u * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory (unlikely) */
        for (idx_t i = 0; i < 13u; ++i)
            cache[i] = init[i];
        max_computed = 13u;
    }

    /* extend cache if needed */
    if (n > max_computed) {
        idx_t new_size = n;
        seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (p == NULL) return 0;               /* out of memory (unlikely) */
        cache = p;

        /* compute missing terms: use recurrence a(n) = 7*a(n-1) - 46656*a(n-7) */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i - 1] =
                7 * cache[i - 2] - 46656 * cache[i - 8];
        }
        max_computed = n;
    }

    return cache[n - 1];
}
