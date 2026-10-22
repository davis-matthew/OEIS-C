/*
 * A307275  Partial sums of A307205.
 * https://oeis.org/A307275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+2*a(n-4)-2*a(n-5)-a(n-8)+a(n-9) for n>14
   with initial values a(0)=1, a(1)=5, a(2)=13, a(3)=27, a(4)=46,
   a(5)=70, a(6)=99, a(7)=135, a(8)=179, a(9)=227, a(10)=281,
   a(11)=339, a(12)=408, a(13)=476, a(14)=553 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static const seq_t init[15] = {
        1, 5, 13, 27, 46, 70, 99, 135, 179, 227,
        281, 339, 408, 476, 553
    };

    size_t idx = (size_t)n;

    if (cache == NULL) {
        /* Allocate space for the first 15 terms and copy them. */
        cache_size = 15;
        /* cast needed for C++ */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (size_t i = 0; i < 15; ++i) cache[i] = init[i];
    }

    if (idx < cache_size) return cache[idx];

    size_t old_size = cache_size;
    size_t new_size = idx + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    cache = tmp;
    cache_size = new_size;

    /* Only the recurrence is defined for n >= 15. */
    size_t start = old_size;
    if (start < 15) start = 15;

    for (size_t i = start; i <= idx; ++i) {
        cache[i] = cache[i-1]
                 + (seq_t)2 * cache[i-4]
                 - (seq_t)2 * cache[i-5]
                 - cache[i-8]
                 + cache[i-9];
    }

    return cache[idx];
}
