/*
 * A293193  a(n) = n! * [x^n] exp(n*arctanh(x)).
 * https://oeis.org/A293193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*coeff of x^n in exp(n*arctanh(x))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case a(0) = 1. */
    if (n == 0) {
        return 1;
    }
    /*
     * Let c_m be the coefficient of x^m in exp(n*arctanh(x)).
     * The differential equation
     *   (m+1)c_{m+1} - (m-1)c_{m-1} = n c_m
     * yields the integer recurrence for
     *   b_m = m! * c_m
     *
     * b_{m+1} = n*b_m + m*(m-1)*b_{m-1}
     * with initial values b_0 = 1 and b_1 = n.
     * The desired term is a(n) = b_n.
     */
    seq_t b_prev = 1;               /* b_0 */
    seq_t b_curr = (seq_t)n;        /* b_1 = n */

    for (idx_t m = 1; m < n; ++m) {
        seq_t term1 = (seq_t)n * b_curr;                 /* n*b_m       */
        seq_t term2 = (seq_t)m * (seq_t)(m - 1) * b_prev;/* m*(m-1)*b_{m-1} */
        seq_t b_next = term1 + term2;                    /* b_{m+1}     */
        b_prev = b_curr;
        b_curr = b_next;
    }
    return b_curr;  /* This is a(n) = b_n */
}
