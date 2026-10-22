/*
 * A392542  Expansion of 1 / ((1-x)^4 - x^6).
 * https://oeis.org/A392542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-6), a(0) =
 *            1, a(1) = 4, a(2) = 10, a(3) = 20, a(4) = 35, a(5) = 56, a(6) =
 *            85
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 77, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 0..6 */
    static const seq_t init[7] =
    {
        (seq_t)1ULL, (seq_t)4ULL, (seq_t)10ULL,
        (seq_t)20ULL, (seq_t)35ULL, (seq_t)56ULL,
        (seq_t)85ULL
    };

    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* Ensure cache large enough for n */
    if (n + 1 > cache_cap)
    {
        idx_t new_cap = (n + 1 > 7 ? n + 1 : 7);
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache)
            return (seq_t)0; /* memory failure – unlikely in tests */

        /* Copy previous computed values if any */
        if (cache)
        {
            for (idx_t i = 0; i < cache_cap; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* Initialize base terms if not already present */
        for (idx_t i = 0; i < new_cap; ++i)
        {
            if (i < 7)
                new_cache[i] = init[i];
            else
                new_cache[i] = (seq_t)0; /* placeholder, will compute */
        }

        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if needed */
    if (cache[n] == (seq_t)0) /* uncomputed */
    {
        for (idx_t i = 7; i <= n; ++i)
        {
            seq_t val = 0;
            val += (seq_t)4 * cache[i - 1];
            val -= (seq_t)6 * cache[i - 2];
            val += (seq_t)4 * cache[i - 3];
            val -= cache[i - 4];
            val += cache[i - 6];
            cache[i] = val;
        }
    }

    return cache[n];
}
