/*
 * A320991  Fibonacci rabbit sequence number n coded in base nine.
 * https://oeis.org/A320991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1. For n ≥ 2: a(n) = a(n-1) * 9^{F_{n-1}} +
 *            a(n-2), where F_k is the k‑th Fibonacci number (F_0=0, F_1=1).
 *            Define P(k) = 9^{F_k}. Then P(0)=1, P(1)=9 and P(k) = P(k-1) *
 *            P(k-2). Using P we have a(n) = a(n-1) * P(n-1) + a(n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    if (n == 1u)
        return (seq_t)1;

    seq_t a_prev2 = (seq_t)0;          /* a(0) */
    seq_t a_prev1 = (seq_t)1;          /* a(1) */
    seq_t p_prev2 = (seq_t)1;          /* P(0) = 9^0 = 1 */
    seq_t p_prev1 = (seq_t)9;          /* P(1) = 9^1 = 9 */

    for (idx_t k = 2; k <= n; ++k) {
        /* a(k) = a_prev1 * p_prev1 + a_prev2 */
        seq_t a_cur = a_prev1 * p_prev1 + a_prev2;
        /* P(k) = P(k-1) * P(k-2) */
        seq_t p_cur = p_prev1 * p_prev2;

        /* rotate last two for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }
    return a_prev1;
}
