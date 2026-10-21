/*
 * A241144  Numbers n such that n and 8n share at least one digit.
 * https://oeis.org/A241144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A241144: Numbers n such that n and 8n share at least one digit.
 * a(1) = 10, a(2) = 13, a(3) = 14, ...
 * This implementation generates the terms on demand using a simple search.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: test whether n and 8*n have a common digit */
static inline bool share_digit(seq_t n)
{
    unsigned int mask1 = 0, mask2 = 0;
    seq_t tmp = n;

    if (tmp == (seq_t)0) mask1 |= 1;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        mask1 |= 1U << d;
        tmp /= (seq_t)10;
    }

    tmp = (seq_t)8 * n;
    if (tmp == (seq_t)0) mask2 |= 1;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);
        mask2 |= 1U << d;
        tmp /= (seq_t)10;
    }

    return (mask1 & mask2) != 0;
}

/*
 * Returns the n-th term of the sequence (1‑based indexing).
 * The function keeps an internal cache so that repeated calls are efficient.
 */
seq_t a(idx_t n)
{
    /* Static state: cache of terms found so far */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = (seq_t)1;          /* next value to test */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        if (share_digit(next_candidate)) {
            /* Grow storage if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp      = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE); /* allocation fault */
                cache  = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }

    return cache[(size_t)n - 1];
}
