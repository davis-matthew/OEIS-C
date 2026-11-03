/*
 * A256289  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8
 *          -> 0 to the digits of n written in base 9; do not convert back to
 *          base 10.
 * https://oeis.org/A256289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number obtained by incrementing each base‑9 digit
 *            of n by 1 modulo 9
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: transform n written in base 9, digits 0–8, by adding 1 modulo 9
   and interpret the resulting decimal string as a number in seq_t. */
static seq_t _shift_base9_to_dec(idx_t n)
{
    if (n == 0)          /* base‑9 "0" -> "1" -> decimal 1 */
        return (seq_t)1;

    /* maximum number of base‑9 digits for a 64‑bit value is 21,
       allocate a safe buffer. */
    unsigned char digits[64];
    size_t len = 0;
    idx_t tmp = n;

    /* extract base‑9 digits least significant first */
    while (tmp) {
        digits[len++] = (unsigned char)(tmp % 9);
        tmp /= 9;
    }

    seq_t res = 0;
    size_t i = len;
    while (i--) {
        unsigned char d = digits[i];
        d = (d == 8) ? 0 : d + 1;   /* (d + 1) % 9 */
        res = res * 10 + d;
    }
    return res;
}

/* OEIS function */
seq_t a(idx_t n)
{
    return _shift_base9_to_dec(n);
}
