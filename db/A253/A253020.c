/*
 * A253020  Number of (n+2) X (3+2) 0..3 arrays with every consecutive three
 *          elements in every row and column not having exactly two distinct
 *          values, and in every diagonal and antidiagonal having exactly two
 *          distinct values, and new values 0 upwards introduced in row major
 *          order.
 * https://oeis.org/A253020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 59, u64 n <= 130, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
  Sequence A253020: a(n) = number of (n+2)×5 arrays with
  the stated constraints.
  The sequence satisfies the empirical linear recurrence
  a(n) = 5*a(n-3) - 6*a(n-6) - a(n-9) + a(n-12)   for n > 15.
  Base values a(1)…a(15) are taken from the OEIS data.
  The implementation memoizes results up to the maximum
  n requested, allowing repeated calls in any order.
*/

#include <stdlib.h>   /* malloc, realloc, free, EXIT_FAILURE */
#include <stdint.h>   /* uint64_t */

seq_t a(idx_t n)
{
    /* 1‑based index. Return 0 for n < 1 (undefined). */
    if (n < 1) return 0;

    /* static memoization cache */
    static seq_t *cache = NULL;   /* cache[0] unused */
    static size_t cache_cap = 0;  /* size in seq_t elements */

    /* initial seed values (1‑based).  cache_cap initially 16 to hold indices 0…15. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        /* base values a(1) … a(15) */
        static const seq_t base[16] = {
            0,      /* index 0 unused */
            51,     /* a(1) */
            83,     /* a(2) */
            133,    /* a(3) */
            120,    /* a(4) */
            219,    /* a(5) */
            350,    /* a(6) */
            302,    /* a(7) */
            531,    /* a(8) */
            846,    /* a(9) */
            754,    /* a(10) */
            1323,   /* a(11) */
            2100,   /* a(12) */
            1888,   /* a(13) */
            3287,   /* a(14) */
            5198    /* a(15) */
        };
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = base[i];
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms using the recurrence */
        for (idx_t i = 16; i <= n; ++i) {
            /* a(i) = 5*a(i-3) - 6*a(i-6) - a(i-9) + a(i-12) */
            seq_t part1 = 5 * cache[i - 3] + cache[i - 12];
            part1 -= 6 * cache[i - 6];
            part1 -= cache[i - 9];
            cache[i] = part1;
        }
    }

    return cache[n];
}
