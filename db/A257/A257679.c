/*
 * A257679  The smallest nonzero digit present in the factorial base
 *          representation (A007623) of n, 0 if no nonzero digits present.
 * https://oeis.org/A257679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   smallest nonzero digit in factorial base representation of n;
 *            return 0 when n==0
 * Range:     u8 n <= 5040, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t min = 0;
    idx_t tmp = n;
    for (idx_t i = 1; tmp > 0; ++i) {
        idx_t base = i + 1;
        idx_t d = tmp % base;
        tmp /= base;
        if (d != 0) {
            if (min == 0 || (seq_t)d < min) {
                min = d;
            }
        }
    }
    return min;
}
