/*
 * A267233  Number of length-4 0..n arrays with no following elements greater
 *          than or equal to the first repeated value.
 * https://oeis.org/A267233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n^4 + 17*n^3 + 12*n^2 + n)/6 (OEIS A267233)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 171, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* sequence starts at a(1) */

    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;                     /* n^2 */
    seq_t n3   = n2 * sn;                     /* n^3 */
    seq_t n4   = n3 * sn;                     /* n^4 */

    seq_t num = 6 * n4 + 17 * n3 + 12 * n2 + sn;   /* numerator */
    return num / 6;                          /* integer division, exact */
}
