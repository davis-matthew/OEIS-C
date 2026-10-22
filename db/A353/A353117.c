/*
 * A353117  Base-9 representation of A000422(n).
 * https://oeis.org/A353117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007095(A000422(n)) – here A000422(n) is the decimal
 *            integer obtained by concatenating the numbers n, n‑1, … , 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   digits, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
       Construct the reverse concatenation A000422(n):
       start with 0 and, for i = n … 1, shift the current value left
       by digits(i) decimal places (multiply by 10^digits(i)) and add i.
       -------------------------------------------------------------------- */
    seq_t concat = 0;
    for (idx_t i = n; i >= 1; --i) {
        int d       = digits(i);              /* decimal digits of i   */
        seq_t mult  = pow10(d);               /* 10^d as a seq_t       */
        concat      = seq_mul(concat, mult);  /* concat * 10^d          */
        concat      = concat + (seq_t)i;      /* add i                  */
        if (i == 1) break;   /* avoid underflow of unsigned idx_t */
    }

    /* --------------------------------------------------------------------
       Convert the big integer 'concat' from base 10 to base 9.
       The remainders (0‑8) are collected LSB first in a dynamic array.
       -------------------------------------------------------------------- */
    seq_t tmp = concat;
    size_t  cap = 32;
    size_t  len = 0;
    uint8_t *dig = (uint8_t *)malloc(cap);
    if (!dig) return 0;        /* allocation failure – return 0 */

    if (tmp == 0) {
        dig[len++] = 0;
    } else {
        while (tmp != 0) {
            seq_t r = tmp % 9;  /* remainder 0‑8 */
            if (len >= cap) {
                cap <<= 1;
                uint8_t *tmp2 = (uint8_t *)realloc(dig, cap);
                if (!tmp2) { free(dig); return 0; }
                dig = tmp2;
            }
            dig[len++] = (uint8_t)r;
            tmp = tmp / 9;
        }
    }

    /* --------------------------------------------------------------------
       Interpret the base‑9 digits as decimal digits: read the array from
       most‑significant to least‑significant, multiplying the result by
       10 each step and adding the next digit.
       -------------------------------------------------------------------- */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = seq_mul(result, 10);
        result = result + dig[len - 1 - i];
    }

    free(dig);
    return result;
}
