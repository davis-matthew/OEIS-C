/*
 * A306278  Numbers congruent to 2 or 11 mod 14.
 * https://oeis.org/A306278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n - 4 + (-1)^n
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 7*n - 5 when n is odd, a(n) = 7*n - 3 when n is even */

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the OEIS sequence.  The behavior for n == 0 is unspecified. */
    if (n & 1)  /* odd */
        return (seq_t)7 * n - 5;
    else        /* even */
        return (seq_t)7 * n - 3;
}
