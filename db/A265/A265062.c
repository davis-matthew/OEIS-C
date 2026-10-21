/*
 * A265062  Coordination sequence for (2,4,7) tiling of hyperbolic plane.
 * https://oeis.org/A265062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)-a(n-10) for n ≥ 11
 *            with initial values a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=12,
 *            a(5)=17, a(6)=25, a(7)=36, a(8)=50, a(9)=70, a(10)=98
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 62, u64 n <= 127,
 *            u128 n <= 259, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)8, (seq_t)12,
        (seq_t)17, (seq_t)25, (seq_t)36, (seq_t)50, (seq_t)70, (seq_t)98
    };
    if (n <= 10) return init[n];

    /* Circular buffer of the last 11 computed values */
    seq_t buf[11];
    for (idx_t i = 0; i <= 10; ++i) buf[i] = init[i];

    for (idx_t i = 11; i <= n; ++i) {
        seq_t val =
            buf[(i - 3) % 11] +
            buf[(i - 4) % 11] +
            buf[(i - 5) % 11] +
            buf[(i - 6) % 11] +
            buf[(i - 7) % 11] -
            buf[(i -10) % 11];
        buf[i % 11] = val;
    }
    return buf[n % 11];
}
