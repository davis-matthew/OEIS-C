/*
 * A269692  Number of length-6 0..n arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo n+1.
 * https://oeis.org/A269692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 15*n^4 + 8*n^3 - 7*n^2 - 4*n + 1 (for n>1),
 *            a(1) = 40
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)40;

    seq_t res = (seq_t)1;          /* coefficient of n^6 */
    res = res * (seq_t)n + (seq_t)6;   /* +6*n^5 */
    res = res * (seq_t)n + (seq_t)15;  /* +15*n^4 */
    res = res * (seq_t)n + (seq_t)8;   /* +8*n^3 */
    res = res * (seq_t)n - (seq_t)7;   /* -7*n^2 */
    res = res * (seq_t)n - (seq_t)4;   /* -4*n  */
    res = res * (seq_t)n + (seq_t)1;   /* +1    */
    return res;
}
