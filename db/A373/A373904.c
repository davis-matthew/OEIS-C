/*
 * A373904  a(n) = Sum_{k=0..floor(n/2)} binomial(n+4*k,n-2*k).
 * https://oeis.org/A373904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) Initial terms: a(0)=1, a(1)=1, a(2)=2, a(3)=8, a(4)=30,
 *            a(5)=98
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)30;
    if (n == 5) return (seq_t)98;

    /* sliding window of the last 6 values */
    seq_t prev[6];
    prev[0] = (seq_t)1;   /* a(0) */
    prev[1] = (seq_t)1;   /* a(1) */
    prev[2] = (seq_t)2;   /* a(2) */
    prev[3] = (seq_t)8;   /* a(3) */
    prev[4] = (seq_t)30;  /* a(4) */
    prev[5] = (seq_t)98;  /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t a_n = (seq_t)6 * prev[5]   /* 6*a(n-1) */
                 - (seq_t)14 * prev[4]  /* 14*a(n-2) */
                 + (seq_t)20 * prev[3]  /* 20*a(n-3) */
                 - (seq_t)15 * prev[2]  /* 15*a(n-4) */
                 + (seq_t)6  * prev[1]  /* 6*a(n-5) */
                 - prev[0];            /* a(n-6) */

        /* shift the window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = a_n;
    }

    return prev[5];
}
