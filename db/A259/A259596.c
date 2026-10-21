/*
 * A259596  Denominators of the other-side convergents to sqrt(7).
 * https://oeis.org/A259596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 16*a(n-4) - a(n-8) (n > 7) Initial values: a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=5, a(4)=17, a(5)=31, a(6)=48, a(7)=79
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t init[8] = {1, 2, 3, 5, 17, 31, 48, 79};
    if (n < 8) return init[n];
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i) buf[i] = init[i];
    for (idx_t k = 8; k <= n; ++k) {
        seq_t val = buf[(k - 4) & 7] * (seq_t)16 - buf[(k - 8) & 7];
        buf[k & 7] = val;
    }
    return buf[n & 7];
}
