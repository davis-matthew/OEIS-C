/*
 * A281445  Nonnegative k for which (2*k^2 + 1)/11 is an integer.
 * https://oeis.org/A281445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * floor((n-1)/2) + (n%2?4:7)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281445: Nonnegative k for which (2*k^2 + 1)/11 is integer. */

seq_t a(idx_t n)
{
    /* n starts from 1 (offset 1).  For n < 1 the result is undefined. */
    idx_t q   = (n - 1) / 2;           /* floor((n-1)/2)   */
    seq_t val = (seq_t)q * (seq_t)11;  /* 11 * q           */
    if (n & 1)                          /* odd n  -> add 4   */
        val += (seq_t)4;
    else                                 /* even n -> add 7   */
        val += (seq_t)7;
    return val;
}
