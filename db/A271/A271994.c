/*
 * A271994  The chalcogen sequence (a(n) = A018227(n)-2).
 * https://oeis.org/A271994
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 12*n^2 + 25*n + (-1)^n*3*(n+2) - 30)/12
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The formula is valid for n >= 2; for other n we return 0,
       although the behaviour is unspecified by the OEIS entry. */
    if (n < 2)
        return 0;

    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;

    seq_t term1 = (seq_t)2 * n3;            /* 2*n^3   */
    seq_t term2 = (seq_t)12 * n2;           /* 12*n^2  */
    seq_t term3 = (seq_t)25 * ns;           /* 25*n    */
    seq_t term4 = (seq_t)3 * (ns + (seq_t)2); /* 3*(n+2) */

    seq_t base = term1 + term2 + term3;     /* sum of the first three terms */

    /* add or subtract term4 depending on the parity of n */
    if ((n & 1) == 0) {
        base += term4;
    } else {
        base -= term4;
    }

    base -= (seq_t)30;                      /* subtract the constant 30 */

    return base / (seq_t)12;
}
