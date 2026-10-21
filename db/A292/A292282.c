/*
 * A292282  a(n) = 2*(n-1)^3*n^2*(n+1).
 * https://oeis.org/A292282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)^3*n^2*(n+1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 36, u64 n <= 1448,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, and for any n < 1 is undefined */
        return 0;

    seq_t nn  = (seq_t)n;
    seq_t nm1 = nn - 1;
    seq_t np1 = nn + 1;

    seq_t res = 2;
    res *= nm1;  /* (n-1) */
    res *= nm1;  /* (n-1)^2 */
    res *= nm1;  /* (n-1)^3 */
    res *= nn;   /* * n   */
    res *= nn;   /* * n^2 */
    res *= np1;  /* * (n+1) */
    return res;
}
