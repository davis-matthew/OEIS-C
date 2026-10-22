/*
 * A347111  a(n) is n in binary rendered as a base-10 number minus n in
 *          ternary rendered as a base-10 number.
 * https://oeis.org/A347111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal_number_from_binary(n) -
 *            decimal_number_from_ternary(n)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Where decimal_number_from_binary(n) interprets the binary representation
    of n as a base‑10 integer, and decimal_number_from_ternary(n) does the
    analogous interpretation in base 3. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Arrays large enough for the maximum possible digit counts */
    unsigned char binary[64];
    unsigned char ternary[45];

    size_t blen = 0, tlen = 0;
    idx_t tmp = n;

    /* Extract binary digits (least significant first) */
    while (tmp != 0) {
        binary[blen++] = (unsigned char)(tmp & 1);
        tmp >>= 1;
    }

    /* Build decimal value from binary digits (most significant first) */
    seq_t bin_val = 0;
    for (size_t i = blen; i-- > 0; ) {
        bin_val = bin_val * 10 + binary[i];
    }

    /* Extract ternary digits (least significant first) */
    tmp = n;
    while (tmp != 0) {
        ternary[tlen++] = (unsigned char)(tmp % 3);
        tmp /= 3;
    }

    /* Build decimal value from ternary digits (most significant first) */
    seq_t ter_val = 0;
    for (size_t i = tlen; i-- > 0; ) {
        ter_val = ter_val * 10 + ternary[i];
    }

    return bin_val - ter_val;
}
