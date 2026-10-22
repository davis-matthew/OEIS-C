/*
 * A320226  Number of integer partitions of n whose non-1 parts are all equal.
 * https://oeis.org/A320226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{r=1}^{⌊n/2⌋} (⌊n/r⌋ – 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n / 2;
    seq_t sum = 0;
    for (idx_t r = 1; r <= half; ++r) {
        sum += (seq_t)(n / r);
    }
    return sum - (seq_t)half + 1;
}
