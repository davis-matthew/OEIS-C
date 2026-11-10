/*
 * A280737  a(n) = A007302(n) - 1.
 * https://oeis.org/A280737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007302(n) - 1, where A007302(n) = hammingweight(n XOR
 *            3*n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9542 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A007302_a(idx_t n) {
    unsigned __int128 x = (unsigned __int128)n ^ ((unsigned __int128)n * 3);
    seq_t count = 0;
    while (x) {
        x &= x - 1;
        count++;
    }
    return count;
}

seq_t a(idx_t n) {
    return A007302_a(n) - 1;
}
