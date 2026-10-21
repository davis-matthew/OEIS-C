/*
 * A250155  Number of length 1+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250155:
   a(n) = (7/2)n^6 + 14n^5 + (105/4)n^4 + (63/2)n^3 + (91/4)n^2 + 8n + 1
   This is equivalent to
     a(n) = (14 n^6 + 56 n^5 + 105 n^4 + 126 n^3 + 91 n^2 + 32 n + 4) / 4
   The division by 4 is always exact for integer n ≥ 1.
   Implementation uses only integer arithmetic of type seq_t.
*/

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once. */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n. */
    seq_t n2 = nn * nn;          /* n^2   */
    seq_t n3 = n2 * nn;          /* n^3   */
    seq_t n4 = n3 * nn;          /* n^4   */
    seq_t n5 = n4 * nn;          /* n^5   */
    seq_t n6 = n5 * nn;          /* n^6   */

    /* Build the numerator of the rational expression. */
    seq_t num = (seq_t)14 * n6
              + (seq_t)56 * n5
              + (seq_t)105 * n4
              + (seq_t)126 * n3
              + (seq_t)91  * n2
              + (seq_t)32  * nn
              + 4;

    /* Exact division by 4. */
    return num / 4;
}
