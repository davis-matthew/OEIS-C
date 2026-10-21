/*
 * A290168  If n is even then a(n) = n^2*(n+2)/8, otherwise a(n) =
 *          (n-1)*n*(n+1)/8.
 * https://oeis.org/A290168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n+2)/8 for even n, (n-1)*n*(n+1)/8 for odd n
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {
        /* even n : a(n) = (n/2)^2 * (n/2+1) */
        seq_t k = nn / (seq_t)2;
        seq_t k_plus = k + (seq_t)1;
        seq_t res = k * k;
        res = res * k_plus;
        return res;
    } else {
        /* odd n : a(n) = (n/2) * (n/2+1) * n / 2  */
        seq_t k = nn / (seq_t)2;
        seq_t k_plus = k + (seq_t)1;
        seq_t res = k * k_plus;
        res = res * nn;      /* nn is 2k+1 */
        res = res / (seq_t)2;
        return res;
    }
}
