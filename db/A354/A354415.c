/*
 * A354415  First differences of A354414.
 * https://oeis.org/A354415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let F_k denote the k-th Fibonacci number (F_1=1, F_2=1). The
 *            term a(n) is the first Fibonacci number whose index k is such
 *            that F_{k-1} ≥ n+1; then a(n) = F_k. Equivalently, a(n) =
 *            F_{t+1} where t is the smallest integer with F_t ≥ n+1
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: the sequence is defined for n ≥ 1.  */
    if (n == 0)
        return (seq_t)0;  /* undefined for offset, but kept for safety */

    /* Target Fibonacci value to exceed.  */
    seq_t target = (seq_t)n + (seq_t)1;

    /* Generate Fibonacci numbers until we reach the smallest one
       that is ≥ target.  */
    seq_t f_prev = (seq_t)0;   /* F_0 */
    seq_t f_cur  = (seq_t)1;   /* F_1 */

    /* Loop until f_cur >= target; then f_cur == F_t. */
    while (f_cur < target) {
        seq_t f_next = f_prev + f_cur;  /* F_{i+1} = F_{i-1} + F_i */
        f_prev = f_cur;                /* shift for next iteration */
        f_cur  = f_next;
    }

    /* Now f_prev = F_{t-1} and f_cur = F_t.  The required value is
       F_{t+1} = F_{t-1} + F_t.  */
    return f_prev + f_cur;
}
