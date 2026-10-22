/*
 * A360360  Given a deck of colored cards, move the top card below the
 *          bottom-most card of the same color, with one other card between
 *          them. (If the top and bottom cards have the same color, the top
 *          card is moved to the bottom of the deck; if there is no other card
 *          of the same color, the top card is moved on...
 * https://oeis.org/A360360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^floor((n-1)/2) for n >= 3, with a(1)=1, a(2)=2
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* handle the first two special cases */
    if (n <= 1)            /* n == 1 or n == 0 (offset not used but harmless) */
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    /* for n >= 3 compute 2^floor((n-1)/2) */
    idx_t exp = (n - 1) >> 1;   /* floor((n-1)/2) */
    return pow2(exp);
}
