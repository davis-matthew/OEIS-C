/*
 * A333834  Multiples of 10 whose sum of digits is 10.
 * https://oeis.org/A333834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A333834:  Multiples of 10 whose sum of digits is 10
 *  Generation via brute‑force search with memoisation.
 *  a(n) = 10 * (n-th integer whose digit sum equals 10).
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Cache of already generated terms */
    static seq_t *cache = NULL;     /* array of a(1), a(2), … */
    static size_t cache_cap = 0;    /* allocated size of cache */
    static idx_t term_count = 0;    /* how many terms are stored */
    static seq_t next_candidate = 1;/* next integer to test (starting from 1) */

    if (n == 0) return 0; /* offset is 1, but guard against 0 */

    /* Ensure the cache is large enough for n terms */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (term_count < n) {
        if (digit_sum(next_candidate) == (seq_t)10) {
            cache[term_count] = next_candidate * (seq_t)10;
            ++term_count;
        }
        ++next_candidate;
    }

    return cache[(size_t)(n - 1)];
}
