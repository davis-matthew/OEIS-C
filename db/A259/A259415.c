/*
 * A259415  Triangular numbers (A000217) that are the sum of seventeen
 *          consecutive triangular numbers.
 * https://oeis.org/A259415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A259415: Triangular numbers that equal the sum of 17 consecutive triangular numbers.
 * Recurrence (valid for n >= 10):
 *     a(n) = a(n-1) + 4354 * a(n-4) - 4354 * a(n-5) - a(n-8) + a(n-9)
 * Initial terms (offset = 1):
 *     a(1) = 1326
 *     a(2) = 9180
 *     a(3) = 24531
 *     a(4) = 1979055
 *     a(5) = 5325216
 *     a(6) = 39529386
 *     a(7) = 106368405
 *     a(8) = 8616365901
 *     a(9) = 23185550130
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_size     = 0;
    static size_t cache_capacity = 0;
    
    /* initialize cache with first nine terms */
    if (cache_size == 0) {
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort(); /* out of memory */

        cache[0] = (seq_t)1326;
        cache[1] = (seq_t)9180;
        cache[2] = (seq_t)24531;
        cache[3] = (seq_t)1979055;
        cache[4] = (seq_t)5325216;
        cache[5] = (seq_t)39529386;
        cache[6] = (seq_t)106368405;
        cache[7] = (seq_t)8616365901;
        cache[8] = (seq_t)23185550130;
        cache_size = 9;
    }

    /* generate terms up to requested index */
    while (cache_size < n) {
        /* grow storage if necessary */
        if (cache_size >= cache_capacity) {
            size_t new_cap = cache_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* out of memory */
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* compute next term by the recurrence */
        size_t i = cache_size;             /* i is zero–based index of the new term (a(i+1)) */
        seq_t val = cache[i - 1];          /* a(n-1)   */

        val += (seq_t)4354 * cache[i - 4]; /* +4354*a(n-4) */
        val -= (seq_t)4354 * cache[i - 5]; /* -4354*a(n-5) */
        val -= cache[i - 8];               /* -a(n-8)   */
        val += cache[i - 9];               /* +a(n-9)   */

        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
