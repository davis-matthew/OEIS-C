/*
 * A242214  Numbers n not divisible by 10 such that n^3 contains at least one
 *          0.
 * https://oeis.org/A242214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 753, u64 n <= 2500, u128 n <= 2500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242214: nth number not divisible by 10 whose cube contains
   at least one decimal zero digit.  The function searches sequentially
   until the n‑th such number has been found, caching the results so that
   non‑monotone calls are efficient. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for previously found terms                         */
    static seq_t *cache = NULL;  /* cache of results */
    static size_t cache_len = 0; /* how many we have stored */
    static size_t cache_cap = 0; /* capacity of cache array */
    static idx_t cur = 0;        /* next candidate to test */

    /* Grow cache as needed */
    if (cache == NULL) {
        cache_cap = 8;          /* initial capacity */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* Find additional terms until we have enough */
    while (cache_len < n) {
        cur++;                    /* next integer */
        if (cur % 10 == 0)        /* skip multiples of 10 */
            continue;

        /* Compute cube in seq_t to avoid overflow in smaller types */
        seq_t cube = (seq_t)cur;
        cube = cube * (seq_t)cur;
        cube = cube * (seq_t)cur;

        /* Check whether cube contains a '0' digit */
        bool has0 = false;
        while (cube > 0) {
            if (cube % 10 == 0) {
                has0 = true;
                break;
            }
            cube /= 10;
        }

        if (has0) {
            /* Ensure cache has room for the new item */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_len++] = cur;
        }
    }

    return cache[n - 1];
}
