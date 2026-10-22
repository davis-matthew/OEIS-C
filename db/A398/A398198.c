/*
 * A398198  Lower (3/5,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7), with
 *            a(0)=0, a(1)=1, a(2)=5, a(3)=12, a(4)=22, a(5)=34, a(6)=48
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial values for indices 0..6 */
    static const seq_t init[7] = {0, 1, 5, 12, 22, 34, 48};

    if (n < 7) {
        return init[n];
    }

    /* Buffer holding the last 8 computed terms */
    seq_t buf[8];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];
    buf[4] = init[4];
    buf[5] = init[5];
    buf[6] = init[6];
    /* buf[7] is uninitialized until first iteration, but will not
       be read before being written. */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = 2 * buf[(i - 1) & 7]
                    - buf[(i - 2) & 7]
                    + buf[(i - 5) & 7]
                    - 2 * buf[(i - 6) & 7]
                    + buf[(i - 7) & 7];
        buf[i & 7] = val;
    }

    return buf[n & 7];
}
