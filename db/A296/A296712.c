/*
 * A296712  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9841 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th element of OEIS A296712:
 * Numbers whose decimal digits have an equal number of rises and falls.
 * a(1) = 1.  The function searches natural numbers in increasing order
 * and counts rises (digit(i) < digit(i+1)) and falls (digit(i) > digit(i+1)).
 */
#include <stdbool.h>

static bool equal_rises_falls(seq_t x)
{
    /* Count rises and falls in the decimal representation of x. */
    if (x < (seq_t)10)   /* single digit: 0 rises, 0 falls */
        return true;
    seq_t prev = x % (seq_t)10;  /* least significant digit */
    x /= (seq_t)10;
    int rises = 0, falls = 0;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)10;
        if (cur > prev)  rises++;
        else if (cur < prev) falls++;
        prev = cur;
        x /= (seq_t)10;
    }
    return rises == falls;
}

seq_t a(idx_t n)
{
    /* Find the n-th integer satisfying the rise/fall condition. */
    idx_t found = 0;
    seq_t cur = (seq_t)0;
    for (;;) {                      /* loop until we have n values */
        cur++;
        if (equal_rises_falls(cur)) {
            found++;
            if (found == n)
                return cur;
        }
    }
}
