/*
 * A332781  a(n) = (a(n-1) XOR a(n-4)) + 1, a(0) = a(1) = a(2) = a(3) = 0.
 * https://oeis.org/A332781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = (a(n-1) XOR a(n-4)) + 1, a(0) = a(1) = a(2) = a(3) = 0
 * Range:     u8 n <= 396, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 3) {
        return (seq_t)0;
    }
    seq_t a0 = (seq_t)0, a1 = (seq_t)0, a2 = (seq_t)0, a3 = (seq_t)0, ai;
    for (idx_t i = 4; i <= n; ++i) {
        ai = (a3 ^ a0) + (seq_t)1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
