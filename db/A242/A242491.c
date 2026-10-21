/*
 * A242491  Numbers avoiding subtractive notation when written in Roman
 *          numerals.
 * https://oeis.org/A242491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number formed by writing n in base 8 and replacing digits
 *            * 0→0, 1→1, 2→2, 3→3, 4→5, 5→6, 6→7, 7→8. This is the sequence
 *            of * positive integers whose decimal representation contains no
 *            4 or 9
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t digit_map[8] = {(seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3,
                                   (seq_t)5, (seq_t)6, (seq_t)7, (seq_t)8};

seq_t a(idx_t n)
{
    idx_t tmp = n;
    unsigned char digits[64];
    idx_t len = 0;

    /* Convert n to base‑8 digits (least significant first) */
    while (tmp) {
        digits[len++] = (unsigned char)(tmp & 7); /* tmp % 8 */
        tmp >>= 3;                               /* tmp / 8 */
    }

    /* Build the decimal number from the mapped digits (most significant first) */
    seq_t res = 0;
    while (len--) {
        res = res * 10 + digit_map[digits[len]];
    }

    return res;
}
