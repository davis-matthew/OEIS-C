/*
 * A260521  Concatenate the positions of digits 9, 8,..., 0 in the decimal
 *          representation of n, using 1 for the rightmost digit etc., and 0
 *          when the digit does not occur.
 * https://oeis.org/A260521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate for d = 9 … 0 the positions (1‑based from the
 *            right) of digit d in n's decimal representation; if d does not
 *            occur, insert a single 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Extract digits of n (right‑most first) */
    idx_t tmp = n;
    int   num_digits = digit_len((seq_t)n);          /* n may be 0 -> 1 digit */
    unsigned char digits[64];                       /* enough for all cases */

    for (int pos = 1; pos <= num_digits; ++pos) {
        digits[pos] = (unsigned char)(tmp % 10U);
        tmp /= 10U;
    }

    seq_t result = 0;

    /* Process digits from 9 down to 0 */
    for (int d = 9; d >= 0; --d) {
        seq_t block   = 0;
        int   block_len = 0;

        /* Append positions of digit d in ascending order (pos 1,2,…) */
        for (int pos = 1; pos <= num_digits; ++pos) {
            if (digits[pos] == (unsigned char)d) {
                seq_t pos_val   = (seq_t)pos;
                int   pos_len   = digit_len(pos_val);
                block = block * pow10((idx_t)pos_len) + pos_val;
                block_len += pos_len;
            }
        }

        /* If digit d never occurs, insert a single 0 */
        if (block_len == 0) {
            block_len = 1;
            block = 0;
        }

        /* Append the block to the overall result */
        result = result * pow10((idx_t)block_len) + block;
    }

    return result;
}
