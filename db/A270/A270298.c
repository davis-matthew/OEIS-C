/*
 * A270298  Numbers which are representable as a sum of eight but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 3744, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n)=8*t+28 where t is the (n‑1)th nonnegative integer
   that is not congruent to 1 (mod 3), 4 (mod 5), or 0 (mod 7).
   This generates the numbers representable as a sum of eight (but not fewer)
   consecutive non‑negative integers. */

#include <stdlib.h>
#include <stdint.h>

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *cache = NULL;     /* array of a(i) values */
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* current capacity of cache */
    static idx_t next_t = 0;        /* next t to test for validity */

    if (n == 0)
        return (seq_t)0;           /* offset begins at 1; below does not matter */

    /* Grow the cache until we have at least n terms. */
    while (cache_size < n)
    {
        /* Find next valid t. */
        while (1)
        {
            /* Conditions: t ≠ 1 (mod 3), t ≠ 4 (mod 5), t ≠ 0 (mod 7) */
            if ((next_t % 3 != 1) && (next_t % 5 != 4) && (next_t % 7 != 0))
                break;
            ++next_t;
        }

        /* Compute the term: a = 8*t + 28. */
        seq_t term = (seq_t)8 * next_t + (seq_t)28;

        /* Ensure cache capacity. */
        if (cache_size == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();          /* memory allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = term;
        ++next_t;
    }

    return cache[n - 1];
}
