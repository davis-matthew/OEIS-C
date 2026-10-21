/*
 * A269470  Number of length-6 0..n arrays with no repeated value equal to the
 *          previous repeated value.
 * https://oeis.org/A269470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 6*n^5 + 11*n^4 + 4*n^3 - n^2 + n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t r  = (seq_t)1;          /* coefficient of n^6 */
    r = r * nn + (seq_t)6;         /* +6*n^5 */
    r = r * nn + (seq_t)11;        /* +11*n^4 */
    r = r * nn + (seq_t)4;         /* +4*n^3 */
    r = r * nn;                    /* multiply to get n^4 term */
    r--;                            /* subtract 1 (coefficient of -n^2) */
    r = r * nn + (seq_t)1;          /* +1*n (coefficient of n) */
    r = r * nn;                    /* final multiply by n gives n^6 + … + n */
    return r;
}
