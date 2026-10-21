/*
 * A260527  Concatenate the positions of digits 0, 1,..., 9 in the decimal
 *          representation of n, using 1 for the leftmost digit etc., skip
 *          digits which don't occur.
 * https://oeis.org/A260527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the positions (1‑indexed from the left) of
 *            each digit 0..9 that occurs in the decimal representation of n;
 *            digits that do not occur are skipped
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_len_seq_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_len_seq_value.h"

/* helper: 10^exp as seq_t (exp small, exp ≤ 20 is enough for idx_t up to 64‑bit) */
static seq_t pow10_seq(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n)
{
    /* Maximum 20 decimal digits for a 64‑bit integer + 1 safety margin */
    unsigned char rev[32];
    size_t rsize = 0;

    /* Collect digits of n in reverse order */
    if (n == 0)
    {
        rev[0] = 0;
        rsize = 1;
    }
    else
    {
        idx_t m = n;
        while (m > 0)
        {
            rev[rsize++] = (unsigned char)(m % 10);
            m /= 10;
        }
    }

    seq_t cur[10] = {0};   /* concatenated positions for digits 0..9 */

    /* Process digits from left to right, positions are 1‑based */
    for (size_t i = 0; i < rsize; ++i)
    {
        unsigned d = rev[rsize - 1 - i];
        idx_t pos = (idx_t)(i + 1);             /* position of this digit */
        /* length of pos in decimal digits */
        idx_t temp = pos;
        size_t pos_len = 0;
        do
        {
            ++pos_len;
            temp /= 10;
        } while (temp > 0);
        cur[d] = cur[d] * pow10_seq(pos_len) + (seq_t)pos;
    }

    /* Concatenate the results for digits 0..9, skipping zeros */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d)
    {
        if (cur[d] != 0)
        {
            size_t cur_len = digit_len_seq_value(cur[d]);
            result = result * pow10_seq(cur_len) + cur[d];
        }
    }

    return result;
}
