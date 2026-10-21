/*
 * A256455  Numbers that appear at least once in a Pythagorean triple (a, b,
 *          b+1).
 * https://oeis.org/A256455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = merge of all odd integers ≥3 with the even integers
 *            ((odd^2)-1)/2
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t odd   = (seq_t)3;                 /* next odd term        */
    seq_t even  = (seq_t)4;                 /* next even term       */
    seq_t a_val = (seq_t)3;                 /* odd a used for even */

    while (1) {
        seq_t next;
        if (odd < even) {                   /* take next odd number */
            next = odd;
            odd += (seq_t)2;
        } else {                            /* take next even number */
            next = even;
            a_val += (seq_t)2;              /* next odd a value     */
            even = (a_val * a_val - (seq_t)1) / (seq_t)2;
        }
        count++;
        if (count == n) return next;
    }
}
