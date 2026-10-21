/*
 * A294661  Numbers whose square contains all of the digits 1 through 9.
 * https://oeis.org/A294661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest positive integer whose square contains
 *            all digits 1–9 at least once
 * Range:     u8 none, u16 none, u32 n <= 971, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>              /* size_t */
#include "../utils/grow_seq.h"   /* ensure_capacity for dynamic arrays */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = 1;  /* next integer to test */

    /* Ensure we have at least n terms cached */
    while (cache_len < (size_t)n) {
        seq_t sq = (seq_t)next_candidate * (seq_t)next_candidate;

        unsigned mask = 0;
        seq_t tmp = sq;
        while (tmp > 0) {
            unsigned digit = (unsigned)(tmp % (seq_t)10);
            if (digit != 0) mask |= 1u << digit;
            tmp /= (seq_t)10;
        }

        /* All digits 1–9 present iff bits 1–9 are set */
        if ((mask & 0x3FEu) == 0x3FEu) {
            if (cache_len + 1 > cache_cap) {
                /* grow_seq ensures capacity at least needed */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
