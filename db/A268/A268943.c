/*
 * A268943  Number of length-n 0..7 arrays with no repeated value unequal to
 *          the previous repeated value plus one mod 7+1.
 * https://oeis.org/A268943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 34*a(n-2) - 56*a(n-3) (n ≥ 4) with initial
 *            values a(1)=8, a(2)=64, a(3)=504
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)64;
    if (n == 3)
        return (seq_t)504;

    /* iterative computation, no dynamic allocation required */
    seq_t a1 = (seq_t)8;   /* a(n-3)   */
    seq_t a2 = (seq_t)64;  /* a(n-2)   */
    seq_t a3 = (seq_t)504; /* a(n-1)   */
    seq_t a4;              /* a(n)     */

    for (idx_t i = 4; i <= n; ++i) {
        a4 = (seq_t)13 * a3
            - (seq_t)34 * a2
            - (seq_t)56 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
