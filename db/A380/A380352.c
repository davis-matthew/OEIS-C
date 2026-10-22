/*
 * A380352  In the ternary expansion of n, from right to left: replace the
 *          second, fourth, sixth, etc. nonzero digit, says d, by 3-d.
 * https://oeis.org/A380352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   in the ternary expansion of n, from right to left replace the *
 *            second, fourth, sixth, ... nonzero digit d by 3-d. The function
 *            returns * the resulting integer
 * Range:     u8 n <= 243, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t pow3 = 1;
    idx_t m = n;
    idx_t nonzero = 0;
    while (m) {
        idx_t d = m % 3;
        if (d != 0) {
            ++nonzero;
            if (nonzero % 2 == 0)       /* 2nd, 4th, 6th … nonzero digit */
                d = 3 - d;              /* replace 1↔2 */
        }
        if (d != 0)
            result += (seq_t)d * pow3;
        pow3 *= 3;
        m /= 3;
    }
    return result;
}
