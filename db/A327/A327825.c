/*
 * A327825  "Rotation" of n: swap the first [d/2] and last [d/2] digits, when
 *          n has d digits.
 * https://oeis.org/A327825
 *
 * Offset:    979    (a(979) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = rotation of decimal digits of n by swapping the first
 *            floor(d/2) digits with the last floor(d/2) digits; for odd d the
 *            middle digit remains unchanged
 * Range:     u8 none, u16 n <= 1032, u32 n <= 1032, u64 n <= 1032,
 *            u128 n <= 1032, big any n
 * Validated: partial, pass, 54 terms through n = 1032 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    int d       = num_digits_idx(n);
    int half    = d / 2;
    seq_t pow10_h = pow10(half);

    if (d % 2 == 0) {
        seq_t first = nseq / pow10_h;
        seq_t last  = nseq % pow10_h;
        return last * pow10_h + first;
    } else {
        seq_t pow10_hplus1 = pow10_h * (seq_t)10;
        seq_t first   = nseq / pow10_hplus1;
        seq_t middle  = (nseq / pow10_h) % (seq_t)10;
        seq_t last    = nseq % pow10_h;
        return last * pow10_hplus1 + middle * pow10_h + first;
    }
}
