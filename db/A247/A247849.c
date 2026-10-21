/*
 * A247849  Apollo missions that landed humans on the moon.
 * https://oeis.org/A247849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 10 + n + (n >= 3 ? 1 : 0) for 1 ≤ n ≤ 6; 0 otherwise
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1)
        return 0;
    if (n > 6)
        return 0;
    idx_t base = 10 + n;
    seq_t val = (seq_t)base;
    if (n >= 3)
        val += 1;
    return val;
}
