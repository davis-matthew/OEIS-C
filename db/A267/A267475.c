/*
 * A267475  Number of length-7 0..n arrays with no following elements larger
 *          than the first repeated value.
 * https://oeis.org/A267475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (n=1..8): 71, 1112, 8770, 44901, 171601, 532840, 1418740, 3357537 */

seq_t a(idx_t n) {
    /* First eight terms (index 1..8). */
    static const seq_t init[9] = {
        0,              /* dummy 0th element to make indices 1..8 align */
        71, 1112, 8770,
        44901, 171601,
        532840, 1418740, 3357537
    };
    if (n <= 8) {
        return init[n];
    }

    /* maintain the last eight values of the recurrence */
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t a6 = init[6];
    seq_t a7 = init[7];
    seq_t a8 = init[8];

    for (idx_t i = 9; i <= n; ++i) {
        /* All coefficients are positive in the positive and negative sums,
           so the final subtraction is well‑defined and does not underflow. */
        seq_t pos = 8 * a8 + 56 * a6 + 56 * a4 + 8 * a2;
        seq_t neg = 28 * a7 + 70 * a5 + 28 * a3 + a1;
        seq_t next = pos - neg;          /* pos >= neg for n >= 9 */

        /* shift the window of last eight values */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = next;
    }

    return a8;
}
