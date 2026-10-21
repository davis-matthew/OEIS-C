/*
 * A271675  Numbers m such that 3*m + 4 is a square.
 * https://oeis.org/A271675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(3*k+4) for n odd; a(n) = (k+1)*(3*k-1) for n even,
 *            where k = (n-1)/2 for odd n and k = n/2 for even n. This is
 *            equivalent to a(n) = A001082(n+1)-1 =
 *            (6*n*(n+1)+(2*n+1)*(-1)^n-1)/8-1
 * Range:     u8 n <= 17, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    if ((n & 1) == 0) {           /* even n = 2k */
        seq_t k = (seq_t)n / 2;
        return (k + 1) * (3 * k - 1);
    } else {                    /* odd n = 2k+1 */
        seq_t k = ((seq_t)n - 1) / 2;
        return k * (3 * k + 4);
    }
}
