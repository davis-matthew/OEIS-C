/*
 * A269621  Number of length-5 0..n arrays with no repeated value differing
 *          from the previous repeated value by other than plus two, zero or
 *          minus 1.
 * https://oeis.org/A269621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 7*n^3 + 12*n^2 + 3*n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    /* Compute each power of n and accumulate the result */
    seq_t result = x * x * x * x * x;          /* n^5 */
    result += (seq_t)5 * x * x * x * x;        /* + 5*n^4 */
    result += (seq_t)7 * x * x * x;            /* + 7*n^3 */
    result += (seq_t)12 * x * x;               /* +12*n^2 */
    result += (seq_t)3 * x;                    /* + 3*n */

    return result;
}
