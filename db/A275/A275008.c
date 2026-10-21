/*
 * A275008  Decimal expansion of R/F, where R is the molar gas constant and F
 *          is Faraday's constant according to the 2019 SI system.
 * https://oeis.org/A275008
 *
 * Offset:    -4    (a(-4) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit (n+9) of the decimal expansion of
 *            1380649/16021766340 (the exact value k/e = 1.380649e-23 /
 *            1.602176634e-19). The number is less than one, so the first
 *            non‑zero digit appears at position n = -4
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Exact numerator and denominator of k/e. */
    const uint64_t num = 1380649ULL;
    const uint64_t den = 16021766340ULL;   /* < 2^37, so 64‑bit is safe */

    /* Position of the desired decimal digit after removing the leading
       four zeros (offset -4).  For n = -4 we need the 5th digit. */
    idx_t pos = n + 9;

    uint64_t rem = num;          /* current remainder of the division */
    uint64_t digit = 0;

    for (idx_t i = 0; i < pos; ++i) {
        rem   = rem * 10;                /* shift decimal point */
        digit = rem / den;                /* next digit */
        rem   = rem % den;                /* remainder for next step */
    }
    return (seq_t)digit;
}
