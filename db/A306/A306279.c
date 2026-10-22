/*
 * A306279  Numbers congruent to 3 or 18 mod 22.
 * https://oeis.org/A306279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*n - 6 + 2*(-1)^n
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent piecewise:  a(n) = 11*n - 8  (n odd),  a(n) = 11*n - 4 (n even) */

seq_t a(idx_t n)
{
    /* OEIS offset n >= 1; behaviour for n < 1 is irrelevant */
    seq_t tn = (seq_t)n;
    if (n & 1)          /* odd n */
        return 11 * tn - 8;
    else                /* even n */
        return 11 * tn - 4;
}
