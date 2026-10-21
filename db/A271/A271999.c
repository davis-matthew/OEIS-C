/*
 * A271999  Halogen sequence: a(n) = A018227(n)-1.
 * https://oeis.org/A271999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) * a(1) = 1 * a(2) = 11 * a(n) = (1/12)*(2*n^3 + 12*n^2 +
 *            28*n - 12) for n>2 and even * a(n) = (1/12)*(2*n^3 + 12*n^2 +
 *            22*n - 24) for n > 2 and odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small base cases explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;

    /* Convert the index to seq_t for subsequent calculations */
    seq_t nt  = (seq_t)n;
    seq_t nt2 = nt * nt;          /* n²  */
    seq_t nt3 = nt2 * nt;         /* n³  */

    if ((n & 1) == 0) {           /* even n > 2 */
        return ( ( (seq_t)2 * nt3 + (seq_t)12 * nt2 + (seq_t)28 * nt - (seq_t)12 ) )
            / (seq_t)12;
    } else {                      /* odd n  > 2 */
        return ( ( (seq_t)2 * nt3 + (seq_t)12 * nt2 + (seq_t)22 * nt - (seq_t)24 ) )
            / (seq_t)12;
    }
}
