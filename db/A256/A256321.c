/*
 * A256321  Number of partitions of 5n into exactly 3 parts.
 * https://oeis.org/A256321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6);
   initial values: a(0)=0,a(1)=2,a(2)=8,a(3)=19,a(4)=33,a(5)=52 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t stored = 0;

    /* Ensure the internal array is large enough. */
    if (n >= (idx_t)cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)(n + 1))
            newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return a safe fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* Compute values up to and including n. */
    for (size_t i = stored; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)8;
        } else if (i == 3) {
            cache[i] = (seq_t)19;
        } else if (i == 4) {
            cache[i] = (seq_t)33;
        } else if (i == 5) {
            cache[i] = (seq_t)52;
        } else {
            /* Recurrence for i >= 6. */
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-6];
            cache[i] = sum - cache[i-4] - cache[i-5];
        }
    }

    stored = (size_t)n + 1;
    return cache[(size_t)n];
}
