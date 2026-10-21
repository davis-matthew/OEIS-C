/*
 * A260520  Concatenate the positions of digits 0, 1, ..., 9 in the decimal
 *          representation of n, using 1 for the leftmost digit etc., and 0
 *          when the digit does not occur.
 * https://oeis.org/A260520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate, for digits 0‑9, the decimal positions
 *            (1‑based, from the left) at which each digit occurs in the
 *            decimal representation of n; if a digit does not appear, use a
 *            single 0. The concatenated segments are joined in order 0,1,…,9
 * Range:     u8 none, u16 none, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static inline int digits_count(idx_t x)
{
    int c = 0;
    do { x /= 10; ++c; } while (x > 0);
    return c;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* Store the decimal digits of n from least‑significant to most‑significant.
       MAX_DIGITS must be a compile‑time constant to avoid VLA initialization
       errors.  A 64‑bit integer has at most 20 decimal digits. */
#define MAX_DIGITS 32
    int digits[MAX_DIGITS];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp > 0 && len < MAX_DIGITS) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Build the value for each digit 0‑9.  seg_val[d] holds the current
       concatenated positions for digit d, and seg_len[d] records the
       number of decimal digits in that segment. */
    seq_t seg_val[10];
    int    seg_len[10];
    for (int i = 0; i < 10; ++i) {
        seg_val[i] = 0;
        seg_len[i] = 0;
    }

    for (int i = len - 1; i >= 0; --i) {   /* iterate from most‑significant to least */
        int d = digits[i];                 /* current decimal digit */
        idx_t pos = (idx_t)(len - i);      /* 1‑based position from the left */
        int pos_len = digits_count(pos);
        seq_t factor = pow10((idx_t)pos_len);
        seg_val[d] = seg_val[d] * factor + (seq_t)pos;
        seg_len[d] += pos_len;
    }

    /* Concatenate the ten segments in order 0…9.  If a digit never
       appeared, its segment is a single zero digit. */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d) {
        if (seg_len[d] == 0) {            /* digit d did not occur */
            result = result * 10 + 0;
        } else {
            seq_t f = pow10((idx_t)seg_len[d]);
            result = result * f + seg_val[d];
        }
    }

    return result;
}
