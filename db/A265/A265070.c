/*
 * A265070  Coordination sequence for (2,6,infinity) tiling of hyperbolic
 *          plane.
 * https://oeis.org/A265070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-5) for n>6 Initial values: a(0)=1,
 *            a(1)=3, a(2)=5, a(3)=8, a(4)=13, a(5)=21, a(6)=33
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 47, u64 n <= 96, u128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* initial values for n = 0 .. 6 */
    static const seq_t init[7] = {1, 3, 5, 8, 13, 21, 33};
    if (n < 7) return init[(int)n];

    /* use a circular buffer of length 7 to keep the needed previous terms */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i) buf[i] = init[i];

    seq_t val = init[6];          /* value for n = 6, will become the answer when n == 6 */
    for (idx_t i = 7; i <= n; ++i) {
        int idx1 = (int)((i - 1) % 7);
        int idx3 = (int)((i - 3) % 7);
        int idx5 = (int)((i - 5) % 7);
        val = buf[idx1] + buf[idx3] + buf[idx5];
        buf[(int)(i % 7)] = val;
    }
    return val;
}
