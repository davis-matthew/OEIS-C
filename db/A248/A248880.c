/*
 * A248880  Number of tilings of an n X 1 rectangle by tiles of dimension 1 X
 *          1 and 2 X 1 such that every tile shares an equal-length edge with
 *          a tile of the same size.
 * https://oeis.org/A248880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=1, a(6)=4; for
 *            n>=7: a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-6)
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 69, u64 n <= 136,
 *            u128 n <= 270, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[7] = {1, 0, 1, 1, 2, 1, 4};
    if (n < 7) return base[n];
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i) buf[i] = base[i];
    for (idx_t i = 7; i <= n; ++i) {
        idx_t m = i % 7;
        seq_t t = buf[(i - 1) % 7];
        t += buf[(i - 2) % 7];
        t += buf[(i - 6) % 7];
        buf[m] = t - buf[(i - 3) % 7];
    }
    return buf[n % 7];
}
