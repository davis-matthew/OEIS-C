/*
 * A242963  Numbers n such that A242962(n) = sigma(n) = A000203(n).
 * https://oeis.org/A242963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n == 1) ? 5 : n + 5
 * Range:     u8 n <= 250, u16 n <= 14995, u32 n <= 14995, u64 n <= 14995,
 *            u128 n <= 14995, big any n
 * Validated: standard, pass, 82 terms through n = 7816 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)5;
    return (seq_t)n + (seq_t)5;
}
