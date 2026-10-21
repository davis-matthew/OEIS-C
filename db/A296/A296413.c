/*
 * A296413  a(1) = a(2) = a(3) = 1, a(4) = 4, a(5) = 3; a(n) = a(n-a(n-1)) +
 *          a(n-a(n-2)) + a(n-a(n-3)) for n > 5.
 * https://oeis.org/A296413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) with
 *            a(1)=a(2)=a(3)=1, a(4)=4, a(5)=3
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 16 : *cap;
    while (newcap < needed) newcap <<= 1;
    void *tmp = realloc(*ptr, newcap * elem_size);
    if (!tmp) abort();          /* terminate on allocation failure */
    *ptr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* highest index that has been computed */

    /* Ensure we can address index n. */
    ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Initialise the first five terms if needed. */
    if (cache_len < 5)
    {
        for (idx_t i = cache_len + 1; i <= 5 && i <= n; ++i)
        {
            if (i == 1 || i == 2 || i == 3)
                cache[i] = (seq_t)1;
            else if (i == 4)
                cache[i] = (seq_t)4;
            else            /* i == 5 */
                cache[i] = (seq_t)3;
            cache_len = i;
        }
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t prev3 = cache[i - 3];

        idx_t idx1 = i - (idx_t)prev1;
        idx_t idx2 = i - (idx_t)prev2;
        idx_t idx3 = i - (idx_t)prev3;

        seq_t val = cache[idx1] + cache[idx2] + cache[idx3];
        cache[i] = val;
    }
    cache_len = n;

    return cache[n];
}
