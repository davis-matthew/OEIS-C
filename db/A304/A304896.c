/*
 * A304896  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3, 4
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for the sequence A304896. * * Base values (for 1 ≤ n
 *            ≤ 16): * * a(1) = 1 * a(2) = 23 * a(3) = 62 * a(4) = 330 * a(5)
 *            = 1249 * a(6) = 5562 * a(7) = 23101 * a(8) = 99064 * a(9) =
 *            418753 * a(10) = 1782180 * a(11) = 7560125 * a(12) = 32121500 *
 *            a(13) = 136371438 * a(14) = 579184407 * a(15)...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* -------------------------------------------------------------------
     * Static cache to store previously computed terms.
     * ------------------------------------------------------------------- */
    static seq_t *cache = NULL;          /* array of terms 0 .. cache_cap-1 */
    static idx_t cache_cap = 0;          /* allocated size (>= needed indices + 1) */
    static idx_t max_computed = 0;       /* highest index that has a valid entry */

    /* -------------------------------------------------------------------
     * Pre‑computed first 16 terms, stored modulo the width of seq_t.
     * They are computed once with a 128‑bit integer to avoid truncation
     * before the cast.
     * ------------------------------------------------------------------- */
    static seq_t init[16];
    static int init_done = 0;

    if (!init_done)
    {
        __int128 tmp[16] = {
            1,
            23,
            62,
            330,
            1249,
            5562,
            23101,
            99064,
            418753,
            1782180,
            7560125,
            32121500,
            136371438,
            579184407,
            2459401732,
            10444343450
        };
        for (idx_t i = 0; i < 16; ++i)
            init[i] = (seq_t)tmp[i];
        init_done = 1;
    }

    /* Undefined for n < 1 – return 0 for safety. */
    if (n == 0)
        return 0;

    /* -------------------------------------------------------------------
     * Ensure the cache is large enough to hold index n.
     * ------------------------------------------------------------------- */
    if (cache_cap < n + 1)
    {
        /* Allocate a new cache of size n + 1 elements. */
        seq_t *new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache)
            return 0;          /* allocation failure – unlikely for these limits */

        /* Copy previous values if any. */
        if (cache)
        {
            for (idx_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        cache      = new_cache;
        cache_cap  = n + 1;
    }

    /* -------------------------------------------------------------------
     * Initialise the base cases if they haven't been set yet.
     * ------------------------------------------------------------------- */
    if (max_computed == 0)
    {
        idx_t up = (cache_cap > 16) ? 16 : (idx_t)(cache_cap - 1);
        for (idx_t i = 1; i <= up; ++i)
            cache[i] = init[i - 1];
        max_computed = up;
    }

    /* Return the pre‑computed value for small n. */
    if (n <= max_computed)
        return cache[n];

    /* -------------------------------------------------------------------
     * Compute missing terms up to n.
     * For indices 1 .. 16, the base values are already stored.
     * For indices > 16, apply the recurrence.
     * ------------------------------------------------------------------- */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i <= 16)
        {
            cache[i] = init[i - 1];
            continue;
        }

        seq_t term = 0;
        term += cache[i - 1];
        term += cache[i - 2] * (seq_t)12;
        term += cache[i - 3] * (seq_t)11;
        term -= cache[i - 4] * (seq_t)12;
        term -= cache[i - 5] * (seq_t)15;
        term += cache[i - 6] * (seq_t)14;
        term += cache[i - 7] * (seq_t)14;
        term += cache[i - 8] * (seq_t)26;
        term += cache[i - 9] * (seq_t)5;
        term += cache[i -10];
        term -= cache[i -11];
        term -= cache[i -12] * (seq_t)17;
        term += cache[i -13];
        term -= cache[i -14] * (seq_t)5;
        term += cache[i -15] * (seq_t)2;
        cache[i] = term;
    }

    max_computed = n;
    return cache[n];
}
