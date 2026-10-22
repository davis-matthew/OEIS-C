/*
 * A399025  a(n) is the number of inequivalent ways of choosing 4 points on an
 *          n X n grid that form a square up to rotations.
 * https://oeis.org/A399025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n+2) = (1/3)*(n^2+2*n+3)*(n+1)^2 a(2*n+1) =
 *            (1/6)*n*(n+1)*(2*n^2+2*n+5) (n ≥ 0, here n is the index in the
 *            sequence, starting from 1)
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 446, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute in terms of k:
       if n is even: k = n/2 - 1   (so a = a(2*k+2))
       if n is odd : k = (n-1)/2  (so a = a(2*k+1))          */
    if (n % 2 == 0) {
        seq_t k = (seq_t)(n / 2 - 1);
        seq_t k1 = k + 1;
        seq_t t1 = k * k + 2 * k + 3;       /* k^2 + 2k + 3 */
        seq_t t2 = k1 * k1;                 /* (k+1)^2    */
        return (t1 * t2) / 3;
    } else {
        seq_t k = (seq_t)((n - 1) / 2);
        seq_t t1 = k * (k + 1);              /* k*(k+1)    */
        seq_t t2 = (seq_t)(2 * k * k + 2 * k + 5);  /* 2k^2+2k+5  */
        return (t1 * t2) / 6;
    }
}
