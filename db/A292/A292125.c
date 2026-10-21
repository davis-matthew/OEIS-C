/*
 * A292125  Positive numbers n such that the set of base-7 digits of n equals
 *          both the set of base-8 digits of n and the set of base-9 digits of
 *          n.
 * https://oeis.org/A292125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer whose sets of digits in * base
 *            7, base 8, and base 9 are identical (by value). The routine *
 *            searches numbers incrementally, caching found terms so that any
 *            call * with arbitrary n is answered in O(n) time
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Persistent cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_x = 1;            /* next number to test */

    if (n == 0) return (seq_t)0;           /* offset is 1, but safe */

    /* Compute terms until we have at least n terms stored */
    while (cache_len < (size_t)n) {
        uint64_t x   = next_x++;
        uint64_t t   = x;
        int mask7 = 0, mask8 = 0, mask9 = 0;

        /* Base‑7 digit mask */
        while (t) {
            mask7 |= 1 << (t % 7);
            t /= 7;
        }

        /* Base‑8 digit mask */
        t = x;
        while (t) {
            mask8 |= 1 << (t % 8);
            t /= 8;
        }

        /* Base‑9 digit mask */
        t = x;
        while (t) {
            mask9 |= 1 << (t % 9);
            t /= 9;
        }

        /* If all three masks agree, store the number */
        if (mask7 == mask8 && mask8 == mask9) {
            if (cache_len >= cache_cap) {
                if (cache_cap == 0)
                    cache_cap = 16;
                else
                    cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();           /* out of memory */
            }
            cache[cache_len++] = (seq_t)x;
        }
    }

    return cache[(size_t)n - 1];
}
