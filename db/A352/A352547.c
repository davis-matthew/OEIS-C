/*
 * A352547  Numbers having more odd than even digits when written in base 10.
 * https://oeis.org/A352547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth positive integer having more odd than even decimal digits. */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Edge case: n >= 1 guaranteed by specification. */
    seq_t candidate = 1;          /* current integer being tested   */
    idx_t   found    = 0;          /* how many satisfying numbers found */

    for (;;) {
        seq_t temp   = candidate;
        unsigned odd  = 0;
        unsigned even = 0;

        /* Count odd and even digits of temp. */
        while (temp > 0) {
            unsigned digit = (unsigned)(temp % 10);
            if (digit & 1U)
                ++odd;
            else
                ++even;
            temp /= 10;
        }

        if (odd > even) {
            ++found;
            if (found == n)
                return candidate;
        }

        ++candidate;
    }
}
