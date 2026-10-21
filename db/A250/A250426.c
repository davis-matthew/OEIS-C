/*
 * A250426  Number of (n+1)X(2+1) 0..1 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((e+1)^2*(e+2)/2) * ((o+1)^2*(o+2)/2) where e =
 *            floor((n+2)/2), o = floor((n+1)/2)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 97, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute parity block sizes */
    idx_t e_idx = (n + 2) / 2;          /* number of even positions  (e)   */
    idx_t o_idx = (n + 1) / 2;          /* number of odd  positions  (o)   */

    /* cast to the sequence type for arithmetic */
    seq_t e_val = (seq_t)e_idx;
    seq_t o_val = (seq_t)o_idx;

    /* compute f(e) = (e+1)^2 * (e+2) / 2 */
    seq_t e1 = e_val + 1;
    seq_t e2 = e_val + 2;
    seq_t a_even = e1 * e1;   /* (e+1)^2 */
    a_even = a_even * e2;     /* (e+1)^2 * (e+2) */
    a_even = a_even / 2;      /* divide by 2, exact division */

    /* compute f(o) = (o+1)^2 * (o+2) / 2 */
    seq_t o1 = o_val + 1;
    seq_t o2 = o_val + 2;
    seq_t a_odd = o1 * o1;    /* (o+1)^2 */
    a_odd = a_odd * o2;       /* (o+1)^2 * (o+2) */
    a_odd = a_odd / 2;        /* divide by 2, exact division */

    /* final product */
    return a_even * a_odd;
}
