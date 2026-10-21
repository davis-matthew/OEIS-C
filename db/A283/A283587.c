/*
 * A283587  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 654", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 256*a(n-8) - 256*a(n-9) for n>12, with a(0)=1,
 *            a(1)=3, a(2)=7, a(3)=13, a(4)=29, a(5)=53, a(6)=113, a(7)=219,
 *            a(8)=475, a(9)=859, a(10)=1819, a(11)=3515, a(12)=7595
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[13] = {
        1,   3,   7,   13,  29,  53, 113, 219,
        475, 859, 1819, 3515,7595
    };
    if (n <= 12) return init[n];

    seq_t buf[13];
    for (idx_t i = 0; i < 13; ++i) buf[i] = init[i];

    for (idx_t i = 13; i <= n; ++i) {
        idx_t j   = i % 13;
        idx_t j1  = (i - 1) % 13;
        idx_t j8  = (i - 8) % 13;
        idx_t j9  = (i - 9) % 13;
        buf[j] = buf[j1] + (seq_t)256 * buf[j8] - (seq_t)256 * buf[j9];
    }
    return buf[n % 13];
}
