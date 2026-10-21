/*
 * A269497  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by one.
 * https://oeis.org/A269497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 15*n^4 + 8*n^3 + 5*n^2 + 6*n - 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t r = (seq_t)1;
    r = r * N + (seq_t)6;   /* n + 6 */
    r = r * N + (seq_t)15;  /* n^2 + 6n + 15 */
    r = r * N + (seq_t)8;   /* n^3 + 6n^2 + 15n + 8 */
    r = r * N + (seq_t)5;   /* n^4 + 6n^3 + 15n^2 + 8n + 5 */
    r = r * N + (seq_t)6;   /* n^5 + 6n^4 + 15n^3 + 8n^2 + 5n + 6 */
    r = r * N - (seq_t)1;   /* n^6 + 6n^5 + 15n^4 + 8n^3 + 5n^2 + 6n - 1 */
    return r;
}
