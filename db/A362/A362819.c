/*
 * A362819  Number of ordered pairs of involutions on [n] that commute.
 * https://oeis.org/A362819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*(n-1)*a(n-2) + (n-1)*(n-2)*(n-3)*a(n-4)
 *            Initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=10
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;

    /* Store last four computed values:
       a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)   */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)4; /* a(2) */
    seq_t a3 = (seq_t)10;/* a(3) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* convert indices to seq_t once per iteration */
        seq_t i1 = (seq_t)(i - 1);
        seq_t i2 = (seq_t)(i - 2);
        seq_t i3 = (seq_t)(i - 3);

        /* recurrence: a(i) = a(i-1) + 3*(i-1)*a(i-2) + (i-1)*(i-2)*(i-3)*a(i-4) */
        ai = a3
            + (seq_t)3 * i1 * a2
            + i1 * i2 * i3 * a0;

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3; /* a(n) after the loop */
}
