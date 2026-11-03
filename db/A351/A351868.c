/*
 * A351868  In the decimal expansion of a number, replace each run of
 *          consecutive equal digits, say of k consecutive d's, by the decimal
 *          expansion of k*d.
 * https://oeis.org/A351868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal operator that replaces each run of k equal digits
 *            d by the decimal representation of k*d (concatenated). a(n) is
 *            the value returned by applying this operator to n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9877 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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
    /* Handle n = 0 separately because the digit extraction loop
       would produce an empty array.  The operator maps 0 to 0. */
    if (n == 0)
        return 0;

    /* Extract decimal digits of n into an array (least significant
       first).  20 digits are enough for an unsigned 64‑bit integer. */
    uint8_t digits[21];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t result = 0;
    int i = (int)len - 1;          /* start with most significant digit */

    while (i >= 0) {
        uint8_t d = digits[i];
        int runlen = 1;
        i--;

        /* Count how many equal digits follow (still in reverse order). */
        while (i >= 0 && digits[i] == d) {
            runlen++;
            i--;
        }

        /* Compute the product of run length and digit. */
        seq_t prod = (seq_t)runlen * (seq_t)d;

        /* Append this product to the result. */
        int plen = num_digits(prod);           /* number of decimal digits in prod */
        result = result * pow10((idx_t)plen) + prod;
    }

    return result;
}
