/*
 * A297042  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-14
 *          digits of n; see Comments.
 * https://oeis.org/A297042
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

/* Computes OEIS A297042: number of monotone pieces in the base‑14
   digit representation of n.  A piece is a maximal contiguous
   subsequence of digits that is either strictly increasing, strictly
   decreasing, or constant, and cannot be extended left or right
   while preserving that monotonicity. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to base‑14 digits (least significant first). */
    uint8_t tmp[64];
    size_t len = 0;
    idx_t x = n;
    do {
        tmp[len++] = (uint8_t)(x % (idx_t)14);
        x /= (idx_t)14;
    } while (x != 0);

    if (len <= 1) {
        return (seq_t)0;          /* single digit → no piece */
    }

    /* Reverse to get most significant digit first. */
    uint8_t digits[64];
    for (size_t i = 0; i < len; ++i) {
        digits[i] = tmp[len - 1 - i];
    }

    seq_t pieces = 0;
    for (size_t i = 0; i + 1 < len; ++i) {
        int r;
        if (digits[i] < digits[i + 1]) {
            r = 1;                /* increasing */
        } else if (digits[i] > digits[i + 1]) {
            r = -1;               /* decreasing */
        } else {
            r = 0;                /* constant */
        }

        /* Extend run while relation remains the same. */
        size_t k = i + 1;
        while (k + 1 < len) {
            int s;
            if (digits[k] < digits[k + 1]) {
                s = 1;
            } else if (digits[k] > digits[k + 1]) {
                s = -1;
            } else {
                s = 0;
            }
            if (s != r) {
                break;
            }
            ++k;
        }

        /* If the previous digit shares the same relation,
           this run is not maximal and will be counted from the
           earlier start index. */
        if (i > 0) {
            int t;
            if (digits[i - 1] < digits[i]) {
                t = 1;
            } else if (digits[i - 1] > digits[i]) {
                t = -1;
            } else {
                t = 0;
            }
            if (t == r) {
                continue;          /* not a maximal piece */
            }
        }

        ++pieces;
    }

    return pieces;
}
