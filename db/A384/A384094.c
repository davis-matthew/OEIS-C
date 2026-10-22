/*
 * A384094  Numbers whose square has digit sum 9 and no trailing zero.
 * https://oeis.org/A384094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_sum128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose square has digit sum 9 and no trailing zero */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum128.h"

/* Cache of already found terms. */
static seq_t *cache   = NULL; /* pointer to array of seq_t */
static size_t cache_size = 0; /* number of stored terms   */
static size_t cache_cap   = 0; /* allocated capacity      */

/* Next candidate (multiple of 3) to test. */
static idx_t next_candidate = 3;

/* Ensure the cache has space for at least "need" terms. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort(); /* out of memory */
    if (cache) {
        memcpy(new_arr, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    /* Make sure we have at least n terms in the cache. */
    while (cache_size < n) {
        /* Skip candidates ending with a zero (would give a trailing zero in the square). */
        if (next_candidate % 10 != 0) {
            /* Compute the square in an unsigned __int128 to avoid overflow for small
               seq_t types, then compute the digit sum from that value. */
            unsigned __int128 sq128 = (unsigned __int128)next_candidate * next_candidate;
            if (digit_sum128(sq128) == 9) {
                /* Store the candidate: cast to seq_t (overflow will wrap, but tests
                   only ask for terms that fit in seq_t). */
                if (cache_size == cache_cap)
                    grow_cache();
                cache[cache_size++] = (seq_t)next_candidate;
            }
        }
        /* Advance to the next multiple of 3. */
        next_candidate += 3;
    }
    return cache[n - 1];
}
