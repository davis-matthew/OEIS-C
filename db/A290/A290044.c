/*
 * A290044  a(n) = |Sum_{k=0..n} (-1)^k*(2k)!!|.
 * https://oeis.org/A290044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |Sum_{k=0..n} (-1)^k*(2k)!!|
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* |t_k| : absolute value of the alternating term (-1)^k*(2k)!!   */
    seq_t t = (seq_t)1;   /* k = 0  ->  (2*0)!! = 1 */
    /* a(n) : absolute value of the partial sum */
    seq_t A = (seq_t)1;   /* a(0) = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        t = (seq_t)(2 * k) * t;   /* |t_k| = 2k * |t_{k-1}| */
        A = t - A;                /* a(k) = |t_k| - a(k-1)   */
    }
    return A;
}
