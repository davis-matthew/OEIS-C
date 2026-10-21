/*
 * A295009  Numbers k such that the largest digit of k^2 is 9.
 * https://oeis.org/A295009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1237, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A295009: Numbers k such that the largest digit of k^2 is 9
 * (i.e. k^2 contains the digit 9).
 * We generate the sequence on demand, caching the results.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration for dynamic array helpers (provided by the system). */
extern void *ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size);

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;          /* array of terms   */
    static size_t cache_len = 0;         /* number of terms stored */
    static size_t cache_cap = 0;         /* current capacity   */
    static idx_t next_candidate = 1;     /* next integer to test */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        seq_t square = (seq_t)next_candidate * next_candidate;

        /* Check if the square contains digit 9. */
        seq_t tmp = square;
        bool has9 = false;
        while (tmp > 0)
        {
            if (tmp % 10 == 9)
            {
                has9 = true;
                break;
            }
            tmp /= 10;
        }

        if (has9)
        {
            /* Ensure capacity and store the term. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)ensure_capacity(new_cap, &cache_cap,
                                                  (void **)&cache,
                                                  sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }

        next_candidate++;
    }

    return cache[n - 1];
}

/* Simple allocator that uses realloc and returns the new pointer */
void *ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed > *cap)
    {
        size_t new_cap = needed * 2;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return *ptr;
}
