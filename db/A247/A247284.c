/*
 * A247284  Subrecords in A048673: maximum value between two consecutive
 *          records in A048673.
 * https://oeis.org/A247284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) for n > 7. Initial values:
 *            a(1)=4, a(2)=6, a(3)=13, a(4)=18, a(5)=38, a(6)=63, a(7)=113
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms; zero-based internally */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* grow cache if needed */
    if (n > cache_len) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
        /* initialize new entries */
        for (size_t i = cache_len; i < new_cap; ++i)
            cache[i] = 0;
    }

    /* compute terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t val;
        /* i corresponds to a(i+1) because cache is zero-based */
        if (i == 0)         val = (seq_t)4;
        else if (i == 1)    val = (seq_t)6;
        else if (i == 2)    val = (seq_t)13;
        else if (i == 3)    val = (seq_t)18;
        else if (i == 4)    val = (seq_t)38;
        else if (i == 5)    val = (seq_t)63;
        else if (i == 6)    val = (seq_t)113;
        else { /* recurrence */
            seq_t a1 = cache[i-1];          /* a(i)  = a(i-1+1) */
            seq_t a2 = cache[i-2];          /* a(i-1) */
            seq_t a3 = cache[i-3];          /* a(i-2) */
            val = a1 + ((seq_t)3 * a2) - ((seq_t)3 * a3);
        }
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
