/*
 * A384961  Consecutive states of the linear congruential pseudo-random number
 *          generator (257*s + 41) mod 2^16 when started at s=1.
 * https://oeis.org/A384961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (257 * a(n-1) + 41) mod 2^16, with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t table[65536];
    static int initialized = 0;
    if (!initialized) {
        table[0] = 1;
        for (idx_t i = 1; i < 65536; ++i) {
            table[i] = ((table[i - 1] * (seq_t)257) + (seq_t)41) & (seq_t)0xFFFF;
        }
        initialized = 1;
    }
    idx_t idx = (n - 1) % 65536;
    return table[idx];
}
