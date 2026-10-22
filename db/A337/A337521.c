/*
 * A337521  a(n) = L(n)*a(n-1) + a(n-2) with a(0) = a(1) = 1 and L(n) the
 *          Lucas numbers A000032.
 * https://oeis.org/A337521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n) * a(n-1) + a(n-2) with initial values a(0) = a(1) =
 *            1; L(0) = 2, L(1) = 1, and L(k) = L(k-1) + L(k-2) for k >= 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a_prev2 = (seq_t)1;   /* a(n-2)   */
    seq_t a_prev1 = (seq_t)1;   /* a(n-1)   */
    seq_t a_curr  = (seq_t)0;   /* a(n)     */

    seq_t l_prev2 = (seq_t)2;   /* L(n-2)   */
    seq_t l_prev1 = (seq_t)1;   /* L(n-1)   */
    seq_t l_curr  = (seq_t)0;   /* L(n)     */

    for (idx_t i = 2; i <= n; ++i) {
        l_curr = l_prev1 + l_prev2;          /* compute L(i)   */
        a_curr = l_curr * a_prev1 + a_prev2; /* compute a(i)   */
        /* shift indices for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
        l_prev2 = l_prev1;
        l_prev1 = l_curr;
    }
    return a_curr;
}
