/*
 * A257223  Numbers that have at least one divisor containing the digit 6 in
 *          base 10.
 * https://oeis.org/A257223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   has_digit6_uint64, has_divisor_with_digit6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: numbers that have at least one divisor
   whose decimal representation contains the digit 6. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/has_digit6_uint64.h"
#include "../utils/has_divisor_with_digit6.h"

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined below offset */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of terms stored     */
    static size_t cache_cap   = 0;      /* capacity of cache array     */
    static idx_t next_k = 1;           /* next integer to test       */

    /* extend cache until we have n terms */
    while (cache_size < (size_t)n) {
        /* ensure enough room */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* unrecoverable memory error */
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* find next number that satisfies the property */
        while (!has_divisor_with_digit6(next_k))
            ++next_k;

        /* store it */
        cache[cache_size++] = (seq_t)next_k++;
    }

    return cache[n - 1];
}
