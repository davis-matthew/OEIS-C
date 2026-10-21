/*
 * A279768  Numbers n such that the sum of digits of 8n equals 16.
 * https://oeis.org/A279768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   allocate_seq_array, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer m such that digit_sum(8*m) == 16 */

#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/allocate_seq_array.h"

/* static state for memoisation */
static seq_t *cache       = NULL;  /* array of found terms */
static size_t cache_len   = 0;     /* number of terms stored */
static size_t cache_cap   = 0;     /* allocated capacity of cache */
static idx_t last_checked = 0;     /* last integer examined */

/* allocate an array of seq_t of given length, works in C and C++ */

/* free an array allocated by allocate_seq_array */
static void deallocate_seq_array(seq_t *p, size_t n) {
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

/* compute the n-th element of A279768 */
seq_t a(idx_t n) {
    /* if cache already has the term, return it */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    const seq_t target = (seq_t)16;   /* digit sum we seek */

    /* extend cache until it contains the n-th element */
    while (cache_len < (size_t)n) {
        last_checked++;

        /* test candidate */
        seq_t val = (seq_t)last_checked * (seq_t)8;
        if (digit_sum(val) == target) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_arr = allocate_seq_array(new_cap);
                if (!new_arr) abort();            /* allocation failure */
                for (size_t i = 0; i < cache_len; ++i)
                    new_arr[i] = cache[i];
                deallocate_seq_array(cache, cache_cap);
                cache = new_arr;
                cache_cap = new_cap;
            }
            /* store found term */
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
