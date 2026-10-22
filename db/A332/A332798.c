/*
 * A332798  Numbers whose smallest prime factor is 19.
 * https://oeis.org/A332798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 621, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _spf19_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers whose smallest prime factor is 19
    The term a(n) is the nth positive integer whose smallest prime factor is 19.
    The algorithm generates successive multiples of 19 and retains those that
    are not divisible by any prime less than 19 (2,3,5,7,11,13,17).
    This guarantees the smallest prime factor is exactly 19.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_spf19_ok.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Fast return for the trivial first term */
    if (n == 1) return (seq_t)19;

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;

    /* Next multiplier of 19 to test */
    static uint64_t m_next = 1;   /* corresponds to 19 * m_next */

    /* Extend cache until we have at least n terms */
    while (cache_sz < n) {
        /* Make sure we have space for a new term */
        if (cache_sz >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next multiple of 19 satisfying the condition */
        for (;;) {
            unsigned __int128 prod = (unsigned __int128)19 * m_next;
            /* Since our values are small for the test sizes, cast is safe */
            uint64_t val = (uint64_t)prod;

            if (_spf19_ok(val)) {
                cache[cache_sz++] = (seq_t)val;
                m_next++;
                break;
            }
            m_next++;
        }
    }

    return cache[n - 1];
}
