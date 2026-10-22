/*
 * A327139  Numbers k such that cos(2k) > cos(2k+2) < cos(2k+4).
 * https://oeis.org/A327139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A327139:
 *   A(n) = the n-th positive integer k such that cos(2k) > cos(2k+2) < cos(2k+4).
 *   The condition is equivalent to
 *     sin(2k+1) > 0  and  sin(2k+3) < 0.
 *   The sequence is computed by a simple brute‑force scan of successive k,
 *   keeping a static cache of found values so that each call only needs to
 *   extend the cache to the requested index.
 */

#include <stdlib.h>
#include <math.h>   /* for sin() */

/* Global cache for a(n) values */
static seq_t *cache      = NULL;   /* dynamic array of computed terms */
static size_t cache_len  = 0;      /* number of terms currently cached   */
static size_t cache_cap  = 0;      /* current allocation capacity        */
static idx_t  last_k     = 1;      /* next k to test (starts at 1)       */

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is undefined for n = 0; return 0 for safety */
        return 0;

    /* extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* compute the two sine values needed for the local‑min check */
        double ang1 = (double)last_k * 2.0 + 1.0;   /* 2k + 1   */
        double ang3 = (double)last_k * 2.0 + 3.0;   /* 2k + 3   */
        double s1   = sin(ang1);
        double s3   = sin(ang3);

        if (s1 > 0.0 && s3 < 0.0)        /* sin(2k+1)>0 and sin(2k+3)<0 → local minimum */
        {
            /* grow the cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)          /* memory allocation failed */
                    return 0;
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
        ++last_k;
    }

    /* return the requested term (0‑based index cache_len-1) */
    return cache[n - 1];
}
