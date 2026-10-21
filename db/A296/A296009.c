/*
 * A296009  Smallest number m such that (2n-1)*m has only odd digits.
 * https://oeis.org/A296009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = smallest m such that (2n-1)*m has only odd decimal digits
 * Range:     u8 n <= 10, u16 n <= 1947, u32 n <= 8247, u64 n <= 8247,
 *            u128 n <= 8247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache for memoisation */
static seq_t *cache = NULL;       /* sequence values */
static idx_t  cache_cap = 0;      /* allocated space (in seq_t elements) */
static idx_t  cache_len = 0;      /* number of values stored */

/* Test whether all decimal digits of x are odd.
   Assumes x is non‑negative. */
static bool has_only_odd_digits(seq_t x)
{
    if (x == 0)            /* zero contains the even digit 0 */
        return false;
    while (x > 0)
    {
        seq_t d = x % (seq_t)10;       /* current least‑significant digit */
        if ((d & (seq_t)1) == 0)      /* even digit found */
            return false;
        x /= (seq_t)10;                /* drop the digit */
    }
    return true;
}

/* Helper to grow the cache with malloc/realloc. */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;                       /* already big enough */

    idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < needed)
        new_cap <<= 1;

    if (cache == NULL)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    cache_cap = new_cap;
}

/* Main sequence function.  a(1) is the first term listed in the OEIS b‑file. */
seq_t a(idx_t n)
{
    if (n == 0)                    /* the sequence is defined for n>=1; return 0 for safety */
        return (seq_t)0;

    grow_cache(n);                  /* make sure the cache holds the required index */

    /* compute values that have not yet been cached */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t d = (seq_t)2 * (seq_t)i - (seq_t)1;   /* d = 2*i-1, always odd */
        seq_t m = (seq_t)1;                         /* start with the smallest odd m */

        while (true)
        {
            seq_t prod = d * m;                     /* compute (2i-1)*m */
            if (has_only_odd_digits(prod))         /* lucky product */
            {
                cache[i - 1] = m;                  /* store m for this n */
                break;
            }
            m += (seq_t)2;                         /* try the next odd m */
        }
    }

    cache_len = n;
    return cache[n - 1];
}
