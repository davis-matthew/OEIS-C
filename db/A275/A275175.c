/*
 * A275175  a(n) = (2 * a(n-3) + a(n-1) * a(n-5)) / a(n-6), a(0) = a(1) = ...
 *          = a(5) = 1.
 * https://oeis.org/A275175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*a(n-3) + a(n-1)*a(n-5)) / a(n-6),
 *            a(0)=a(1)=...=a(5)=1. For n ≥ 15 the linear recurrence a(n) =
 *            35*a(n-5) - 35*a(n-10) + a(n-15) holds
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms, indexed 0.. */
    static idx_t cache_size = 0;  /* number of elements currently stored */

    /* Ensure the cache is large enough for index n. */
    if (cache_size <= n)
    {
        idx_t new_size = n + 1;
        size_t bytes = (size_t)new_size * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, bytes);
        if (!tmp) exit(1);          /* out of memory – abort */
        cache = tmp;

        /* Compute new entries in increasing order. */
        for (idx_t i = cache_size; i < new_size; ++i)
        {
            if (i <= 5)
            {
                cache[i] = (seq_t)1; /* initial values */
            }
            else if (i <= 14)
            {
                /* Non‑linear division recurrence (exact division). */
                seq_t val = (seq_t)2 * cache[i - 3]
                          + cache[i - 1] * cache[i - 5];
                val = val / cache[i - 6];
                cache[i] = val;
            }
            else
            {
                /* Linear recurrence of order 15. */
                seq_t val = (seq_t)35 * cache[i - 5]
                          - (seq_t)35 * cache[i - 10]
                          + cache[i - 15];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
