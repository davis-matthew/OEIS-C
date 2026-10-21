/*
 * A296760  Numbers whose base-16 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_more_rises
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth number whose base‑16 digits have more rises than falls
   by brute‑force search. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/has_more_rises.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)1;
    idx_t count = 0;

    for (;; ++x) {
        if (has_more_rises(x)) {
            ++count;
            if (count == n) {
                return x;
            }
        }
    }
}
