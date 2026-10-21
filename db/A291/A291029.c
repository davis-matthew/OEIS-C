/*
 * A291029  p-INVERT of the positive integers, where p(S) = 1 - S - S^2 - S^3.
 * https://oeis.org/A291029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 18*a(n-2) + 25*a(n-3) - 18*a(n-4) + 7*a(n-5) -
 *            a(n-6) a(0)=1, a(1)=4, a(2)=15, a(3)=55, a(4)=198, a(5)=706
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
    #include <cstddef>   /* for size_t */
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array holding computed terms */
    static idx_t cache_size = 0;         /* number of valid entries in cache */

    /* Initial values for the recurrence */
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)15,
        (seq_t)55,
        (seq_t)198,
        (seq_t)706
    };

    /* The cache needs at least 6 entries to hold the initial terms */
    idx_t target = n + 1;
    if (target < 6) target = 6;          /* ensure space for base cases */

    if (cache_size < target) {           /* need to grow the cache */
        /* Remember old size before allocation */
        idx_t old_size = cache_size;

        /* Allocate new storage */
        seq_t *new_cache = NULL;
#if defined(__cplusplus)
        new_cache = new seq_t[target];
#else
        new_cache = (seq_t *)malloc(target * sizeof(seq_t));
#endif
        if (!new_cache) return (seq_t)0;  /* out‑of‑memory guard */

        /* Copy existing values into the new buffer */
        for (idx_t i = 0; i < old_size; ++i)
            new_cache[i] = cache[i];

        /* Free old buffer */
#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif

        cache = new_cache;
        cache_size = target;

        /* Initialize any missing base terms */
        for (idx_t i = old_size; i < 6 && i < cache_size; ++i)
            cache[i] = init[i];

        /* Compute new terms using the recurrence */
        idx_t start = (old_size > 6) ? old_size : 6;
        for (idx_t i = start; i < cache_size; ++i)
        {
            seq_t val = 7 * cache[i - 1];
            val -= 18 * cache[i - 2];
            val += 25 * cache[i - 3];
            val -= 18 * cache[i - 4];
            val += 7 * cache[i - 5];
            val -= cache[i - 6];
            cache[i] = val;
        }
    }

    return cache[n];
}
