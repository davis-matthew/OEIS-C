/*
 * A241133  Number of n X 2 0..2 arrays with no element equal to the same
 *          number of vertical neighbors as horizontal neighbors, with new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A241133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+2*a(n-2)-11*a(n-3)-6*a(n-4)+8*a(n-5), n>6
 *            initial values: a(1)=1, a(2)=2, a(3)=5, a(4)=17, a(5)=56,
 *            a(6)=195
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* obvious base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)17;
    if (n == 5) return (seq_t)56;
    if (n == 6) return (seq_t)195;

    /* use a sliding window of the last 5 values: prev[0]=a(i-5), ..., prev[4]=a(i-1) */
    seq_t prev[5] = { (seq_t)2, (seq_t)5, (seq_t)17, (seq_t)56, (seq_t)195 };
    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 4 * prev[4] + 2 * prev[3] - 11 * prev[2] - 6 * prev[1] + 8 * prev[0];
        /* shift window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }
    return prev[4]; /* a(n) */
}
