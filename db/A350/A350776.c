/*
 * A350776  Nonnegative integers whose balanced ternary expansion has no two
 *          consecutive nonzero digits.
 * https://oeis.org/A350776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   _valid_no_consecutive_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function enumerates nonnegative integers whose balanced ternary
   expansion has no two consecutive nonzero digits (digits ±1).  It
   searches sequentially from 0 upwards and returns the n‑th such number
   (n is 1‑based: a(1) = 0). */
#include <stdbool.h>
#include "../utils/_valid_no_consecutive_nonzero.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined behaviour for n==0
                                            but return 0 just in case      */
    idx_t count = (idx_t)0;
    seq_t cand = (seq_t)0;
    for (;;) {
        if (_valid_no_consecutive_nonzero(cand)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
        ++cand;
    }
}
