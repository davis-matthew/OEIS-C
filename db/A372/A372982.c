/*
 * A372982  a(n) = floor((a(n-1)+a(n-2)+n)/(a(n-3)+1)), where a(1)=1, a(2)=2,
 *          a(3)=3.
 * https://oeis.org/A372982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((a(n-1)+a(n-2)+n)/(a(n-3)+1)) Initial values:
 *            a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 461, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first three terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* Keep the three most recent terms */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)3;   /* a(n-1) */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        curr = a2 + a1 + (seq_t)i;          /* numerator: a(n-1)+a(n-2)+n */
        curr = curr / (a0 + (seq_t)1);      /* denominator: a(n-3)+1 */
        a0 = a1;                            /* shift window */
        a1 = a2;
        a2 = curr;
    }
    return a2;
}
