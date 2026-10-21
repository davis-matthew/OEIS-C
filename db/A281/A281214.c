/*
 * A281214  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 342", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A281214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*a(n-2) + a(n-4) - 100*a(n-6) Initial values: a(0)=1,
 *            a(1)=11, a(2)=100, a(3)=1110, a(4)=10001, a(5)=111011
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return predefined first six terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 11;
    if (n == 2) return 100;
    if (n == 3) return 1110;
    if (n == 4) return 10001;
    if (n == 5) return 111011;

    /* Ring buffer storing the last 7 computed values. */
    seq_t buf[7];
    buf[0] = 1;
    buf[1] = 11;
    buf[2] = 100;
    buf[3] = 1110;
    buf[4] = 10001;
    buf[5] = 111011;
    /* buf[6] will be computed in the loop */

    idx_t i;
    for (i = 6; i <= n; ++i) {
        /* Indices: (i-2), (i-4), (i-6) modulo 7 are equivalent to
           (i+5), (i+3), (i+1) respectively for i >= 6. */
        seq_t v = (seq_t)100 * buf[(i + 5) % 7]      /* 100*a(i-2)  */
                +                         buf[(i + 3) % 7] /*       + a(i-4) */
                - (seq_t)100 * buf[(i + 1) % 7]; /*       - 100*a(i-6) */
        buf[i % 7] = v;
    }
    return buf[n % 7];
}
