/*
 * A309873  Period-doubling turn sequence, +1 when the 2-adic valuation of n
 *          is even or -1 when odd.
 * https://oeis.org/A309873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign, easy
 * Formula:   a(n) = (-1)^A007814(n) = +1 when the 2‑adic valuation of n is
 *            even, -1 when it is odd. Here A007814(n) is the number of low
 *            (trailing) 0 bits of n
 * Range:     i8 n <= 4096, i16 n <= 4096, i32 n <= 4096, i64 n <= 4096,
 *            i128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count trailing zeros in n (the exponent of 2 dividing n). */
    idx_t exp2 = 0;
    idx_t x = n;                   /* keep n unchanged */
    while ((x & 1) == 0) {          /* while n is even */
        ++exp2;
        x >>= 1;                    /* divide by 2 */
    }
    /* Return +1 for even exponent, -1 for odd. */
    return (exp2 & 1) ? (seq_t)-1 : (seq_t)1;
}
