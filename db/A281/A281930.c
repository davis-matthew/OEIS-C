/*
 * A281930  Number of n X 2 0..1 arrays with no element equal to more than
 *          four of its king-move neighbors, with the exception of exactly two
 *          elements, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A281930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 14*a(n-3) - 21*a(n-4) - 12*a(n-5) -
 *            4*a(n-6) Empirical recurrence, initial values: a(1)=0, a(2)=0,
 *            a(3)=1, a(4)=6, a(5)=33, a(6)=166
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is defined for n>=1 only, but guard anyway */
        return (seq_t)0;

    /* handle the first six terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)33;
    if (n == 6) return (seq_t)166;

    /* ring buffer holding the last six values:
       prev[0] = a(i-6), prev[5] = a(i-1) at loop iteration i   */
    seq_t prev[6];
    prev[0] = (seq_t)0;   /* a(1)   */
    prev[1] = (seq_t)0;   /* a(2)   */
    prev[2] = (seq_t)1;   /* a(3)   */
    prev[3] = (seq_t)6;   /* a(4)   */
    prev[4] = (seq_t)33;  /* a(5)   */
    prev[5] = (seq_t)166; /* a(6)   */

    seq_t cur;
    for (idx_t i = 7; i <= n; ++i) {
        cur = prev[5] * (seq_t)6
            - prev[4] * (seq_t)3
            - prev[3] * (seq_t)14
            - prev[2] * (seq_t)21
            - prev[1] * (seq_t)12
            - prev[0] * (seq_t)4;

        /* shift the ring buffer for next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }
    return prev[5];
}
