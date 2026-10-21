/*
 * A260699  a(2n+6) = a(2n) + 12*n + 20, a(2n+1) = (n+1)*(2*n+1), with a(0)=0,
 *          a(2)=2, a(4)=9.
 * https://oeis.org/A260699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n+1) = (n+1)*(2n+1); a(6k) = 2k(9k+1); a(6k+2) =
 *            2(9k^2+7k+1); a(6k+4) = 18k^2+26k+9;
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1U) {                     /* odd indices */
        seq_t k = (seq_t)(n >> 1);    /* k = (n-1)/2 */
        return (k + 1) * ((2 * k) + 1);
    } else {                          /* even indices */
        idx_t mod6 = n % 6;
        if (mod6 == 0) {
            seq_t k = (seq_t)(n / 6);
            return 2 * k * (9 * k + 1);
        } else if (mod6 == 2) {
            seq_t k = (seq_t)((n - 2) / 6);
            return 2 * (9 * k * k + 7 * k + 1);
        } else { /* mod6 == 4 */
            seq_t k = (seq_t)((n - 4) / 6);
            return 18 * k * k + 26 * k + 9;
        }
    }
}
