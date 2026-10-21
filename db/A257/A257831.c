/*
 * A257831  In decimal representation of n: replace each digit with its binary
 *          representation.
 * https://oeis.org/A257831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   for n, replace each decimal digit by its binary representation,
 *            concatenate those binary strings, and interpret the resulting
 *            binary string as a decimal number
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 487, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t POW10[5] = {1, 10, 100, 1000, 10000};
static const int  BINLEN[10]  = {1, 1, 2, 2, 3, 3, 3, 3, 4, 4};
static const int  BINVAL[10]  = {0, 1, 10, 11, 100, 101, 110, 111, 1000, 1001};

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into a small array, most
       significant digit first.  */
    int digits[20];          /* enough for 64‑bit numbers */
    int len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp != 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        res = res * POW10[BINLEN[d]] + BINVAL[d];
    }
    return res;
}
