/*
 * A325805  "Sloping octal numbers": write numbers in octal under each other
 *          (right-justified), read diagonals in upward direction, convert to
 *          decimal.
 * https://oeis.org/A325805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value obtained by reading the upward diagonals of the
 *            right‑justified octal representation of the integers. The
 *            function implements the sequence defined by OEIS A325805. It
 *            works for all n ≥ 0, in O(number of digits of the result) time,
 *            which is O(log n)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* collect the octal digits that appear on diagonal n,
       from the smallest i upwards.  The maximum number of
       digits is very small, so a fixed small buffer suffices. */
    seq_t digits[64];
    size_t dcnt = 0;

    for (idx_t i = n; ; ++i) {
        idx_t j = i - n;          /* position from the rightmost digit */

        /* number of octal digits of i */
        idx_t tmp = i;
        size_t len = 1;
        while (tmp >= 8) {
            len++;
            tmp /= 8;
        }

        if (j >= (idx_t)len) break;   /* no further digit on this diagonal */

        /* digit at position j (0 = rightmost) */
        idx_t dtmp = i;
        for (idx_t k = 0; k < j; ++k) dtmp /= 8;
        digits[dcnt++] = (seq_t)(dtmp % 8);
    }

    /* build the decimal value from the collected digits,
       starting with the largest i (most significant digit). */
    seq_t res = 0;
    while (dcnt--) {
        res = res * ((seq_t)8) + digits[dcnt];
    }

    return res;
}
