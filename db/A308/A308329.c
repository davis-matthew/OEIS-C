/*
 * A308329  Even moments of the trace of elements of the binary icosahedral
 *          group.
 * https://oeis.org/A308329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 20*a(n-2) + 17*a(n-3) - 4*a(n-4) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed first five terms */
    static const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)14 };

    if (n < 5) return init[n];

    /* Sliding window of the last five values:
       buf[0] = a(i-4), buf[1] = a(i-3), buf[2] = a(i-2),
       buf[3] = a(i-1), buf[4] = a(i)   (to be computed)    */
    seq_t buf[5];
    buf[0] = init[0];
    buf[1] = init[1];
    buf[2] = init[2];
    buf[3] = init[3];
    buf[4] = init[4];

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = 8 * buf[4] - 20 * buf[3] + 17 * buf[2] - 4 * buf[1];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = val;
    }
    return buf[4];
}
