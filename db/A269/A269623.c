/*
 * A269623  Number of length-7 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two, zero or
 *          minus 1.
 * https://oeis.org/A269623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^7 + 7*n^6 + 11*n^5 + 35*n^4 + 18*n^3 + 36*n^2 − 19*n − 7   (for n > 2)
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)92;
    if (n == 2) return (seq_t)1740;

    seq_t N = (seq_t)n;
    seq_t N2 = N * N;              /* n^2 */
    seq_t N3 = N2 * N;             /* n^3 */
    seq_t N4 = N3 * N;             /* n^4 */
    seq_t N5 = N4 * N;             /* n^5 */
    seq_t N6 = N5 * N;             /* n^6 */
    seq_t N7 = N6 * N;             /* n^7 */

    seq_t res = N7;
    res += (seq_t)7 * N6;
    res += (seq_t)11 * N5;
    res += (seq_t)35 * N4;
    res += (seq_t)18 * N3;
    res += (seq_t)36 * N2;
    res -= (seq_t)19 * N;
    res -= (seq_t)7;

    return res;
}
