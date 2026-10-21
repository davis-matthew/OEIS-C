/*
 * A268229  Rotate the Sierpinski triangle A047999 counterclockwise by 45
 *          degrees to get a square array; a(n) = period of row n.
 * https://oeis.org/A268229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal number whose digits are B(k) for k = 0..p-1,
 *            where p = 2^{⌊log₂ n⌋+1} (or p = 1 for n = 0) and B(k) = 1 if (k
 *            & n) == 0, otherwise B(k) = 0. Thus the period bits are exactly
 *            the positions k having no overlapping 1‑bits with n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the number of bits needed to represent n (0 -> 0). */
static inline idx_t bitlen(idx_t n)
{
    idx_t len = 0;
    while (n != 0) {
        n >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* Handle the special case n = 0: period = "1". */
    if (n == 0)
        return (seq_t)1;

    /* Length of the minimal period. */
    idx_t t = bitlen(n);               /* number of bits in n (>0) */
    idx_t p = ((idx_t)1) << t;         /* 2^t */

    seq_t result = 0;
    for (idx_t k = 0; k < p; ++k) {
        seq_t digit = ((k & n) == 0) ? (seq_t)1 : (seq_t)0;
        result = result * (seq_t)10 + digit;  /* decimal digits */
    }
    return result;
}
