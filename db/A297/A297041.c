/*
 * A297041  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-13
 *          digits of n; see Comments.
 * https://oeis.org/A297041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A297041: Number of pieces in the list of base‑13 digits of n.
 *  A piece is a maximal contiguous segment where the digits are
 *  strictly increasing, strictly decreasing, or equal.  The number
 *  of pieces is 1 plus the number of sign changes between successive
 *  digits, with an equality (0 difference) counted as a change.
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* numbers 1 … 12 have only one base‑13 digit → 0 pieces */
    if (n < 13u)
        return 0;

    /* collect base‑13 digits, least significant first */
    uint8_t digits[64];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 13u);
        tmp /= 13u;
    }

    /* at least two digits now */
    seq_t pieces = 1;
    int lastdiff = 0;                /* 0: undefined or equality, 1: asc, -1: desc */

    for (int i = len - 1; i >= 1; --i) {
        uint8_t higher = digits[i];
        uint8_t lower  = digits[i - 1];
        int diff;
        if (higher < lower)
            diff = 1;                /* ascending */
        else if (higher > lower)
            diff = -1;               /* descending */
        else
            diff = 0;                /* equal */

        if (i == len - 1) {          /* first comparison */
            if (diff != 0)
                lastdiff = diff;
        } else {
            if (diff != lastdiff) {
                pieces++;
                lastdiff = diff;
            }
        }
    }
    return pieces;
}
