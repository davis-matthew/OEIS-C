/*
 * A250362  Number of length 4 arrays x(i), i=1..4 with x(i) in i..i+n and no
 *          value appearing more than 3 times.
 * https://oeis.org/A250362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250362: Number of length‑4 arrays x(i), i=1..4, with
   x(i) in [i, i+n] and no value appearing more than three times.
   Formula (for n > 1):
     a(n) = n^4 + 4*n^3 + 6*n^2 + 3*n + 3
   Special case:
     a(1) = 16
*/

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;

    seq_t x  = (seq_t)n;          /* n */
    seq_t x2 = x * x;             /* n^2 */
    seq_t x3 = x2 * x;            /* n^3 */
    seq_t x4 = x3 * x;            /* n^4 */

    seq_t term1 = x4;                 /* n^4        */
    seq_t term2 = 4 * x3;             /* 4*n^3      */
    seq_t term3 = 6 * x2;             /* 6*n^2      */
    seq_t term4 = 3 * x;              /* 3*n        */

    return term1 + term2 + term3 + term4 + (seq_t)3;
}
