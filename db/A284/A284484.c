/*
 * A284484  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 961", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-20 - 81*(-10)^n + 101*10^n)/180 for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n‑1)+100*a(n‑2)-100*a(n‑3) for n>5, with */
/*  a(0)=1, a(1)=0, a(2)=111, a(3)=1011, a(4)=1111, a(5)=101111 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)111;
    if (n == 3) return (seq_t)1011;
    if (n == 4) return (seq_t)1111;
    if (n == 5) return (seq_t)101111;

    /* iterate using the recurrence */
    seq_t a_nm3 = (seq_t)1011;      /* a(3)  */
    seq_t a_nm2 = (seq_t)1111;      /* a(4)  */
    seq_t a_nm1 = (seq_t)101111;    /* a(5)  */
    seq_t current = 0;

    for (idx_t k = 6; k <= n; ++k)
    {
        current = a_nm1 + (seq_t)100 * a_nm2 - (seq_t)100 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = current;
    }
    return a_nm1;
}
