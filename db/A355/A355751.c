/*
 * A355751  Positive numbers k such that the centered cube number k^3 +
 *          (k+1)^3 is equal to the difference of two positive cubes and to
 *          A352759(n).
 * https://oeis.org/A355751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*(2*n-1)^3 - 1)/2
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 491, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2*n-1 fits comfortably into idx_t for all practical n */
    seq_t t = (seq_t)(2 * n - 1);        /* t = (2*n - 1)              */
    seq_t t2 = t * t;                    /* t2 = t^2                   */
    seq_t t3 = t2 * t;                   /* t3 = t^3                   */
    seq_t val = (seq_t)(9 * t3 - 1) / 2; /* a(n) = (9*t^3 - 1)/2       */
    return val;
}
